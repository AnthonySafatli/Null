#include "Headers/Command.h"

#include "NFD/nfd.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "Headers/Program.h"
#include "Headers/Uniforms.h"
#include "Headers/Character.h"
#include "Headers/Misc.h"
#include "Headers/NoteViewer.h"

extern Program program;

Colour::Colour()                                   : error(false), r(0), g(0), b(0), a(1) {}
Colour::Colour(float r, float g, float b)          : error(false), r(r), g(g), b(b), a(1) {}
Colour::Colour(float r, float g, float b, float a) : error(false), r(r), g(g), b(b), a(a) {}

void Command::Execute(const std::string input)
{
	std::string cleanInput = Trim(input, " ");
	std::transform(cleanInput.begin(), cleanInput.end(), cleanInput.begin(), ::tolower);
	std::vector<std::string> args = Split(input, ' ');

	if (args.size() < 1)
		return;

	std::string command = args[0];
	args.erase(args.begin());

	if (command == "echo")
		Echo(args);
	else if (command == "size")
		TextSize(args);
	else if (command == "background")
		BackgroundColour(args);
	else if (command == "foreground")
		ForegroundColour(args);
	else if (command == "speed")
		CursorSpeed(args);
	else if (command == "settings")
		Settings(args);
	else if (command == "help")
		Help(args);
	else if (command == "open")
		Open(args);
	else if (command == "save")
		Save(args);
	else if (command == "rename")
		Rename(args);
	else if (command == "copy")
		Copy(args);
	else if (command == "paste")
		Paste(args);
	else if (command == "cut")
		Cut(args);
	else if (command == "undo")
		Undo(args);
	else if (command == "redo")
		Redo(args);
	else if (command == "scroll")
		Scroll(args);
	else if (command == "refresh")
		Refresh(args);
	else if (command == "note")
		Note(args);
	else if (command == "quit")
		Quit(args);
	else
		program.RenderStatus("ERROR: Unknown command " + command);
}

/* ===== Commands ====== */

void Command::Echo(const std::vector<std::string> args) 
{
	/*
	echo text ... text
	: prints text to the status bar
	*/

	std::stringstream ss;
	for (std::string str : args) ss << str + " ";

	program.RenderStatus(ss.str());
}

