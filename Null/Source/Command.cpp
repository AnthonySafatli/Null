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