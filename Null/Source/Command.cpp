#include "Headers/Command.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>

#include "Headers/Program.h"
#include "Headers/Uniforms.h"
#include "Headers/TextEditor.h"
#include "Headers/TextViewer.h"
#include "Headers/TextArea.h"

#define String std::string
#define Vector std::vector
#define Map std::map

extern Program program;

struct Colour 
{
	bool error; float r; float g; float b; float a;
	Colour(); Colour(float r, float g, float b);
};
Colour::Colour()                          : error(false), r(0), g(0), b(0), a(1) {}
Colour::Colour(float r, float g, float b) : error(false), r(r), g(g), b(b), a(1) {}

Vector<String> Split(const String str, const char separator);
Colour ParseColour(const Vector<String> args);
Map<String, Colour> GenerateColourMap();
String GetDir(const Vector<String> args);
bool WriteToFile(const String dir);
String GetTextFromEditor();

void Command::Execute(const std::string input)
{
	Vector<String> args = Split(input, ' ');

	if (args.size() < 1)
		return;

	String command = args[0];
	args.erase(args.begin());

	// TODO: Update status for commands
	if (command == "echo")
		Echo(args);
	else if (command == "size")
		TextSize(args);
	else if (command == "background")
		BackgroundColour(args);
	else if (command == "foreground")
		ForegroundColour(args);
	else if (command == "speed")
		CursorSpeed(args);
	else if (command == "settings")
		Settings(args);
	else if (command == "help")
		Help(args);
	else if (command == "open")
		Open(args);
	else if (command == "save")
		Save(args);
	else if (command == "journal")
		Journal(args);
	else
		program.RenderStatus("Error: Unknown Command " + command);
}

/* ===== Commands ====== */

void Command::Echo(const std::vector<std::string> args) 
{
	/*
	echo text ... text
	: prints text to the status bar
	*/

	std::stringstream ss;
	for (String str : args) ss << str + " ";

	program.RenderStatus(ss.str());
}
void Command::TextSize(const std::vector<std::string> args)
{
	/* 
	> size +
	: increases the text size by one
	:
	> size -
	: decreases the text size by one
	:
	> size n
	: sets the text size to n
	:
	> size default
	: sets the text size to the default value
	*/

	if (args.size() != 1)
	{
		program.RenderStatus("Command 'size' only takes one argument");
		return;
	}

	String sizeStr = args[0];

	if (sizeStr == "default")
	{
		program.textSize = 24;
		UpdateUniform1f(program.openGL.u_size.location, program.textSize * 0.001);
		program.RenderStatus("Text Size set to 24");
		return;
	}
	else if (sizeStr == "+")
	{
		UpdateUniform1f(program.openGL.u_size.location, ++program.textSize * 0.001);
		program.RenderStatus("Text Size set to " + std::to_string(program.textSize));
		return;
	}
	else if (sizeStr == "-")
	{
		UpdateUniform1f(program.openGL.u_size.location, --program.textSize * 0.001);
		program.RenderStatus("Text Size set to " + std::to_string(program.textSize));
		return;
	}

	try
	{
		float size = std::stof(sizeStr);
		program.textSize = size;
		UpdateUniform1f(program.openGL.u_size.location, size * 0.001);
		program.RenderStatus("Text Size set to " + std::to_string(program.textSize));
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("Invalid Argument: must be a number");
	}
}
void Command::CursorSpeed(const std::vector<std::string> args)
{
	/* 
	> speed +
	: increases the text speed by one
	:
	> speed -
	: decreases the text speed by one
	:
	> speed n
	: sets the text speed to n
	:
	> speed default
	: sets the text speed to the default value
	*/

	if (args.size() != 1)
	{
		program.RenderStatus("Command 'speed' only takes one argument");
		return;
	}

	String speedStr = args[0];

	if (speedStr == "default")
	{
		program.textSize = 16;
		program.RenderStatus("Cursor Speed set to 16");
		return;
	}
	else if (speedStr == "+")
	{
		program.RenderStatus("Cursor Speed set to " + std::to_string(++program.cursorSpeed));
		return;
	}
	else if (speedStr == "-")
	{
		program.RenderStatus("Cursor Speed set to " + std::to_string(--program.cursorSpeed));
		return;
	}

	try
	{
		float speed = std::stof(speedStr);
		program.cursorSpeed = speed;
		program.RenderStatus("Cursor Speed set to " + std::to_string(program.cursorSpeed));
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("Invalid Argument: must be a unsigned integer");
	}
}