void Command::TextSize(const std::vector<std::string> args)
{
	/* 
	> size +
	: increases the text size by one
	:
	> size -
	: decreases the text size by one
	:
	> size n
	: sets the text size to n
	:
	> size default
	: sets the text size to the default value
	*/

	if (args.size() != 1)
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'size' does not take any arguments.");
		return;
	}

	std::string sizeStr = args[0];

	if (sizeStr == "default")
	{
		program.textSize = 24;
		UpdateUniform1f(program.openGL.u_size.location, program.textSize * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("SUCCESS: Text size set to 24");
		return;
	}
	else if (sizeStr == "+")
	{
		UpdateUniform1f(program.openGL.u_size.location, ++program.textSize * 0.001);
		program.RenderStatus("SUCCESS: Text size set to " + FloatToString(program.textSize));
		UpdateMaxHeightWidth();
		return;
	}
	else if (sizeStr == "-")
	{
		if (program.textSize <= 1)
			return;

		UpdateUniform1f(program.openGL.u_size.location, --program.textSize * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("SUCCESS: Text Size set to " + FloatToString(program.textSize));
		return;
	}

	try
	{
		float size = std::stof(sizeStr);
		program.textSize = std::abs(size);
		UpdateUniform1f(program.openGL.u_size.location, size * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("SUCCESS: Text Size set to " + FloatToString(program.textSize));
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("ARGUMENT ERROR: Argument must be a number)");
	}
}

void Command::CursorSpeed(const std::vector<std::string> args)
{
	/* 
	> speed +
	: increases the text speed by one
	:
	> speed -
	: decreases the text speed by one
	:
	> speed n
	: sets the text speed to n
	:
	> speed default
	: sets the text speed to the default value
	*/

	if (args.size() != 1)
	{
		program.RenderStatus("ARGUMENT ERROR: Command 'speed' needs 1 argument(s)");
		return;
	}

	std::string speedStr = args[0];

	if (speedStr == "default")
	{
		program.cursorSpeed = 50;
		program.RenderStatus("SUCCESS: Cursor Speed set to 50");
		return;
	}
	else if (speedStr == "+")
	{
		program.RenderStatus("SUCCESS: Cursor Speed set to " + FloatToString(++program.cursorSpeed));
		return;
	}
	else if (speedStr == "-")
	{
		if (program.cursorSpeed <= 1)
			return;

		program.RenderStatus("SUCCESS: Cursor Speed set to " + FloatToString(--program.cursorSpeed));
		return;
	}

	try
	{
		float speed = std::stof(speedStr);
		program.cursorSpeed = std::abs(speed);
		program.RenderStatus("SUCCESS: Cursor Speed set to " + FloatToString(program.cursorSpeed));
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("Invalid Argument: must be a unsigned integer");
	}
}

void Command::BackgroundColour(const std::vector<std::string> args)
{
	/* 
	> background n
	: sets r g b channels of background to n and a to 1
	:
	> background r g b
	: sets the r g b channels of the background to r g b and a to 1
	:
	> background r g b a
	: sets the r g b a channels of the background to r g b a
	:
	> background default
	: sets the background colour to the default
	:
	> background colour 
	: sets the background colour to a css colour and the a channel to 1
	:
	> background colour a 
	: sets the background colour to a css colour and the a channel to a
	*/

	Colour colour = ParseColour(args, "background", Colour(0.03, 0.05, 0.09, 0.85), program.background.a);
	program.background = colour;

	if (!colour.error)
	{
		program.background = colour;
		UpdateBackground(colour.r, colour.g, colour.b, colour.a);
	}
}

void Command::ForegroundColour(const std::vector<std::string> args)
{
	/*
	> foreground n
	: sets r g b channels of foreground to n and a to 1
	:
	> foreground r g b
	: sets the r g b channels of the foreground to r g b and a to 1
	:
	> foreground r g b a
	: sets the r g b a channels of the foreground to r g b a
	:
	> foreground default
	: sets the foreground colour to the default
	:
	> foreground colour 
	: sets the foreground colour to a css colour and the a channel to 1
	:
	> foreground colour a 
	: sets the foreground colour to a css colour and the a channel to a
	*/

	Colour colour = ParseColour(args, "foreground", Colour(1.0, 1.0, 1.0), program.foreground.a);
	program.foreground = colour;

	if (!colour.error)
	{
		program.foreground = colour;
		UpdateUniform4f(program.openGL.u_foreground.location, colour.r, colour.g, colour.b, colour.a);
	}
}

void Command::Help(const std::vector<std::string> args) 
{
	/* 
	> help
	: opens help page
	:
	> help [command]
	: opens help page for command
	*/

	if (args.size() > 1)
	{
		program.RenderStatus("ARGUMENT ERROR: Command 'help' needs 0 or 1 argument(s)");
		return;
	}
	
	if (args.size() == 0)
	{
		program.OpenTextViewer(R""""(Welcome to Null
Here is a list of all commands possible
Enter 'help [command name]' to see more details for each command

'echo'       | Prints a line of text to the status bar
'size'       | Changes the size of the text
'background' | Changes the background colour
'foreground' | Changes the foreground colour
'speed'      | Changes the speed of the cursor animation
'settings'   | Shows all the customizable settings
'help'       | Shows a description of a command
'open'       | Opens a file in the editor
'save'       | Saves a file to disk
'rename'     | Renames a file
'copy'       | Copies the line the cursor is at
'paste'      | Pastes contents of clipboard into the editor
'cut'        | Removes the line the cursor is at
'undo'       | Undos the last change made in the editor
'redo'       | Redos the last undoed change
'scroll'     | Scrolls to a certain line
'refresh'    | Refreshes the page
'note'       | Opens note menu
'quit'       | Quits the application)"""", "Help", HELP);
	}
	else
	{
		std::string help;

		if (args[0] == "echo")
		{
			help = R""""(echo:
Prints a line of text to the status bar

echo [text]
-> Prints text to status bar)"""";
		}
		else if (args[0] == "size")
		{
			help = R""""(size:
Changes the size of the text

size +
-> increases the text size by one

size -
-> decreases the text size by one

size [n]
-> sets the text size to n

size default
-> sets the text size to the default value)"""";
		}
		else if (args[0] == "background")
		{
			help = R""""(background:
Changes the background colour

background [n]
-> sets red, green, blue channels of background to n and alpha channel to 1

background [r] [g] [b]
-> sets the red, green, blue channels of the background to r g b and alpha channel to 1

background [r] [g] [b] [a]
-> sets the red, green, blue, alpha channels of the background to r g b a

background default
-> sets the background to the default value

background [colour]
-> sets the background to the css colour, the alpha channel remains unchanged

background [colour] [a]
-> sets the background to the css colour and the alpha channel to a)"""";
		}
		else if (args[0] == "foreground")
		{
			help = R""""(foreground:
Changes the foreground colour

foreground [n]
-> sets red, green, blue channels of foreground to n and alpha channel to 1

foreground [r] [g] [b]
-> sets the red, green, blue channels of the foreground to r g b and alpha channel to 1

foreground [r] [g] [b] [a]
-> sets the red, green, blue, alpha channels of the foreground to r g b a

foreground default
-> sets the foreground to the default value

foreground [colour]
-> sets the foreground to the css colour, the alpha channel remains unchanged

foreground [colour] [a]
-> sets the foreground to the css colour and the alpha channel to a)"""";
		}
		else if (args[0] == "speed")
		{
			help = R""""(speed:
Changes the speed of the cursor animation

speed +
-> increases the cursor speed by one

speed -
-> decreases the cursor speed by one

speed [n]
-> sets the cursor speed to n

speed default
-> sets the cursor speed to the default value)"""";
		}
		else if (args[0] == "settings")
		{
			help = R""""(settings:
Shows all the customizable settings

settings
-> opens settings page)"""";
		}
		else if (args[0] == "help")
		{
			help = R""""(help:
Shows a description of a command

help
-> opens help page

help [command]
-> opens help page for command)"""";
		}
		else if (args[0] == "open")
		{
			help = R""""(open:
Opens a file in the editor

open
-> opens file dialog to select file

open new
-> opens blank page in the editor)"""";
		}
		else if (args[0] == "save")
		{
			help = R""""(save:
Saves a file to disk

save
-> saves the file

save as
-> saves the file with a new name)"""";
		}
		else if (args[0] == "rename")
		{
			help = R""""(rename:
Renames a file

rename [name]
-> renames the file to name)"""";
		}
		else if (args[0] == "copy")
		{
			help = R""""(copy:
Copies the line the cursor is at

copy
-> copies the line from cursor)"""";
		}
		else if (args[0] == "paste")
		{
			help = R""""(paste:
Pastes contents of clipboard into the editor

paste
-> pastes text into new line)"""";
		}
		else if (args[0] == "cut")
		{
			help = R""""(cut:
Removes the line the cursor is at

cut
-> cuts line from cursor
)"""";
		}
		else if (args[0] == "undo")
		{
			help = R""""(undo:
Undos the last change made in the editor

undo
-> undos the last change)"""";
		}
		else if (args[0] == "redo")
		{
			help = R""""(redo:
Redos the last undoed change

redo
-> redos the last change)"""";
		}
		else if (args[0] == "scroll")
		{
			help = R""""(scroll:
Scrolls to a certain line

scroll +
-> scrolls up a line

scroll -
-> scrolls down a line

scroll [n]
-> scrolls to line n)"""";
		}
		else if (args[0] == "refresh")
		{
			help = R""""(refresh:
Refreshes the page

refresh
-> refreshes the page currently being shown)"""";
		}
		else if (args[0] == "note")
		{
			help = R""""(note:
Opens note menu

note
-> opens notebook to view notes and folders

note open [folder]
-> opens notebook to view notes and folders at folder (multiple folders to view nested folders)

note folder [name]
-> creates folder with name in the current open folder (multiple folders to view nested folders)
* folder names cannot contain spaces

note new [name]
-> creates a new note in current folder with name

note del [name]
-> deletes folder or note with name)"""";
		}
		else if (args[0] == "quit")
		{
			help = R""""(quit:
Quits the application

quit
-> quits the application)"""";
		}
		else
		{
			program.RenderStatus("ARGUEMNT ERROR: Command '" + args[0] + "' not found");
			return;
		}

		program.OpenTextViewer(help, "Help", HELP);
	}

	program.RenderStatus("SUCCESS: Help page loaded");
}

void Command::Settings(const std::vector<std::string> args)
{
	/* 
	> settings
	: opens settings page
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'settings' does not take any arguments.");
		return;
	}

	program.LoadSettings();
	program.RenderStatus("SUCCESS: Settings page loaded");
}

void Command::Open(const std::vector<std::string> args)
{
	/* 
	> open
	: Opens NFD and file selected
	: 
	> open new
	: Opens new editor
	*/
	
	if (args.size() == 0)
	{
		nfdchar_t* path;
		nfdresult_t result = NFD_OpenDialog(&path, NULL, 0, NULL);

		if (result != NFD_OKAY)
		{
			if (result != NFD_CANCEL)
				program.RenderStatus("ERROR: An error occurred while opening the file explorer");

			return;
		}

		std::stringstream ss;
		ss << path;
		program.OpenFile(ss.str());

		NFD_FreePath(path);
		return;
	}

	if (args.size() == 1 && args[0] == "new")
	{
		program.OpenEditor();
		return;
	}

	program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help open' command for more details.");
}

void Command::Save(const std::vector<std::string> args)
{
	/*
	> save
	: saves the text
	:
	> save as 
	: saves with a new name
	*/

	auto editor = dynamic_cast<TextEditor*>(program.area);
	if (editor == NULL)
		return;

	if (args.size() == 0)
	{
		if (editor->fileName == "")
		{
			SaveFile(&editor);
			return;
		}

		std::ofstream file(editor->fileDirectory);
		file << editor->GetText();

		if (SavedSuccessfully(editor->fileDirectory))
			program.RenderStatus("SUCCESS: " + editor->fileName + " saved successfully");
		else
			program.RenderStatus("ERROR: An error occurred saving " + editor->fileName);

		return;
	}

	if (args.size() == 1 && args[0] == "as")
	{
		if (editor->isNote)
		{
			SaveNote(&editor);
			return;
		}

		SaveFile(&editor);
		return;
	}

	program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help save' command for more details.");
}

void Command::Rename(const std::vector<std::string> args)
{
	/*
	> rename name
	: renames the opened file to name
	*/

	auto editor = dynamic_cast<TextEditor*>(program.area);
	if (editor == NULL)
		return;

	if (args.size() == 0)
	{
		program.RenderStatus("ARGUMENT ERROR: Command 'rename' needs 1 or more argument(s)");
		return;
	}

	std::stringstream argStr;
	for (std::string str : args) argStr << str + " ";

	try 
	{
		std::filesystem::path fileDir(editor->fileDirectory);
		std::filesystem::rename(fileDir, fileDir.parent_path() / Trim(argStr.str()));
		fileDir.replace_filename(Trim(argStr.str()));

		editor->fileDirectory = fileDir.string();
		editor->fileName = fileDir.filename().string();

		program.RenderStatus("SUCCESS: File renamed to: " + argStr.str());
	}
	catch (const std::filesystem::filesystem_error& error)
	{
		program.RenderStatus("ERROR: An error occurred renaming " + editor->fileName);
	}
}

void Command::Copy(const std::vector<std::string> args)
{
	/*
	> copy
	: copies line from cursor
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'copy' does not take any arguments.");
		return;
	}

	glfwSetClipboardString(program.window, program.area->rows[program.textY].c_str());

	program.RenderStatus("SUCCESS: Copied line " + std::to_string(program.textY + 1));
}

void Command::Paste(const std::vector<std::string> args)
{
	/*
	> pastes
	: pastes text into new line
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'paste' does not take any arguments.");
		return;
	}

	program.area->rows.insert(program.area->rows.begin() + program.textY, glfwGetClipboardString(program.window));
	Refresh(std::vector<std::string>());
	program.area->AddLeftMargin();

	program.RenderStatus("SUCCESS: Pasted to line " + std::to_string(program.textY + 1));
}

void Command::Cut(const std::vector<std::string> args)
{
	/*
	> cut
	: cuts line from cursor
	*/
	
	if (args.size() > 0)
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'cut' does not take any arguments.");
		return;
	}

	Copy(args);
	program.area->rows.erase(program.area->rows.begin() + program.textY);
	Refresh(std::vector<std::string>());
	program.area->RemoveLeftMargin();
	if (program.textY >= program.area->rows.size())
		program.textY--;
	if (program.textX > program.area->rows[program.textY].size())
		program.area->MoveEnd();

	program.RenderStatus("SUCCESS: Cut line " + std::to_string(program.textY + 1));
}

void Command::Undo(const std::vector<std::string> args)
{
	/*
	> undo
	: undos the last change
	*/

	if (args.size() > 0) 
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'undo' does not take any arguments.");
		return;
	}

	auto editor = dynamic_cast<TextEditor*>(program.area);
	if (editor == NULL)
		return;

	if (editor->undoStack.size() < 1)
		return;

	editor->redoStack.push(UndoObject(program.area->rows, program.textX, program.textY));
	program.area->rows = editor->undoStack.top().text;
	program.textX = editor->undoStack.top().textX;
	program.textY = editor->undoStack.top().textY;
	editor->undoStack.pop();

	Refresh(std::vector<std::string>());
	editor->UpdateRowColVisual();
}

void Command::Redo(const std::vector<std::string> args)
{
	/*
	> redo
	: redos the last change
	*/

	if (args.size() > 0) 
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'redo' does not take any arguments.");
		return;
	}
	
	auto editor = dynamic_cast<TextEditor*>(program.area);
	if (editor == NULL)
		return;

	if (editor->redoStack.size() < 1)
		return;

	editor->undoStack.push(UndoObject(program.area->rows, program.textX, program.textY));
	program.area->rows = editor->redoStack.top().text;
	program.textX = editor->redoStack.top().textX;
	program.textY = editor->redoStack.top().textY;
	editor->redoStack.pop();

	Refresh(std::vector<std::string>());
	editor->UpdateRowColVisual();
}

