#include "Headers/NoteViewer.h"

#include <filesystem>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <shlobj.h>
#include <chrono>
#include <ctime>

#include "GLFW/glfw3.h"

#include "Headers/TextArea.h"
#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

NoteItem::NoteItem(bool isFile, std::filesystem::path path) : isFile(isFile), path(path) { }

NoteViewer::NoteViewer(std::filesystem::path documentPath) : isRoot(true), folderPath(), locatingError(false)
{
	type = NOTE;

	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	ConstructorStart(3, false);

	std::string path = documentPath.string() + "\\NullNotes";
	itemPaths = GetAllPaths(path);

	for (char c : path) AddCharacter(c);
	Return(); Return();

	PrintPaths();

	ConstructorEnd();
	
	program.textY = 2;
	RemoveDirectoryArrows();

	UpdateArrow();
}

NoteViewer::NoteViewer(std::filesystem::path documentPath, std::vector<std::string> folders) : folderPath(folders), locatingError(false)
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	isRoot = folders.size() == 0;

	ConstructorStart(3, false);

	std::filesystem::path currentPath = documentPath / "NullNotes";
	for (std::string folder : folders)
		currentPath = currentPath / folder;

	if (std::filesystem::is_directory(currentPath))
	{
		std::stringstream folderNameStream = std::stringstream();
		folderNameStream << "\\NullNotes";
		for (std::string folder : folders) folderNameStream << "\\" << folder;
		std::string path = documentPath.string() + folderNameStream.str();
		itemPaths = GetAllPaths(path);

		for (char c : path) AddCharacter(c);
		Return(); Return();
		
		if (!isRoot)
		{
			AddCharacter('.');
			AddCharacter('.');
			Return();
		}

		PrintPaths();
	}
	else
	{
		for (char c : documentPath.string()) AddCharacter(c);
		AddCharacter('\\'); AddCharacter('~');
		Return(); Return();

		if (!isRoot)
		{
			AddCharacter('.');
			AddCharacter('.');
			Return();
		}

		const std::string message = "Error Locating Folder...";
		for (int i = 0; i < message.size(); i++) AddCharacter(message[i]);

		locatingError = true;
	}

	ConstructorEnd();	

	program.textY = 2;
	RemoveDirectoryArrows();
	
	UpdateArrow();
}

void NoteViewer::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
		return;

	if (program.textY == (unsigned int)rows.size())
		return;

	switch (key)
	{
	case GLFW_KEY_UP:
		if (program.textY == 2) 
			break;
		
		MoveUp();
		break;
	case GLFW_KEY_DOWN:
		MoveDown();
		break;
	case GLFW_KEY_ENTER:
		// DEBUG: Bug fix, cannot open test.note
		OpenItem();
		return;
	}

	UpdateArrow();
}

void NoteViewer::AddLeftMargin()
{
	AddCharacterToMargin(ARROW_START, -2, MARGIN);
	AddCharacterToMargin(NOT_ARROW_END, -1, MARGIN);
}

void NoteViewer::RemoveLeftMargin()
{
	program.marginVertices.resize(program.marginVertices.size() - (4 * (leftMargin - 1)));
}

std::filesystem::path NoteViewer::GetDocumentsFolder()
{

#ifdef _WIN32

	std::filesystem::path documentsPath;

	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		documentsPath = path;
		return documentsPath;
	}
	else
		return std::filesystem::path();

#else
	
	const char* homeDir = std::getenv("HOME");
	if (homeDir == nullptr)
		return std::filesystem::path();
	else
		return std::filesystem::path(homeDir) / "Documents";

#endif

}

std::vector<NoteItem> NoteViewer::GetAllPaths(const std::string initPath)
{
	if (!std::filesystem::is_directory(initPath))
		return std::vector<NoteItem>();

	std::vector<NoteItem> paths;

	for (const auto& entry : std::filesystem::directory_iterator(initPath)) 
	{
		if (std::filesystem::is_regular_file(entry.path()))
			paths.push_back(NoteItem(true, entry.path()));
		else if (std::filesystem::is_directory(entry.path()))
			paths.push_back(NoteItem(false, entry.path()));
	} 

	return paths;
}

void NoteViewer::PrintPath(std::filesystem::path path, bool isFile)
{
	std::string pathStr = path.filename().string();

	if (!isFile)
	{
		AddCharacter(FOLDER_ICON_START);
		AddCharacter(FOLDER_ICON_END);
	}

	for (int i = 0; i < pathStr.size(); i++)
		AddCharacter(pathStr[i]);
	Return();
	return;
}

