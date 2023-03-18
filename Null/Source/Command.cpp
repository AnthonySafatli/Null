#include "Headers/Command.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Headers/Program.h"
#include "Headers/Uniforms.h"
#include "Headers/TextEditor.h"
#include "Headers/TextViewer.h"
#include "Headers/TextArea.h"

#define String std::string
#define Vector std::vector
#define Map std::map

extern Program program;

Colour::Colour()                                   : error(false), r(0), g(0), b(0), a(1) {}
Colour::Colour(float r, float g, float b)          : error(false), r(r), g(g), b(b), a(1) {}
Colour::Colour(float r, float g, float b, float a) : error(false), r(r), g(g), b(b), a(a) {}

Vector<String> Split(const String str, const char separator);
Colour ParseColour(const Vector<String> args, const String commandName, const Colour defaultColour, const float defaultA);
Map<String, Colour> GenerateColourMap();
String GetDir(const Vector<String> args);
bool WriteToFile(const String dir);
String GetTextFromEditor();
String RemoveLeadingWhitespace(const String str);
bool isFloat(const String number);
void PrintColour(const String commandName, const Colour colour);

void Command::Execute(const std::string input)
{
	Vector<String> args = Split(input, ' ');

	if (args.size() < 1)
		return;

	String command = args[0];
	args.erase(args.begin());

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
		Open(args, input);
	else if (command == "save")
		Save(args, input);
	else if (command == "notebook")
		Notebook(args);
	else if (command == "quit")
		Quit(args);
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
		program.cursorSpeed = 50;
		program.RenderStatus("Cursor Speed set to 50");
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

	Colour colour = ParseColour(args, "background", Colour(0.03, 0.05, 0.09, 0.85), program.background.a);
	program.background = colour;

	if (!colour.error)
		UpdateBackground(colour.r, colour.g, colour.b, colour.a);
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

	Colour colour = ParseColour(args, "foreground", Colour(1.0, 1.0, 1.0), program.foreground.a);
	program.foreground = colour;

	if (!colour.error)
		UpdateUniform4f(program.openGL.u_foreground.location, colour.r, colour.g, colour.b, colour.a);
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

void Command::Open(const std::vector<std::string> args, std::string input)
{
	/*
	> open dir
	: opens text file in dir
	:
	> open new
	: opens new editor in dir
	:
	> open new rel dir
	: opens new text file in dir
	:
	> open rel dir
	: opens text file in dir (relative path)
	:
	> open new rel dir
	: opens new text file in dir (relative path)
	*/

	if (args.size() == 0)
	{
		program.RenderStatus("Command 'open' must have at least one argument");
		return;
	}

	if (args.size() == 1 && args[0] == "new")
	{
		program.OpenEditor();
		return;
	}

	bool newCommand = args[0] == "new";
	
	auto editor = dynamic_cast<TextEditor*>(program.area);
	bool relativable = true;

	if (editor == nullptr)                           // Not in TextEditor mode
		relativable = false;
	if (relativable && editor->fileDirectory == "")  // No path to references
		relativable = false;

	String path = input.substr(input.find_first_of(newCommand ? "open new" : "open") + (newCommand ? 8 : 4)); 
	path = RemoveLeadingWhitespace(path);

	if (args[0] == "rel" || (args.size() > 1 && args.at(1) == "rel"))
	{
		if (!relativable)
		{
			program.RenderStatus("Cannot use argument 'rel', there is nothing to reference");
			return;
		}

		String subStr = path.substr(3);
		path = editor->fileDirectory + RemoveLeadingWhitespace(subStr);
	}
	else
	{
		if (path[0] != 'C' || path[1] != ':' || path[2] != '\\')
		{
			program.RenderStatus("This is not an absolute path");
			return;
		}
	}

	if (newCommand)
		program.OpenEditor("", path);
	else
		program.OpenFile(path);
}

void Command::Save(const std::vector<std::string> args, std::string input)
{
	/*
	> save
	: saves the file
	:
	> save dir
	: saves the file as dir
	:
	> save rel dir
	: saves the file as dir (realtive path)
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

	String path = input.substr(input.find_first_of("save") + (4));
	path = RemoveLeadingWhitespace(path);

	if (args[0] == "rel")
	{
		if (editor->fileDirectory == "")
		{
			program.RenderStatus("Current file location cannot be found");
			return;
		}

		String subStr = path.substr(3);
		path = editor->fileDirectory + RemoveLeadingWhitespace(subStr);
	}
	else
	{
		if (path[0] != 'C' || path[1] != ':' || path[2] != '\\')
		{
			program.RenderStatus("This is not an absolute path");
			return;
		}
	}

	Vector<String> pathVec = Split(path, '/');
	String fileName = pathVec[pathVec.size() - 1];

	if (!WriteToFile(path))
		return;

	editor->SetPath(path);

	program.RenderStatus(fileName + " saved successfully");
}

// TODO: Implement 'notebook' command
void Command::Notebook(const std::vector<std::string> args)
{
	/*
	> notebook
	: opens notebook file viewer
	:
	> notebook ..
	: opens notebook file viewer at subfolder
	:
	> notebook folder subfolder ... subfolder
	: opens notebook file viewer at folder / subfolder
	:
	> notebook folder subfolder ... file
	: opens file editor at file
	:
	> notebook new
	: opens new text editor in folder without name
	:
	> notebook new name
	: opens new text editor in current folder with name
	:
	> notebook new subfolder ... subfolder
	: opens new text editor in subfolder without name
	:
	> notebook new subfolder ... subfolder name
	: opens new text editor in subfolder with name
	*/
}

void Command::Quit(const std::vector<std::string> args)
{
	if (args.size() > 0)
	{
		program.RenderStatus("Command 'quit' can not take any arguments");
		return;
	}

	program.shouldClose = true;
}


/* 
More Possible Commands:
- quit
- undo   (ctrl + z)
- redo   (ctrl + y)
- reload (ctrl + r)
- text / keyboard commands 
*/

/* ====== Misc ====== */

Colour ParseColour(const Vector<String> args, const String commandName, const Colour defaultColour, const float defaultA)
{
	Colour colour;

	if (args.size() > 4 || args.size() < 1)
	{
		colour.error = true;
		program.RenderStatus("Command '" + commandName + "' can only take 1-4 arguments");
		return colour;
	}

	if (!isFloat(args[0]))
	{
		if (args.size() == 1 && args[0] == "default")
		{
			PrintColour(commandName, defaultColour);
			return defaultColour;
		}

		Map<String, Colour> colourMap = GenerateColourMap();
		if (colourMap.find(args[0]) != colourMap.end())
		{
			colour = colourMap[args[0]];
			colour.a = defaultA;

			if (args.size() == 2)
			{
				if (!isFloat(args[1]))
				{
					colour.error = true;
					program.RenderStatus("Error invalid argument after colour value");
					return colour;
				}

				colour.a = std::stof(args[1]);
			}

			PrintColour(commandName, colour);
			return colour;
		}

		colour.error = true;
		program.RenderStatus("Colour " + args[0] + " not found");
		return colour;
	}

	colour.error = true;
	try
	{
		if (args.size() == 1)      // x
		{
			colour.error = false;
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[0]);
			colour.b = std::stof(args[0]);
			colour.a = 1.0f;
		}
		else if (args.size() >= 3) // r g b
		{
			colour.error = false;
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[1]);
			colour.b = std::stof(args[2]);
			colour.a = 1.0f;
		}

		if (args.size() == 4)      // r g b a
		{
			colour.a = std::stof(args[3]);
		}

		if (colour.error)
		{
			program.RenderStatus("Error Invalid Argument(s)"); // invalid amount of numbers
			return colour;
		}
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("Error Invalid Argument(s)");     // not all args are numbers
		return colour;
	}

	colour.error = false;
	PrintColour(commandName, colour);
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

