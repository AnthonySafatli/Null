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

void Command::Execute(const String fullCommand)
{
	Vector<String> args = Split(fullCommand, ' ');

	if (args.size() < 1)
		return;

	String command = args[0];
	args.erase(args.begin());

	// TODO: Update status for commands
	if (command == "echo")
		Echo(args);
	else if (command == "size")
		TextSize(args[0]);
	else if (command == "background")
		BackgroundColour(args);
	else if (command == "foreground")
		ForegroundColour(args);
	else if (command == "settings")
		Settings();
	else 
		program.RenderStatus("Error: Unknown Command " + command);
}

/* ===== Commands ====== */

void Command::Echo(const Vector<String> args)
{
	std::stringstream ss;
	for (String str : args) ss << str + " ";

	program.RenderStatus(ss.str());
}

void Command::TextSize(const String sizeStr) 
{
	float size = 0;

	try 
	{
		size = std::stof(sizeStr);
	}
	catch (const std::exception& e) 
	{
		if (sizeStr != "default")
		{
			program.RenderStatus("Invalid Argument: must be number");
			return;
		}

		size = 24;
	}

	program.textSize = size;
	UpdateUniform1f(program.openGL.u_size.location, size * 0.001);
}

void Command::BackgroundColour(const Vector<String> args) 
{
	Colour colour = ParseColour(args);

	if (colour.error)
	{
		if (args.size() == 1 && args[0] == "default")
		{
			colour.r = 0.03;
			colour.g = 0.05; 
			colour.b = 0.09;
			colour.a = 0.85;
		}
		else
		{
			program.RenderStatus("Invalid Argument(s)");
			return;
		}
	}

	UpdateBackground(colour.r, colour.g, colour.b, colour.a);
}

void Command::ForegroundColour(const Vector<String> args)
{
	Colour colour = ParseColour(args);

	if (colour.error)
	{
		if (args.size() == 1 && args[0] == "default")
		{
			colour.r = 1.0f;
			colour.g = 1.0f;
			colour.b = 1.0f;
			colour.a = 1.0f;
		}
		else
		{
			program.RenderStatus("Invalid Argument(s)");
			return;
		}
	}

	UpdateUniform4f(program.openGL.u_foreground.location, colour.r, colour.g, colour.b, colour.a);
}

void Command::Settings() 
{
	program.LoadSettings();
}

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
		else if (args.size() == 3) // r g b
		{
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[1]);
			colour.b = std::stof(args[2]);
			colour.a = 1.0f;
		}
		else if (args.size() == 4) // r g b a
		{
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[1]);
			colour.b = std::stof(args[2]);
			colour.a = std::stof(args[3]);
		}
		else
		{
			colour.error = true;
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