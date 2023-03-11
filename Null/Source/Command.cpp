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

Vector<String> Split(String str, char separator);

void Command::Execute(const String fullCommand)
{
	Vector<String> args = Split(fullCommand, ' ');

	if (args.size() < 1)
		return;

	String command = args[0];
	args.erase(args.begin());

	if (command == "echo")
		Echo(args);
	else if (command == "size")
		TextSize(args[0]);
	else if (command == "background")
		BackgroundColour(args);
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
	float r, g, b, a;

	try
	{
		if (args.size() == 1)      // x
		{
			r = std::stof(args[0]);
			g = std::stof(args[0]);
			b = std::stof(args[0]);
			a = 1.0f;
		}
		else if (args.size() == 3) // r g b
		{
			r = std::stof(args[0]);
			g = std::stof(args[1]);
			b = std::stof(args[2]);
			a = 1.0f;
		}
		else if (args.size() == 4) // r g b a
		{
			r = std::stof(args[0]);
			g = std::stof(args[1]);
			b = std::stof(args[2]);
			a = std::stof(args[3]);
		}
		else 
		{
			program.RenderStatus("Invalid Argument(s): must be 1, 3, or 4 numbers");
			return;
		}
	}
	catch (const std::exception& e)
	{
		if (args[0] != "default" && args.size() != 1)
		{
			program.RenderStatus("Invalid Argument: must be number");
			return;
		}

		r = 0.03;
		g = 0.05;
		b = 0.09; 
		a = 0.85;
	}

	UpdateBackground(r, g, b, a);
}

/* ====== Misc ====== */

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