// TODO: Add css colours
void Command::BackgroundColour(const std::vector<std::string> args)
{
	/* 
	> background n
	: sets r g b channels of background to n and a to 1
	:
	> background r g b
	: sets the r g b channels of the background to r g b and a to 1
	:
	> background r g b a
	: sets the r g b a channels of the background to r g b a
	:
	> background default
	: sets the background colour to the default
	:
	> background colour 
	: sets the background colour to a css colour and the a channel to 1
	:
	> background colour a 
	: sets the background colour to a css colour and the a channel to a
	*/

	if (args.size() < 4 || args.size() < 1)
	{
		program.RenderStatus("Command 'background' can only take 1-4 arguments");
		return;
	}

	if (args.size() == 1 || args.size() == 2)
	{
		if (args[0] == "default" && args.size() == 1)
		{
			UpdateBackground(0.03, 0.05, 0.09, 0.85);
			program.RenderStatus("Background set to 0.03 0.05 0.09 0.85");
			return;
		}

		std::map<String, Colour> colourMap = GenerateColourMap();

		if (colourMap.count(args[0]))
		{
			// get colour
			// check next arguement 
			// if number change a
		}
	}

	Colour colour = ParseColour(args);
	if (colour.error)
	{
		program.RenderStatus("Command 'background' only takes numbers as arguments");
		return;
	}

	UpdateBackground(colour.r, colour.g, colour.b, colour.a);
	program.RenderStatus("Background set to " + std::to_string(colour.r) + " " + std::to_string(colour.g) + " " + std::to_string(colour.b) + " " + std::to_string(colour.a));
}
void Command::ForegroundColour(const std::vector<std::string> args)
{
	/*
	> foreground n
	: sets r g b channels of foreground to n and a to 1
	:
	> foreground r g b
	: sets the r g b channels of the foreground to r g b and a to 1
	:
	> foreground r g b a
	: sets the r g b a channels of the foreground to r g b a
	:
	> foreground default
	: sets the foreground colour to the default
	:
	> foreground colour 
	: sets the foreground colour to a css colour and the a channel to 1
	:
	> foreground colour a 
	: sets the foreground colour to a css colour and the a channel to a
	*/

	if (args.size() != 1 && args.size() != 3 && args.size() != 4)
	{
		program.RenderStatus("Command 'foreground' takes 1 3 or 4 arguments");
		return;
	}

	if (args.size() == 1 && args[0] == "default")
	{
		UpdateUniform4f(program.openGL.u_foreground.location, 1.0, 1.0, 1.0, 1.0);
		program.RenderStatus("Foreground set to 1.0 1.0 1.0 1.0");
		return;
	}

	Colour colour = ParseColour(args);
	if (colour.error)
	{
		program.RenderStatus("Command 'foreground' only takes numbers as arguments");
		return;
	}

	UpdateUniform4f(program.openGL.u_foreground.location, colour.r, colour.g, colour.b, colour.a);
	program.RenderStatus("Foreground set to " + std::to_string(colour.r) + " " + std::to_string(colour.g) + " " + std::to_string(colour.b) + " " + std::to_string(colour.a));
}


