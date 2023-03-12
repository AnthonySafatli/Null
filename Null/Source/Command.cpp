#include "Headers/Command.h"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Headers/Program.h"
#include "Headers/Uniforms.h"

#define String std::string
#define Vector std::vector

extern Program program;

struct Colour 
{
	bool error; float r; float g; float b; float a;
};

Vector<String> Split(String str, char separator);
Colour ParseColour(Vector<String> args);

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
	else if (command == "journal")
		Journal(args);
	else
		program.RenderStatus("Error: Unknown Command " + command);
}

/* ===== Commands ====== */

// done
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

// done
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

// TODO: CSS colours?
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

	if (args.size() != 1 && args.size() != 3 && args.size() != 4)
	{
		program.RenderStatus("Command 'background' takes 1 3 or 4 arguments");
		return;
	}

	if (args.size() == 1 && args[0] == "default")
	{
		UpdateBackground(0.03, 0.05, 0.09, 0.85);
		program.RenderStatus("Background set to 0.03 0.05 0.09 0.85");
		return;
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

// TODO: CSS colours?
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

// done
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

// TODO: Status dar message for 'help' command
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
			return;
		}

		if (args[0] == "shortcut")
		{
			program.LoadHelp(false, true);
			return;
		}

		program.RenderStatus("Command 'help' can only take 'command' or 'shortcut' as arguments");
		return;
	}

	program.LoadHelp(true, true);
}

// TODO: Status dar message for 'settings' command
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
}

// TODO: Implement 'open' command
void Command::Open(const std::vector<std::string> args)
{
	/*
	> open new
	: opens new editor
	:
	> open dir
	: opens new text file in dir
	:
	> open location name
	: opens new text file in location (eg Desktop) named name
	:
	*/
}

// TODO: Implement 'save' command
void Command::Save(const std::vector<std::string> args)
{
	/*
	> save
	: saves the file
	:
	> save as name
	: saves the file with the name name
	:
	> save as dir
	: saves the file in dir with name
	:
	> save as location name
	: saves the file in the location as name
	*/
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

Colour ParseColour(Vector<String> args)
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

Vector<String> Split(String str, char separator)
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