void Command::Scroll(const std::vector<std::string> args)
{
	/*
	> scroll +
	: scrolls up a line
	:
	> scroll -
	: scrolls down a line
	:
	> scroll n
	: scroll to line n
	*/

	if (args.size() < 1 || args.size() > 1) 
	{
		program.RenderStatus("ARGUMENT ERROR: Command 'scroll' needs 1 argument(s)");
		return;
	}

	if (args[0] == "+")
	{
		if (program.rowIndex > 0)
			program.rowIndex--;

		program.RenderStatus("SUCCESS: Scroll set to " + std::to_string((int)program.rowIndex + 1));
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
		return;
	}
	else if (args[0] == "-")
	{
		if (program.rowIndex + 1 < program.area->rows.size())
			program.rowIndex++;

		program.RenderStatus("SUCCESS: Scroll set to " + std::to_string((int)program.rowIndex + 1));
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
		return;
	}
	
	try 
	{
		unsigned int size = std::stof(args[0]);

		if (size < 0) size = 0;
		if (size > program.area->rows.size() - 1) size = program.area->rows.size();

		program.rowIndex = size - 1;
		program.RenderStatus("SUCCESS: Scroll set to " + std::to_string((int)program.rowIndex + 1));
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
	}
	catch (std::exception)
	{
		program.RenderStatus("ARGUMENT ERROR: Argument must be an unsigned integer");
	}

	return;
}

