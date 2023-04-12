#include "Headers/Command.h"

#include "NFD/nfd.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "Headers/Program.h"
#include "Headers/Uniforms.h"
#include "Headers/Character.h"
#include "Headers/Misc.h"

extern Program program;

Colour::Colour()                                   : error(false), r(0), g(0), b(0), a(1) {}
Colour::Colour(float r, float g, float b)          : error(false), r(r), g(g), b(b), a(1) {}
Colour::Colour(float r, float g, float b, float a) : error(false), r(r), g(g), b(b), a(a) {}

void Command::Execute(const std::string input)
{
	std::string cleanInput = Trim(input, " ");
	std::transform(cleanInput.begin(), cleanInput.end(), cleanInput.begin(), ::tolower);
	std::vector<std::string> args = Split(input, ' ');

	if (args.size() < 1)
		return;

	std::string command = args[0];
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
	/*else if (command == "notebook")
		Notebook(args);*/
	else if (command == "refresh")
		Refresh(args);
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
	for (std::string str : args) ss << str + " ";

	program.RenderStatus(ss.str());
}

void Command::Refresh(const std::vector<std::string> args) 
{
	program.vertices.clear();
	
	for (int i = 0; i < program.area->rows.size(); i++)
	{
		for (int j = 0; j < program.area->rows[i].size(); j++) 
		{
			TexCoords coord = GetCoords(program.area->rows[i][j]);
			program.vertices.push_back(Vertex(0.0, 0.0, coord.u               , coord.v               , i + 1, j + 1, NORMAL));
			program.vertices.push_back(Vertex(1.0, 0.0, coord.u + (1.0 / 10.0), coord.v               , i + 1, j + 1, NORMAL));
			program.vertices.push_back(Vertex(1.0, 1.0, coord.u + (1.0 / 10.0), coord.v + (1.0 / 10.0), i + 1, j + 1, NORMAL));
			program.vertices.push_back(Vertex(0.0, 1.0, coord.u               , coord.v + (1.0 / 10.0), i + 1, j + 1, NORMAL));
		}
	}

	program.RenderStatus("Refresh completed");
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

	std::string sizeStr = args[0];

	if (sizeStr == "default")
	{
		program.textSize = 24;
		UpdateUniform1f(program.openGL.u_size.location, program.textSize * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("Text Size set to 24");
		return;
	}
	else if (sizeStr == "+")
	{
		UpdateUniform1f(program.openGL.u_size.location, ++program.textSize * 0.001);
		program.RenderStatus("Text Size set to " + std::to_string(program.textSize));
		UpdateMaxHeightWidth();
		return;
	}
	else if (sizeStr == "-")
	{
		UpdateUniform1f(program.openGL.u_size.location, --program.textSize * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("Text Size set to " + std::to_string(program.textSize));
		return;
	}

	try
	{
		float size = std::stof(sizeStr);
		program.textSize = size;
		UpdateUniform1f(program.openGL.u_size.location, size * 0.001);
		UpdateMaxHeightWidth();
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

	std::string speedStr = args[0];

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
	{
		program.background = colour;
		UpdateBackground(colour.r, colour.g, colour.b, colour.a);
	}
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
	{
		program.foreground = colour;
		UpdateUniform4f(program.openGL.u_foreground.location, colour.r, colour.g, colour.b, colour.a);
	}
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
	> open
	: Opens NFD and file selected
	: 
	> open new
	: Opens new editor
	*/
	
	if (args.size() == 0)
	{
		nfdchar_t* path;
		nfdresult_t result = NFD_OpenDialog(&path, NULL, 0, NULL);

		if (result != NFD_OKAY)
		{
			if (result != NFD_CANCEL)
				program.RenderStatus("An error occured opening the file dialog");

			return;
		}

		std::stringstream ss;
		ss << path;
		program.OpenFile(ss.str());

		NFD_FreePath(path);
		return;
	}

	if (args.size() == 1 && args[0] == "new")
	{
		program.OpenEditor();
		return;
	}

	program.RenderStatus("Command \"" + input + "\" is invalid");
}

void Command::Save(const std::vector<std::string> args, std::string input)
{
	/*
	> save
	: saves the text
	:
	> save as
	: saves with a new name
	*/

	auto editor = dynamic_cast<TextEditor*>(program.area);
	if (editor == NULL)
		return;

	if (args.size() == 0)
	{
		if (editor->fileName == "")
		{
			SaveFile(&editor);
			return;
		}

		std::ofstream file(editor->fileDirectory);
		file << editor->GetText();

		if (SavedSuccessfully(editor->fileDirectory))
			program.RenderStatus(editor->fileName + " saved successfully");
		else
			program.RenderStatus("Error occured while saving " + editor->fileName);

		return;
	}

	if (args.size() == 1 && args[0] == "as")
	{
		SaveFile(&editor);
		return;
	}

	program.RenderStatus("Command \"" + input + "\" is invalid");
}

void Command::Notebook(const std::vector<std::string> args)
{
	// TODO: Implement 'notebook' command

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
	> notebook new subfolder ... subfolder name
	: opens new text editor in subfolder with name
	:
	> notebook del name
	: deletes note
	:
	> notebook del subfolder .. subfolder name
	: deletes note in subfolder
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