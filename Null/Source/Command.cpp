#include "Headers/Command.h"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Headers/Program.h"

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
	{
		Echo(args);
	}
	else 
	{
		program.RenderStatus("Error: Unknown Command " + command);
	}
}

/* ===== Commands ====== */

void Command::Echo(const Vector<String> args)
{
	std::stringstream ss;
	for (String str : args) ss << str + " ";

	program.RenderStatus(ss.str());
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