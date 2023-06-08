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

#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

// TODO: Make sure to check if user deletes of changes files after validation

NoteViewer::NoteViewer(std::filesystem::path documentPath) : isRoot(true), folderPath()
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	ConstructorStart(3, false);

	itemPaths = GetAllPaths(documentPath.string() + "\\NullNotes");

	PrintPaths();

	ConstructorEnd();

	UpdateArrow();
}

NoteViewer::NoteViewer(std::filesystem::path documentPath, std::vector<std::string> folders) : isRoot(false), folderPath(folders)
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	ConstructorStart(3, false);

	std::stringstream folderNameStream = std::stringstream();
	folderNameStream << "\\NullNotes";
	for (std::string folder : folders) folderNameStream << "\\" << folder;
	itemPaths = GetAllPaths(documentPath.string() + folderNameStream.str());

	AddCharacter('.');
	AddCharacter('.');
	Return();

	PrintPaths();

	ConstructorEnd();

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
		MoveUp();
		break;
	case GLFW_KEY_DOWN:
		MoveDown();
		break;
	case GLFW_KEY_ENTER:
		// TODO: Bug fix, cannot open test.note
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

std::string NoteViewer::GetNoteName(std::string name)
{
	auto current_time = std::chrono::system_clock::now();
	std::time_t time_t_now = std::chrono::system_clock::to_time_t(current_time);
	std::tm time_now;
	localtime_s(&time_now, &time_t_now);

	int year = time_now.tm_year + 1900; 
	int month = time_now.tm_mon;        
	int day = time_now.tm_mday - 1;         
	int hour = time_now.tm_hour;        
	int minute = time_now.tm_min;       

	std::stringstream nameStream;
	nameStream << (char)(hour + 65);
	nameStream << (char)((int)((minute / 10) % 10) + 65) << (char)((minute % 10) + 65);
	nameStream << name;
	nameStream << (char)(month + 65);
	nameStream << ((day > 9) ? ((char)(day + 55)) : ((char)(day + 48)));
	nameStream << (char)((int)((year / 1000) % 10) + 65) << (char)((int)((year / 100) % 10) + 65) << (char)((int)((year / 10) % 10) + 65) << (char)((year % 10) + 65);

	return nameStream.str();
}

std::vector<std::filesystem::path> NoteViewer::GetAllPaths(const std::string initPath)
{
	if (!std::filesystem::is_directory(initPath))
		return std::vector<std::filesystem::path>();

	std::vector<std::filesystem::path> paths;

	for (const auto& entry : std::filesystem::directory_iterator(initPath)) 
	{
		paths.push_back(entry.path());
	} 

	return paths;
}

void NoteViewer::PrintPath(std::filesystem::path path, bool isFile)
{
	std::string pathStr = path.filename().string();
	std::string displayName = pathStr;

	if (isFile)
	{
		displayName = pathStr.substr(3, pathStr.size() - 9);
	}
	else 
	{
		AddCharacter(FOLDER_ICON_START);
		AddCharacter(FOLDER_ICON_END);
	}

	for (int i = 0; i < displayName.size(); i++)
		AddCharacter(displayName[i]);
	Return();
	return;
}

void NoteViewer::PrintPaths()
{
	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (std::filesystem::is_directory(itemPaths[i]))
			PrintPath(itemPaths[i], false);
	}
	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (std::filesystem::is_regular_file(itemPaths[i]))
			PrintPath(itemPaths[i], true);
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
	for (int i = 0; i < program.marginVertices.size() / 8; i++)
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
	// TODO: Implement opening a note
}
