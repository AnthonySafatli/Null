#pragma once

#include <string>
#include <vector>

class Command 
{
private:
	Command();

	/* Commands */
	static void Echo(const std::vector<std::string> args);

public:
	static void Execute(const std::string command);

};