void Command::Help(const std::vector<std::string> args) 
{
	/* 
	> help
	: opens help page
	:
	> help command
	: opens help page with command filter
	: 
	> help shortcut
	: opens help page with shortcuts filter
	*/

	if (args.size() > 1)
	{
		program.RenderStatus("Command 'status' cannot take more than one arguement");
		return;
	}
	
	if (args.size() == 1)
	{
		if (args[0] == "command")
		{
			program.LoadHelp(true, false);
			program.RenderStatus("Help page loaded");
			return;
		}

		if (args[0] == "shortcut")
		{
			program.LoadHelp(false, true);
			program.RenderStatus("Help page loaded");
			return;
		}

		program.RenderStatus("Command 'help' can only take 'command' or 'shortcut' as arguments");
		return;
	}

	program.LoadHelp(true, true);
	program.RenderStatus("Help page loaded");
}
void Command::Settings(const std::vector<std::string> args)
{
	/* 
	> settings
	: opens settings page
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("Command 'settings' does not take any arguments");
		return;
	}

	program.LoadSettings();
	program.RenderStatus("Settings page loaded");
}

// TODO: Rethink path input

// TODO: Make error messages
void Command::Open(const std::vector<std::string> args)
{
	/*
	> open new
	: opens new editor
	:
	> open dir
	: opens new text file in dir
	*/

	if (args.size() != 1)
	{
		// error msg
		return;
	}

	if (args[0] == "new")
	{
		program.OpenEditor();
		return;
	}

	if (args[0][0] != 'C' || args[0][1] != ':' || args[0][2] != '\\')
	{
		// errornot valid path
		return;
	}

	std::ifstream file(args[0]);
	if (!file.is_open()) 
	{
		// error 
		return;
	}
	String str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	Vector<String> dirVec = Split(args[0], '\\');
	String fileName = dirVec[dirVec.size() - 1];
	std::stringstream ss;
	for (int i = 0; i < dirVec.size() - 1; i++) ss << dirVec[i] + '\\';

	program.OpenEditor(str, ss.str(), fileName);
}
// TODO: Make good error messages
void Command::Save(const std::vector<std::string> args)
{
	/*
	> save
	: saves the file
	:
	> save rel path
	: saves the file with the relative path
	:
	> save abs path
	: saves the file in the absolute path
	*/

	auto editor = dynamic_cast<TextEditor*>(program.area);

	if (editor == nullptr) // Not in TextEditor mode
		return;

	if (args.size() == 0)
	{
		if (editor->fileName == "")
		{
			program.RenderStatus("File path unkown");
			return;
		}

		if (!WriteToFile(editor->fileDirectory + editor->fileName))
		{
			program.RenderStatus("Error: " + editor->fileName + " failed to save successfully");
			return;
		}

		program.RenderStatus(editor->fileName + " saved successfully");
		return;
	}

	String dir = GetDir(args);

	if (dir == "")
	{
		program.RenderStatus("Error: Invalid Arguments");
		return;
	}

	Vector<String> dirVec = Split(dir, '\\');
	String fileName = dirVec[dirVec.size() -1]; 

	if (!WriteToFile(dir))
	{
		program.RenderStatus("Error: " + fileName + " failed to save successfully");
		return;
	}

	std::stringstream ss;
	for (int i = 0; i < dirVec.size() - 1; i++) ss << dirVec[i] + '\\';
	editor->fileDirectory = ss.str();

	editor->fileName = fileName;

	program.RenderStatus(fileName + " saved successfully");
}

// TODO: Implement 'journal' command
void Command::Journal(const std::vector<std::string> args)
{
	/*
	> journal
	: opens journal file viewer
	:
	> journal ..
	: opens journal file viewer at subfolder
	:
	> journal folder subfolder ... subfolder
	: opens journal file viewer at folder / subfolder
	:
	> journal folder subfolder ... file
	: opens file editor at file
	:
	> journal new
	: opens new text editor in folder without name
	:
	> journal new name
	: opens new text editor in current folder with name
	:
	> journal new subfolder ... subfolder
	: opens new text editor in subfolder without name
	:
	> journal new subfolder ... subfolder name
	: opens new text editor in subfolder with name
	*/
}

/* 
More Possible Commands:
- undo   (ctrl + z)
- redo   (ctrl + y)
- reload (ctrl + r)
- text / keyboard commands 
*/

/* ====== Misc ====== */

Colour ParseColour(const Vector<String> args)
{
	Colour colour;
	colour.error = false;

	try
	{
		if (args.size() == 1)      // x
		{
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[0]);
			colour.b = std::stof(args[0]);
			colour.a = 1.0f;
		}
		else                       // r g b
		{
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[1]);
			colour.b = std::stof(args[2]);
			colour.a = 1.0f;
		}
		
		if (args.size() == 4)      // r g b a
		{
			colour.a = std::stof(args[3]);
		}
	}
	catch (const std::exception& e)
	{
		colour.error = true;
	}

	return colour;
}