void NoteViewer::PrintPaths()
{
	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (!itemPaths[i].isFile)
			PrintPath(itemPaths[i].path, false);
	}
	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (itemPaths[i].isFile)
			PrintPath(itemPaths[i].path, true);
	}

	if (itemPaths.size() == 0)
	{
		const std::string message = "No Notes or Folders...";
		for (int i = 0; i < message.size(); i++) AddCharacter(message[i]);
		Return();
	}
	RemoveCharacterFromLeft();
}

void NoteViewer::UpdateArrow()
{
	TexCoords spaceCoords = GetCoords(NOT_ARROW_END);
	for (int i = 2; i < program.marginVertices.size() / 8; i++)
	{
		program.marginVertices[(i * 8) + 4].texCoords[0] = spaceCoords.u;
		program.marginVertices[(i * 8) + 4].texCoords[1] = spaceCoords.v;
		program.marginVertices[(i * 8) + 5].texCoords[0] = spaceCoords.u + (1.0 / 10.0);
		program.marginVertices[(i * 8) + 5].texCoords[1] = spaceCoords.v;
		program.marginVertices[(i * 8) + 6].texCoords[0] = spaceCoords.u + (1.0 / 10.0);
		program.marginVertices[(i * 8) + 6].texCoords[1] = spaceCoords.v + (1.0 / 10.0);
		program.marginVertices[(i * 8) + 7].texCoords[0] = spaceCoords.u;
		program.marginVertices[(i * 8) + 7].texCoords[1] = spaceCoords.v + (1.0 / 10.0);
	}

	TexCoords arrowCoords = GetCoords(ARROW_END);
	int arrowIndex = program.textY * 8;
	program.marginVertices[arrowIndex + 4].texCoords[0] = arrowCoords.u;
	program.marginVertices[arrowIndex + 4].texCoords[1] = arrowCoords.v;
	program.marginVertices[arrowIndex + 5].texCoords[0] = arrowCoords.u + (1.0 / 10.0);
	program.marginVertices[arrowIndex + 5].texCoords[1] = arrowCoords.v;
	program.marginVertices[arrowIndex + 6].texCoords[0] = arrowCoords.u + (1.0 / 10.0);
	program.marginVertices[arrowIndex + 6].texCoords[1] = arrowCoords.v + (1.0 / 10.0);
	program.marginVertices[arrowIndex + 7].texCoords[0] = arrowCoords.u;
	program.marginVertices[arrowIndex + 7].texCoords[1] = arrowCoords.v + (1.0 / 10.0);

	UpdateUniform1i(program.openGL.u_cursorRow.location, program.textY + 1);
}

void NoteViewer::OpenItem()
{
	int cursorIndex = program.textY - 2;
	if (!isRoot)
		cursorIndex--;

	if (!isRoot && (cursorIndex < 0))
	{
		// ..
#if _DEBUG
		std::cout << "Go back" << std::endl;
#endif

		folderPath.pop_back();
		if (folderPath.size() == 0)
			program.OpenNoteViewer();
		else if (locatingError)
			program.OpenNoteViewer();
		else
			program.OpenNoteViewer(folderPath);

		return;
	}

	int counter = 0;
	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (itemPaths[i].isFile)
			continue;

		if (!std::filesystem::is_directory(itemPaths[i].path))
		{
			Command::Execute("refresh");
			program.RenderStatus("ERROR: File not found");
			return;
		}

		if (counter == cursorIndex)
		{

#if _DEBUG
			std::cout << "Open Folder: " << itemPaths[i].path.string() << std::endl;
#endif

			folderPath.push_back(itemPaths[i].path.filename().string());
			program.OpenNoteViewer(folderPath);
			return;
		}
		counter++;
	}

	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (!itemPaths[i].isFile)
			continue;

		if (!std::filesystem::is_regular_file(itemPaths[i].path))
		{
			Command::Execute("refresh");
			program.RenderStatus("ERROR: File not found");
			return;
		}

		if (counter == cursorIndex)
		{

#if _DEBUG
			std::cout << "Open Note: " << itemPaths[i].path.string() << std::endl;
#endif

			program.OpenNote(itemPaths[i].path, itemPaths[i].path.filename().string());

			return;
		}
		counter++;
	}
}

void NoteViewer::RemoveDirectoryArrows()
{
	TexCoords space = GetCoords(' ');
	for (int i = 0; i < 16; i++)
	{
		program.marginVertices[i].texCoords[0] = space.u;
		program.marginVertices[i].texCoords[1] = space.v;
	}
}