// Colours list https://www.w3schools.com/cssref/css_colors.php
Map<String, Colour> GenerateColourMap()
{
	std::map<String, Colour> colours;
	
	colours["aliceblue"]            = Colour(240.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0);
	colours["antiquewhite"]         = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["aqua"]                 = Colour(000.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["aquamarine"]           = Colour(127.0 / 255.0, 255.0 / 255.0, 212.0 / 255.0);
	colours["azure"]                = Colour(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["beige"]                = Colour(245.0 / 255.0, 245.0 / 255.0, 220.0 / 255.0);
	colours["bisque"]               = Colour(255.0 / 255.0, 228.0 / 255.0, 196.0 / 255.0);
	colours["black"]                = Colour(000.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["blanchedalmond"]       = Colour(255.0 / 255.0, 235.0 / 255.0, 205.0 / 255.0);
	colours["blue"]                 = Colour(000.0 / 255.0, 000.0 / 255.0, 255.0 / 255.0);
	colours["blueviolet"]           = Colour(135.0 / 255.0, 043.0 / 255.0, 226.0 / 255.0);
	colours["brown"]                = Colour(165.0 / 255.0, 042.0 / 255.0, 042.0 / 255.0);
	colours["burlywood"]            = Colour(222.0 / 255.0, 184.0 / 255.0, 135.0 / 255.0);
	colours["cadetblue"]            = Colour(095.0 / 255.0, 158.0 / 255.0, 160.0 / 255.0);
	colours["chartreuse"]           = Colour(127.0 / 255.0, 255.0 / 255.0, 000.0 / 255.0);
	colours["chocolate"]            = Colour(210.0 / 255.0, 105.0 / 255.0, 030.0 / 255.0);
	colours["coral"]                = Colour(255.0 / 255.0, 127.0 / 255.0, 080.0 / 255.0);
	colours["cornflowerblue"]       = Colour(100.0 / 255.0, 219.0 / 255.0, 237.0 / 255.0);
	colours["cornsilk"]             = Colour(250.0 / 255.0, 248.0 / 255.0, 220.0 / 255.0);
	colours["crimson"]              = Colour(220.0 / 255.0, 020.0 / 255.0, 060.0 / 255.0);
	colours["cyan"]                 = Colour(000.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["darkblue"]             = Colour(000.0 / 255.0, 000.0 / 255.0, 139.0 / 255.0);
	colours["darkcyan"]             = Colour(000.0 / 255.0, 139.0 / 255.0, 139.0 / 255.0);
	colours["darkgoldenrod"]        = Colour(184.0 / 255.0, 134.0 / 255.0, 110.0 / 255.0);
	colours["darkgrey"]             = Colour(169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0);
	colours["darkgray"]             = Colour(169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0);
	colours["darkgreen"]            = Colour(000.0 / 255.0, 100.0 / 255.0, 000.0 / 255.0);
	colours["darkkhaki"]            = Colour(189.0 / 255.0, 183.0 / 255.0, 107.0 / 255.0);
	colours["darkmagenta"]          = Colour(139.0 / 255.0, 000.0 / 255.0, 139.0 / 255.0);
	colours["darkolivegreen"]       = Colour(085.0 / 255.0, 107.0 / 255.0, 047.0 / 255.0);
	colours["darkorange"]           = Colour(255.0 / 255.0, 140.0 / 255.0, 000.0 / 255.0);
	colours["darkorchid"]           = Colour(153.0 / 255.0, 050.0 / 255.0, 204.0 / 255.0);
	colours["darkred"]              = Colour(139.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["darksalmon"]           = Colour(233.0 / 255.0, 150.0 / 255.0, 122.0 / 255.0);
	colours["darkseagreen"]         = Colour(143.0 / 255.0, 188.0 / 255.0, 143.0 / 255.0);
	colours["darkslateblue"]        = Colour(072.0 / 255.0, 061.0 / 255.0, 139.0 / 255.0);
	colours["darkslategrey"]        = Colour(047.0 / 255.0, 079.0 / 255.0, 079.0 / 255.0);
	colours["darkslategray"]        = Colour(047.0 / 255.0, 079.0 / 255.0, 079.0 / 255.0);
	colours["darkturqoise"]         = Colour(000.0 / 255.0, 206.0 / 255.0, 209.0 / 255.0);
	colours["darkviolet"]           = Colour(148.0 / 255.0, 000.0 / 255.0, 211.0 / 255.0);
	colours["deeppink"]             = Colour(255.0 / 255.0, 020.0 / 255.0, 147.0 / 255.0);
	colours["deepskyblue"]          = Colour(000.0 / 255.0, 191.0 / 255.0, 255.0 / 255.0);
	colours["dimgray"]              = Colour(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);
	colours["dimgrey"]              = Colour(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);
	colours["dodgerblue"]           = Colour(030.0 / 255.0, 144.0 / 255.0, 255.0 / 255.0);
	colours["firebrick"]            = Colour(178.0 / 255.0, 034.0 / 255.0, 034.0 / 255.0);
	colours["floralwhite"]          = Colour(255.0 / 255.0, 250.0 / 255.0, 240.0 / 255.0);
	colours["forestgreen"]          = Colour(034.0 / 255.0, 139.0 / 255.0, 034.0 / 255.0);
	colours["fuchsia"]              = Colour(255.0 / 255.0, 000.0 / 255.0, 255.0 / 255.0);
	colours["gainsboro"]            = Colour(220.0 / 255.0, 220.0 / 255.0, 220.0 / 255.0);
	colours["ghostwhite"]           = Colour(248.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0);
	colours["gold"]                 = Colour(255.0 / 255.0, 215.0 / 255.0, 000.0 / 255.0);
	colours["goldenrod"]            = Colour(218.0 / 255.0, 165.0 / 255.0, 032.0 / 255.0);
	colours["gray"]                 = Colour(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["grey"]                 = Colour(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["green"]                = Colour(000.0 / 255.0, 128.0 / 255.0, 000.0 / 255.0);
	colours["greenyellow"]          = Colour(173.0 / 255.0, 255.0 / 255.0, 047.0 / 255.0);
	colours["honeydew"]             = Colour(240.0 / 255.0, 255.0 / 255.0, 240.0 / 255.0);
	colours["hotpink"]              = Colour(255.0 / 255.0, 105.0 / 255.0, 180.0 / 255.0);
	colours["indianred"]            = Colour(205.0 / 255.0, 092.0 / 255.0, 092.0 / 255.0);
	colours["indigo"]               = Colour(075.0 / 255.0, 000.0 / 255.0, 130.0 / 255.0);
	colours["ivory"]                = Colour(255.0 / 255.0, 255.0 / 255.0, 240.0 / 255.0);
	colours["khaki"]                = Colour(240.0 / 255.0, 230.0 / 255.0, 140.0 / 255.0);
	colours["lavender"]             = Colour(230.0 / 255.0, 230.0 / 255.0, 250.0 / 255.0);
	colours["lavenderblush"]        = Colour(255.0 / 255.0, 240.0 / 255.0, 245.0 / 255.0);
	colours["lawngreen"]            = Colour(124.0 / 255.0, 252.0 / 255.0, 000.0 / 255.0);
	colours["lemonchiffon"]         = Colour(255.0 / 255.0, 250.0 / 255.0, 205.0 / 255.0);
	colours["lightblue"]            = Colour(173.0 / 255.0, 216.0 / 255.0, 230.0 / 255.0);
	colours["lightcoral"]           = Colour(240.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["lightcyan"]            = Colour(224.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["lightgoldenrodyellow"] = Colour(250.0 / 255.0, 250.0 / 255.0, 210.0 / 255.0);
	colours["lightgrey"]            = Colour(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);
	colours["lightgray"]            = Colour(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);
	colours["lightgreen"]           = Colour(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);
	colours["lightpink"]            = Colour(255.0 / 255.0, 182.0 / 255.0, 193.0 / 255.0);
	colours["lightsalmon"]          = Colour(255.0 / 255.0, 160.0 / 255.0, 122.0 / 255.0);
	colours["lightseagreen"]        = Colour(032.0 / 255.0, 178.0 / 255.0, 170.0 / 255.0);
	colours["lightskyblue"]         = Colour(135.0 / 255.0, 206.0 / 255.0, 250.0 / 255.0);
	colours["lightslategray"]       = Colour(119.0 / 255.0, 136.0 / 255.0, 153.0 / 255.0);
	colours["lightslategray"]       = Colour(119.0 / 255.0, 136.0 / 255.0, 153.0 / 255.0);
	colours["lightsleelblue"]       = Colour(176.0 / 255.0, 196.0 / 255.0, 222.0 / 255.0);
	colours["lightyellow"]          = Colour(255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0);
	colours["lime"]                 = Colour(000.0 / 255.0, 255.0 / 255.0, 000.0 / 255.0);
	colours["limegreen"]            = Colour(050.0 / 255.0, 205.0 / 255.0, 050.0 / 255.0);
	colours["linen"]                = Colour(250.0 / 255.0, 240.0 / 255.0, 230.0 / 255.0);
	colours["megenta"]              = Colour(255.0 / 255.0, 000.0 / 255.0, 255.0 / 255.0);
	colours["maroon"]               = Colour(128.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["mediumaquamarine"]     = Colour(102.0 / 255.0, 205.0 / 255.0, 170.0 / 255.0);
	colours["mediumblue"]           = Colour(000.0 / 255.0, 000.0 / 255.0, 205.0 / 255.0);
	colours["mediumorchid"]         = Colour(186.0 / 255.0, 085.0 / 255.0, 211.0 / 255.0);
	colours["mediumpurple"]         = Colour(147.0 / 255.0, 112.0 / 255.0, 219.0 / 255.0);
	colours["mediumseagreen"]       = Colour(060.0 / 255.0, 179.0 / 255.0, 113.0 / 255.0);
	colours["mediumslateblue"]      = Colour(123.0 / 255.0, 104.0 / 255.0, 238.0 / 255.0);
	colours["mediumspringgreen"]    = Colour(000.0 / 255.0, 250.0 / 255.0, 154.0 / 255.0);
	colours["mediumturquoise"]      = Colour(072.0 / 255.0, 209.0 / 255.0, 204.0 / 255.0);
	colours["mediumvioletred"]      = Colour(199.0 / 255.0, 021.0 / 255.0, 133.0 / 255.0);
	colours["midnightblue"]         = Colour(025.0 / 255.0, 025.0 / 255.0, 112.0 / 255.0);
	colours["mintcream"]            = Colour(245.0 / 255.0, 255.0 / 255.0, 250.0 / 255.0);
	colours["mistyrose"]            = Colour(255.0 / 255.0, 228.0 / 255.0, 225.0 / 255.0);
	colours["moccasin"]             = Colour(255.0 / 255.0, 228.0 / 255.0, 181.0 / 255.0);
	colours["novajowhite"]          = Colour(255.0 / 255.0, 222.0 / 255.0, 173.0 / 255.0);
	colours["navy"]                 = Colour(000.0 / 255.0, 000.0 / 255.0, 128.0 / 255.0);
	colours["oldlace"]              = Colour(253.0 / 255.0, 245.0 / 255.0, 230.0 / 255.0);
	colours["olive"]                = Colour(128.0 / 255.0, 128.0 / 255.0, 000.0 / 255.0);
	colours["olivedrap"]            = Colour(107.0 / 255.0, 142.0 / 255.0, 035.0 / 255.0);
	colours["orange"]               = Colour(255.0 / 255.0, 165.0 / 255.0, 000.0 / 255.0);
	colours["orangered"]            = Colour(255.0 / 255.0, 069.0 / 255.0, 000.0 / 255.0);
	colours["orchid"]               = Colour(218.0 / 255.0, 112.0 / 255.0, 214.0 / 255.0);
	colours["palegoldenrod"]        = Colour(238.0 / 255.0, 232.0 / 255.0, 170.0 / 255.0);
	colours["palegreen"]            = Colour(152.0 / 255.0, 251.0 / 255.0, 152.0 / 255.0);
	colours["paleturquoise"]        = Colour(175.0 / 255.0, 238.0 / 255.0, 238.0 / 255.0);
	colours["palevioletred"]        = Colour(219.0 / 255.0, 112.0 / 255.0, 147.0 / 255.0);
	colours["papayawhip"]           = Colour(255.0 / 255.0, 239.0 / 255.0, 213.0 / 255.0);
	colours["peachpuff"]            = Colour(255.0 / 255.0, 218.0 / 255.0, 185.0 / 255.0);
	colours["peru"]                 = Colour(205.0 / 255.0, 133.0 / 255.0, 063.0 / 255.0);
	colours["pink"]                 = Colour(255.0 / 255.0, 192.0 / 255.0, 203.0 / 255.0);
	colours["plum"]                 = Colour(221.0 / 255.0, 160.0 / 255.0, 221.0 / 255.0);
	colours["powderblue"]           = Colour(176.0 / 255.0, 224.0 / 255.0, 230.0 / 255.0);
	colours["purple"]               = Colour(128.0 / 255.0, 000.0 / 255.0, 128.0 / 255.0);
	colours["rebeccapurple"]        = Colour(102.0 / 255.0, 051.0 / 255.0, 153.0 / 255.0);
	colours["red"]                  = Colour(255.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["rosybrown"]            = Colour(188.0 / 255.0, 143.0 / 255.0, 143.0 / 255.0);
	colours["royalblue"]            = Colour(065.0 / 255.0, 105.0 / 255.0, 225.0 / 255.0);
	colours["saddlebrown"]          = Colour(139.0 / 255.0, 069.0 / 255.0, 019.0 / 255.0);
	colours["salmon"]               = Colour(250.0 / 255.0, 128.0 / 255.0, 114.0 / 255.0);
	colours["sandybrown"]           = Colour(244.0 / 255.0, 164.0 / 255.0, 096.0 / 255.0);
	colours["seagreen"]             = Colour(046.0 / 255.0, 139.0 / 255.0, 087.0 / 255.0);
	colours["seashell"]             = Colour(255.0 / 255.0, 245.0 / 255.0, 238.0 / 255.0);
	colours["sienna"]               = Colour(160.0 / 255.0, 082.0 / 255.0, 045.0 / 255.0);
	colours["silver"]               = Colour(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
	colours["skyblue"]              = Colour(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0);
	colours["slateblue"]            = Colour(106.0 / 255.0, 090.0 / 255.0, 205.0 / 255.0);
	colours["slategrey"]            = Colour(112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0);
	colours["slategray"]            = Colour(112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0);
	colours["snow"]                 = Colour(255.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0);
	colours["springgreen"]          = Colour(000.0 / 255.0, 255.0 / 255.0, 127.0 / 255.0);
	colours["steelblue"]            = Colour(070.0 / 255.0, 130.0 / 255.0, 180.0 / 255.0);
	colours["tan"]                  = Colour(210.0 / 255.0, 180.0 / 255.0, 140.0 / 255.0);
	colours["teal"]                 = Colour(000.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["thistle"]              = Colour(216.0 / 255.0, 191.0 / 255.0, 216.0 / 255.0);
	colours["tomato"]               = Colour(255.0 / 255.0, 099.0 / 255.0, 071.0 / 255.0);
	colours["turquoise"]            = Colour(064.0 / 255.0, 224.0 / 255.0, 208.0 / 255.0);
	colours["violet"]               = Colour(238.0 / 255.0, 130.0 / 255.0, 238.0 / 255.0);
	colours["wheat"]                = Colour(245.0 / 255.0, 222.0 / 255.0, 179.0 / 255.0);
	colours["white"]                = Colour(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["whitesmoke"]           = Colour(245.0 / 255.0, 245.0 / 255.0, 245.0 / 255.0);
	colours["yellow"]               = Colour(255.0 / 255.0, 255.0 / 255.0, 000.0 / 255.0);
	colours["yellowgreen"]          = Colour(154.0 / 255.0, 205.0 / 255.0, 050.0 / 255.0);

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

bool WriteToFile(const String path)
{
	std::ofstream outputFile(path);

	if (!outputFile.is_open()) {
		program.RenderStatus("Error opening file " + path);
		return false;
	}

	outputFile << GetTextFromEditor();
	outputFile.close();

	if (outputFile.fail()) {
		program.RenderStatus("Error writing to file " + path);
		return false;
	}

	return true;
}

String GetTextFromEditor()
{
	auto editor = dynamic_cast<TextEditor*>(program.area);

	if (editor == nullptr)
		return "";

	return editor->GetText();
}

String RemoveLeadingWhitespace(const String str) {
	String newStr(str);
	int pos = newStr.find_first_not_of(" \t\n\r\f\v");

	if (pos != std::string::npos) 
		newStr.erase(0, pos);

	return newStr;
}

bool isFloat(const String number)
{
	try 
	{
		float f = std::stof(number);
		return true;
	}
	catch (...) 
	{
		return false;
	}
}

void PrintColour(const String commandName, const Colour colour) 
{
	Vector<String> nums = { std::to_string(colour.r), std::to_string(colour.g), std::to_string(colour.b), std::to_string(colour.a) };
	std::stringstream ss;
	ss << commandName << " has been changed to ";

	for (String s : nums)
	{
		int lastZeroIndex = s.find_last_not_of("0");
		if (lastZeroIndex != std::string::npos) {
			s.erase(lastZeroIndex + 1);
		}
		else {
			s.clear();
		}

		ss << (s.size() == 0 ? "0" : s) << " ";
	}

	program.RenderStatus(ss.str());
}