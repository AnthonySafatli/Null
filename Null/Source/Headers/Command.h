#pragma once

#include <string>
#include <vector>

class Command 
{
private:
	Command();

	/* Commands */
	static void Echo(const std::vector<std::string> args);
	static void TextSize(const std::string sizeStr);
	static void BackgroundColour(const std::vector<std::string> args);
	static void ForegroundColour(const std::vector<std::string> args);

public:
	static void Execute(const std::string command);

};