void Command::Refresh(const std::vector<std::string> args)
{
	/* 
	> refresh
	: Recomputes the vertices and margin vertices for the file
	*/

	auto noteViewer = dynamic_cast<NoteViewer*>(program.area);
	if (noteViewer != NULL)
	{
		std::filesystem::path documentPath = NoteViewer::GetDocumentsFolder();
		if (documentPath.empty())
		{
			// No document folder found, note command unavailable
			program.RenderStatus("ERROR: An error occurred loading the notebook");
			return;
		}

		program.OpenNoteViewer(noteViewer->folderPath);

		program.RenderStatus("SUCCESS: Refresh completed");
		return;
	}

	if (program.area->type == SETTINGS)
		program.LoadSettings();

	program.vertices.clear();

	for (int i = 0; i < program.area->rows.size(); i++)
	{
		for (int j = 0; j < program.area->rows[i].size(); j++)
		{
			TexCoords coord = GetCoords(program.area->rows[i][j]);
			program.vertices.push_back(Vertex(0.0, 0.0, coord.u, coord.v, i + 1, j + 1, NORMAL));
			program.vertices.push_back(Vertex(1.0, 0.0, coord.u + (1.0 / 10.0), coord.v, i + 1, j + 1, NORMAL));
			program.vertices.push_back(Vertex(1.0, 1.0, coord.u + (1.0 / 10.0), coord.v + (1.0 / 10.0), i + 1, j + 1, NORMAL));
			program.vertices.push_back(Vertex(0.0, 1.0, coord.u, coord.v + (1.0 / 10.0), i + 1, j + 1, NORMAL));
		}
	}

	program.marginVertices.clear();

	for (int i = 0; i < program.area->rows.size(); i++)
		program.area->AddLeftMargin();

	if (program.textY >= program.area->rows.size())
	{
		program.textY = program.area->rows.size() - 1;
		program.area->MoveEnd();
	}
	else if (program.textX >= program.area->rows[program.textY].size())
		program.area->MoveEnd();

	program.RenderStatus("SUCCESS: Refresh completed");
}

