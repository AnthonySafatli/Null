#pragma once

#include <string>

class Command 
{
private:
	Command();

public:
	// List of commands (static)
	static void Execute(const std::string command);
};