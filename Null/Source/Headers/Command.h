#pragma once

#include <string>
#include <vector>

class Command 
{
private:
	Command();

	/* Commands */
	static void Echo(const std::vector<std::string> args);             // prints text in status bar

	/* Change Settings */
	static void TextSize(const std::string sizeStr);                   // changes text size
	// TODO: have + or - be arguments
	static void BackgroundColour(const std::vector<std::string> args); // changes background colour
	static void ForegroundColour(const std::vector<std::string> args); // changes foreground colour
	
	/* Opens Pages */
	static void Journal(const std::vector<std::string> args);          // open the journal viewer or journal page / folder
	static void Settings();                                            // opens settings viewer
	static void Properties(const std::vector<std::string> args);       // opens properties page for specified file or current file
	static void Help();                                                // opens help viewer
	
	/* Text Editor */
	static void Open(const std::string);                               // opens new file at specified location

public:
	static void Execute(const std::string command);

};