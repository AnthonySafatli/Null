#include "Headers/Command.h"

#include <string>
#include <vector>

#include "Headers/Program.h"

extern Program program;


void Command::Execute(const std::string command)
{
	program.RenderStatus("Command: " + command);
}


std::vector<std::string> Split(std::string str, char separator)
{
	return std::vector<std::string>();
}