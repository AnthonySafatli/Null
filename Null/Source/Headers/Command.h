#pragma once

#include <string>
#include <vector>

class Command 
{
private:
	Command();

	/* Commands */
	static void Echo(const std::vector<std::string> args);                    // prints text in status bar

	/* Change Settings */
	static void TextSize(const std::vector<std::string> args);                // changes text size
	static void BackgroundColour(const std::vector<std::string> args);        // changes background colour
	static void ForegroundColour(const std::vector<std::string> args);        // changes foreground colour
	static void CursorSpeed(const std::vector<std::string> args);             // changes cursor speed
	
	/* Opens Pages */
	static void Help(const std::vector<std::string> args);                    // opens help page
	static void Settings(const std::vector<std::string> args);                // opens settings page
	
	/* Files */
	static void Open(const std::vector<std::string> args, std::string input); // opens files and editor
	static void Save(const std::vector<std::string> args, std::string input); // saves files
	
	/* Notebook */
	static void Notebook(const std::vector<std::string> args);                // deals with notebook

	/* Quit */
	static void Quit(const std::vector<std::string> args);                    // closes application


public:
	static void Execute(const std::string input);

};