void Command::Note(const std::vector<std::string> args)
{
	/* 
	> note
	: opens notebook file viewer
	:
	> note open folder [...] folder
	: opens notebook file viewer at folder
	:
	> note folder folder [...] folder
	: creates folder with name in current folder
	:
	> note new name
	: opens new note in folder
	:
	> note del name
	: deletes folder or note
	*/

	std::filesystem::path documentsPath = NoteViewer::GetDocumentsFolder();
	if (documentsPath.empty())
	{
		// No document folder found, note command unavailable
		program.RenderStatus("ERROR: An error occurred loading the notebook");
		return;
	}

	auto noteViewer = dynamic_cast<NoteViewer*>(program.area);

	if (args.size() == 0)
	{
		program.OpenNoteViewer();
		return;
	}

	if (args[0] == "open")
	{
		if (args.size() <= 1)
		{
			program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help note' command for more details.");
			return;
		}

		std::vector<std::string> folder(args.begin() + 1, args.end());
		program.OpenNoteViewer(folder);
		return;
	}
	if (args[0] == "folder")
	{
		if (args.size() <= 1)
		{
			program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help note' command for more details.");
			return;
		}

		if (!std::filesystem::is_directory(documentsPath / "NullNotes"))
			std::filesystem::create_directory(documentsPath / "NullNotes");

		std::filesystem::path folderPath = documentsPath;
		folderPath = folderPath / "NullNotes";

		if (noteViewer != NULL)
		{
			for (int i = 0; i < noteViewer->folderPath.size(); i++)
				folderPath = folderPath / noteViewer->folderPath[i];
		}

		for (int i = 1; i < args.size(); i++)
		{
			folderPath = folderPath / args[i];
			try 
			{
				std::filesystem::create_directory(folderPath);
			}
			catch (std::exception)
			{
				program.RenderStatus("ERROR: An error occurred creating the folder");
				return;
			}
		}

		if (noteViewer != NULL) 
			Refresh(std::vector<std::string>());
		return;
	}
	if (args[0] == "new")
	{
		if (args.size() <= 1)
		{
			program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help note' command for more details.");
			return;
		}

		std::stringstream argsStr;
		for (int i = 1; i < args.size(); i++)
		{
			if (i != 1) 
				argsStr << " ";
			argsStr << args[i];
		}

		std::string noteName = argsStr.str();

		std::filesystem::path currentNotePath = documentsPath / "NullNotes";
		if (noteViewer != NULL)
		{
			for (int i = 0; i < noteViewer->folderPath.size(); i++)
				currentNotePath = currentNotePath / noteViewer->folderPath[i];
		}

		std::ofstream file(currentNotePath / noteName); 
		if (file.is_open()) 
		{ 
			file.close(); 
			Refresh(std::vector<std::string>());

			program.RenderStatus("SUCCESS: Created note successfully");
		}
		else 
			program.RenderStatus("SUCCESS: Unable to create note");
		
		return;
	}
	if (args[0] == "del")
	{
		// DEBUG: sometimes cannot find file?

		if (args.size() <= 1)
		{
			program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help note' command for more details.");
			return;
		}

		if (noteViewer == NULL)
		{
			program.RenderStatus("ERROR: Unable to complete command on this page");
			return;
		}

		std::stringstream argsStr;
		for (int i = 1; i < args.size(); i++)
		{
			if (i != 1)
				argsStr << " ";
			argsStr << args[i];
		}

		for (int i = 0; i < noteViewer->itemPaths.size(); i++)
		{
			if (noteViewer->itemPaths[i].path.filename().string() != argsStr.str())
				continue;

			try 
			{
				if (std::filesystem::is_regular_file(noteViewer->itemPaths[i].path))
				{
					std::filesystem::remove(noteViewer->itemPaths[i].path);
				}
				else if (std::filesystem::is_directory(noteViewer->itemPaths[i].path))
				{
					std::filesystem::remove_all(noteViewer->itemPaths[i].path);
				}
				else
					throw std::exception();

				Refresh(std::vector<std::string>());
				program.RenderStatus("SUCCESS: " + argsStr.str() + " deleted Successfully");
				return;
			}
			catch (const std::exception& ex) 
			{
				program.RenderStatus("ERROR: An error occurred deleting " + argsStr.str());
				return;
			}
		}

		program.RenderStatus("ARGUMENT ERROR: " + argsStr.str() + " not found");
		return;
	}
	
	program.RenderStatus("ARGUMENT ERROR: Invalid argument(s). Use 'help note' command for more details.");
}

void Command::Quit(const std::vector<std::string> args)
{
	/*
	> quit
	: quits the application
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("ARGUEMNT ERROR: Command 'quit' does not take any arguments.");
		return;
	}

	program.shouldClose = true;
}
