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
		program.RenderStatus("Error: Unknown Command " + command);
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
		program.RenderStatus("Command 'size' only takes one argument");
		return;
	}

	std::string sizeStr = args[0];

	if (sizeStr == "default")
	{
		program.textSize = 24;
		UpdateUniform1f(program.openGL.u_size.location, program.textSize * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("Text Size set to 24");
		return;
	}
	else if (sizeStr == "+")
	{
		UpdateUniform1f(program.openGL.u_size.location, ++program.textSize * 0.001);
		program.RenderStatus("Text Size set to " + FloatToString(program.textSize));
		UpdateMaxHeightWidth();
		return;
	}
	else if (sizeStr == "-")
	{
		if (program.textSize <= 1)
			return;

		UpdateUniform1f(program.openGL.u_size.location, --program.textSize * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("Text Size set to " + FloatToString(program.textSize));
		return;
	}

	try
	{
		float size = std::stof(sizeStr);
		program.textSize = std::abs(size);
		UpdateUniform1f(program.openGL.u_size.location, size * 0.001);
		UpdateMaxHeightWidth();
		program.RenderStatus("Text Size set to " + FloatToString(program.textSize));
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("Invalid Argument: must be a number");
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
		program.RenderStatus("Command 'speed' only takes one argument");
		return;
	}

	std::string speedStr = args[0];

	if (speedStr == "default")
	{
		program.cursorSpeed = 50;
		program.RenderStatus("Cursor Speed set to 50");
		return;
	}
	else if (speedStr == "+")
	{
		program.RenderStatus("Cursor Speed set to " + FloatToString(++program.cursorSpeed));
		return;
	}
	else if (speedStr == "-")
	{
		if (program.cursorSpeed <= 1)
			return;

		program.RenderStatus("Cursor Speed set to " + FloatToString(--program.cursorSpeed));
		return;
	}

	try
	{
		float speed = std::stof(speedStr);
		program.cursorSpeed = std::abs(speed);
		program.RenderStatus("Cursor Speed set to " + FloatToString(program.cursorSpeed));
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
		program.RenderStatus("Command 'status' cannot take more than one arguement");
		return;
	}
	
	if (args.size() == 0)
	{
		program.LoadHelp("");
	}
	else
	{
		program.LoadHelp("background");
	}

	program.RenderStatus("Help page loaded");
}

void Command::Settings(const std::vector<std::string> args)
{
	/* 
	> settings
	: opens settings page
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("Command 'settings' does not take any arguments");
		return;
	}

	program.LoadSettings();
	program.RenderStatus("Settings page loaded");
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
				program.RenderStatus("An error occurred opening the file dialog");

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

	program.RenderStatus("Argument(s) invalid");
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
			program.RenderStatus(editor->fileName + " saved successfully");
		else
			program.RenderStatus("Error occurred while saving " + editor->fileName);

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

	program.RenderStatus("Argument(s) invalid");
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
		program.RenderStatus("Command 'rename' needs at least one argument");
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

		program.RenderStatus("File renamed to: " + argStr.str());
	}
	catch (const std::filesystem::filesystem_error& error)
	{
		program.RenderStatus("Error renaming file: " + editor->fileName);
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
		program.RenderStatus("Command 'copy' does not take any arguments");
		return;
	}

	glfwSetClipboardString(program.window, program.area->rows[program.textY].c_str());

	program.RenderStatus("Copied line " + std::to_string(program.textY + 1));
}

void Command::Paste(const std::vector<std::string> args)
{
	/*
	> pastes
	: pastes text into new line
	*/

	program.area->rows.insert(program.area->rows.begin() + program.textY, glfwGetClipboardString(program.window));
	Refresh(std::vector<std::string>());
	program.area->AddLeftMargin();

	program.RenderStatus("Pasted to line " + std::to_string(program.textY + 1));
}

void Command::Cut(const std::vector<std::string> args)
{
	/*
	> cut
	: cuts line from cursor
	*/
	
	if (args.size() > 0)
	{
		program.RenderStatus("Command 'cut' does not take any arguments");
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

	program.RenderStatus("Cut line " + std::to_string(program.textY + 1));
}

void Command::Undo(const std::vector<std::string> args)
{
	/*
	> undo
	: undoes the last change
	*/

	if (args.size() > 0) {
		program.RenderStatus("Command 'undo' does not take any arguments");
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
	: redoes the last change
	*/

	if (args.size() > 0) {
		program.RenderStatus("Command 'redo' does not take any arguments");
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
		program.RenderStatus("Command 'scroll' takes only 1 argument");
		return;
	}

	if (args[0] == "+")
	{
		if (program.rowIndex > 0)
			program.rowIndex--;

		program.RenderStatus("Scroll set to " + std::to_string((int)program.rowIndex + 1));
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
		return;
	}
	else if (args[0] == "-")
	{
		if (program.rowIndex + 1 < program.area->rows.size())
			program.rowIndex++;

		program.RenderStatus("Scroll set to " + std::to_string((int)program.rowIndex + 1));
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
		return;
	}
	
	try 
	{
		int size = std::stof(args[0]);

		if (size < 0) size = 0;
		if (size > program.area->rows.size() - 1) size = program.area->rows.size();

		program.rowIndex = size - 1;
		program.RenderStatus("Scroll set to " + std::to_string((int)program.rowIndex + 1));
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
	}
	catch (std::exception)
	{
		program.RenderStatus("Invalid argument for command 'scroll'");
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
			program.RenderStatus("Notebook is unavailable at the moment");
			return;
		}

		program.OpenNoteViewer(noteViewer->folderPath);

		program.RenderStatus("Refresh completed");
		return;
	}

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

	program.RenderStatus("Refresh completed");
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
		program.RenderStatus("Notebook is unavailable at the moment");
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
			program.RenderStatus("Command arguments invalid");
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
			program.RenderStatus("Command arguments invalid");
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
				program.RenderStatus("Unable to create folder");
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
			program.RenderStatus("Command arguments invalid");
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

			program.RenderStatus("Created note successfully");
		}
		else 
			program.RenderStatus("Unable to create note");
		
		return;
	}
	if (args[0] == "del")
	{
		// DEBUG: sometimes cannot find file?

		if (args.size() <= 1)
		{
			program.RenderStatus("Command arguments invalid");
			return;
		}

		if (noteViewer == NULL)
		{
			program.RenderStatus("Command not available");
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
				program.RenderStatus(argsStr.str() + " deleted Successfully");
				return;
			}
			catch (const std::exception& ex) 
			{
				program.RenderStatus("Error deleting " + argsStr.str());
				return;
			}
		}

		program.RenderStatus(argsStr.str() + " not found");
		return;
	}

	program.RenderStatus("Invalid argument(s) for command 'note'");
}

void Command::Quit(const std::vector<std::string> args)
{
	/*
	> quit
	: quits the application
	*/

	if (args.size() > 0)
	{
		program.RenderStatus("Command 'quit' does not take any arguments");
		return;
	}

	program.shouldClose = true;
}
