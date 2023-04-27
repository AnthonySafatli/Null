#pragma once

#include <string>
#include <vector>

class Command 
{
private:
	Command();

	/* Misc */
	static void Echo(const std::vector<std::string> args);                    // prints text in status bar

	/* Settings */
	static void TextSize(const std::vector<std::string> args);                // changes text size
	static void CursorSpeed(const std::vector<std::string> args);             // changes cursor speed
	static void BackgroundColour(const std::vector<std::string> args);        // changes background colour
	static void ForegroundColour(const std::vector<std::string> args);        // changes foreground colour
	
	/* Pages */
	static void Help(const std::vector<std::string> args);                    // opens help page
	static void Settings(const std::vector<std::string> args);                // opens settings page
	
	/* Files */
	static void Open(const std::vector<std::string> args, std::string input); // opens files and editor
	static void Save(const std::vector<std::string> args, std::string input); // saves files
	
	/* Clipboard */
	static void Copy(const std::vector<std::string> args);                    // Copies line the cursor is on into the clipboard
	static void Paste(const std::vector<std::string> args);                   // Pastes texts in a new line above the cursor
	static void Cut(const std::vector<std::string> args);                     // Removes the line the cursor is on and copies it to clipboard

	/* Undo */
	static void Undo(const std::vector<std::string> args);                    // Undoes the last change of command
	static void Redo(const std::vector<std::string> args);                    // Redoes the last change or command 

	/* Scroll */
	static void Scroll(const std::vector<std::string> args);                  // Scrolls to a line

	/* Refresh */
	static void Refresh(const std::vector<std::string> args);                 // recreates all the vertices based on the text

	/* Note */
	static void Note(const std::vector<std::string> args);                    // deals with notebook

	/* Quit */
	static void Quit(const std::vector<std::string> args);                    // closes application


public:
	static void Execute(const std::string input);

};

struct Colour
{
	bool error; float r; float g; float b; float a;
	Colour(); Colour(float r, float g, float b); Colour(float r, float g, float b, float a);
};