Vector<String> Split(const String str, const char separator)
{
	Vector<String> strings;

	std::stringstream ss;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != separator)
		{
			ss << str[i];
			continue;
		}
		
		if (ss.str().size() > 0)
		{
			strings.push_back(ss.str());
			ss.clear();
			ss.str("");
		}
	}

	if (ss.str().size() > 0)
		strings.push_back(ss.str());

	return strings;
}

// TODO: Complete colours https://www.w3schools.com/cssref/css_colors.php
Map<String, Colour> GenerateColourMap()
{
	std::map<String, Colour> colours;
	
	colours["aliceblue"]            = Colour(240.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0);
	colours["antiquewhite"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["aqua"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["aquamarine"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["azure"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["beige"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["bisque"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["black"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["blanchedalmond"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["blue"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["blueviolet"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["brown"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["burlywood"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["cadetblue"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["chartreuse"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["chocolate"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["coral"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["cornflowerblue"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["cornsilk"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["crimson"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["cyan"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkblue"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkcyan"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkgoldenrod"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkgrey"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkgray"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkgreen"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkkhaki"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkmagenta"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkolivegreen"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkorange"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkorchid"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkred"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darksalmon"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkseagreen"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkslateblue"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkslategrey"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkslategray"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkturqoise"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["darkviolet"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["deeppink"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["deepskyblue"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["dimgray"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["dimgrey"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["dodgerblue"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["firebrick"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["floralwhite"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["forestgreen"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["fuchsia"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["gainsboro"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["ghostwhite"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["gold"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["goldenrod"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["gray"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["grey"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["green"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["greenyellow"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["honeydew"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["hotpink"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["indianred"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["indigo"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["ivory"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["khaki"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lavender"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lavenderblush"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lawngreen"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lemonchiffon"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightblue"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightcoral"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightcyan"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightgoldenrodyellow"] = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightgrey"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightgray"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightgreen"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightpink"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightsalmon"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightseagreen"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightskyblue"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightslategray"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightslategray"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightsleelblue"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lightyellow"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["lime"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["limegreen"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["linen"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["megenta"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["maroon"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumaquamarine"]     = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumblue"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumorchid"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumpurple"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumseagreen"]       = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumslateblue"]      = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumspringgreen"]    = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumturquoise"]      = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mediumvioletred"]      = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["midnightblue"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mintcream"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["mistyrose"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["moccasin"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["novajowhite"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["navy"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["oldlace"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["olive"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["olivedrap"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["orange"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["orangered"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["orchid"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["palegoldenrod"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["palegreen"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["paleturquoise"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["palevioletred"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["papayawhip"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["peachpuff"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["peru"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["pink"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["plum"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["powderblue"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["purple"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["rebeccapurple"]        = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["red"]                  = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["rosybrown"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["royalblue"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["saddlebrown"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["salmon"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["sandybrown"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["seagreen"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["seashell"]             = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["sienna"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["silver"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["skyblue"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["slateblue"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["slategrey"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["slategray"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["snow"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["springgreen"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["steelblue"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["tan"]                  = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["teal"]                 = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["thistle"]              = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["tomato"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["turquoise"]            = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["violet"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["wheat"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["white"]                = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["whitesmoke"]           = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["yellow"]               = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["yellowgreen"]          = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);

	return colours;
}

String GetDir(const Vector<String> args)
{
	if (args.size() != 2)
		return "";

	if (args[0] == "abs") // absolute path
	{
		if (args[1][0] != 'C' || args[1][1] != ':' || args[1][2] != '\\')
			return "";
			
		return args[1];
	}

	if (args[0] == "rel") // relative path
	{
		auto editor = dynamic_cast<TextEditor*>(program.area);

		if (editor->fileDirectory == "")
			return "";

		return editor->fileDirectory + args[1];
	}

	return "";
}

bool WriteToFile(const String dir)
{
	try
	{
		std::ofstream out(dir);

		if (out.bad())
			return false;

		out << GetTextFromEditor();
	}
	catch (const std::exception& e)
	{
		return false;
	}

	std::ifstream file(dir);
	return file.good(); 
}

String GetTextFromEditor()
{
	auto editor = dynamic_cast<TextEditor*>(program.area);

	if (editor == nullptr)
		return "";

	return editor->GetText();
}