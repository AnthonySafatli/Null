#include "Headers/NoteViewer.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <shlobj.h>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

NoteViewer::NoteViewer(std::filesystem::path documentPath) : isRoot(true)
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	SetLeftMargin(3);
	rows.push_back(std::string());
	AddLeftMargin();

	program.textX = 0;
	program.textY = 0;
	
	program.HideCursor();
	program.showCursor = false;

	paths = GetAllPaths(documentPath.string() + "\\NullNotes");

#if _DEBUG
	std::cout << "Note Files:" << std::endl;
#endif

	if (paths.size() == 0)
	{
		const std::string message = "No Notes or Folders...";
		for (int i = 0; i < message.size(); i++) AddCharacter(message[i]);
		program.textY = 1;
		return;
	}

	for (int i = 0; i < paths.size(); i++)
	{
		if (std::filesystem::is_directory(paths[i]))
		{

#if _DEBUG
		std::cout << paths[i].filename().string() << std::endl;
#endif

			PrintPath(paths[i], false);
		}
	}
	for (int i = 0; i < paths.size(); i++)
	{

		if (std::filesystem::is_regular_file(paths[i]))
		{

#if _DEBUG
			std::cout << paths[i].filename().string() << std::endl;
#endif

			PrintPath(paths[i], true);
		}
	}
	RemoveCharacterFromLeft();

	program.textX = 0;
	program.textY = 0;

	UpdateUniform1i(program.openGL.u_cursorRow.location, program.textY + 1);
	UpdateArrow();
}

NoteViewer::NoteViewer(std::filesystem::path documentPath, std::vector<std::string> folders) : isRoot(false)
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	SetLeftMargin(3);
	rows.push_back(std::string());
	AddLeftMargin();

	program.textX = 0;
	program.textY = 0;

	program.HideCursor();
	program.showCursor = false;

	std::stringstream foldersStream("\\NullNotes");
	for (std::string folder : folders) foldersStream << "\\" << folder;
	paths = GetAllPaths(documentPath.string() + foldersStream.str());

#if _DEBUG
	std::cout << "Note Files:" << std::endl;
#endif

	AddCharacter('.');
	AddCharacter('.');
	Return();

	if (paths.size() == 0)
	{
		const std::string message = "No Notes or Folders...";
		for (int i = 0; i < message.size(); i++) AddCharacter(message[i]);
		return;
	}

	for (int i = 0; i < paths.size(); i++)
	{
		if (std::filesystem::is_directory(paths[i]))
		{

#if _DEBUG
			std::cout << paths[i].filename().string() << std::endl;
#endif

			PrintPath(paths[i], false);
		}
	}
	for (int i = 0; i < paths.size(); i++)
	{

		if (std::filesystem::is_regular_file(paths[i]))
		{

#if _DEBUG
			std::cout << paths[i].filename().string() << std::endl;
#endif

			PrintPath(paths[i], true);
		}
	}
	RemoveCharacterFromLeft();

	program.textX = 0;
	program.textY = 0;


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
		OpenItem();
		break;
	}

	UpdateUniform1i(program.openGL.u_cursorRow.location, program.textY + 1);
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

std::vector<std::filesystem::path> NoteViewer::GetAllPaths(const std::string initPath)
{
	if (!std::filesystem::is_directory(initPath))
		return std::vector<std::filesystem::path>();

	std::vector<std::filesystem::path> paths;

	for (const auto& entry : std::filesystem::directory_iterator(initPath)) 
	{
		paths.push_back(entry.path());
	}

	/* Recursive:
	for (int i = paths.size() - 1; i >= 0; i--)
	{
		std::vector<std::filesystem::path> dirPaths = GetAllPaths(paths[i].generic_string());
		paths.insert(paths.begin() + i, dirPaths.begin(), dirPaths.end());
	}
	*/ 

	return paths;
}

void NoteViewer::PrintPath(std::filesystem::path path, bool isFile)
{
	std::string pathStr = path.filename().string();
	std::string displayName;

	if (!isFile)
		displayName = pathStr;
	else
	{
		std::string name;
		int month, day, year;
		int hour, min;

		hour = pathStr[0] - 65;
		if (hour > 23 || hour < 0) return;

		min = ((pathStr[1] - 65) * 10) + (pathStr[2] - 65);
		if (min < 0 || min > 59) return;

		month = pathStr[pathStr.size() - 6] - 65;
		if (month < 0 || month > 11) return;

		day = pathStr[pathStr.size() - 5] - 48;
		if (day > 9) day = pathStr[pathStr.size() - 5] - 55;
		if (day < 0 || day > 30) return;

		year = ((pathStr[pathStr.size() - 4] - 65) * 1000);
		year += ((pathStr[pathStr.size() - 3] - 65) * 100);
		year += ((pathStr[pathStr.size() - 2] - 65) * 10);
		year += ((pathStr[pathStr.size() - 1] - 65));
		if (year < 0) return;

		name = pathStr.substr(3, pathStr.size() - 9);

		displayName = name + "    | " + DateToString(month, day, year, hour, min);
		// TODO: Determine length of textarea and base displayName on that
	}

	// TODO: Fix Icons
	/*if (isFile)
		AddCharacter(FILE_ICON);
	else
		AddCharacter(FOLDER_ICON);
	AddCharacter(SPACE);*/

	for (int i = 0; i < displayName.size(); i++)
		AddCharacter(displayName[i]);
	Return();
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
}

std::string NoteViewer::DateToString(int month, int day, int year, int hour, int min)
{
	std::stringstream dateStream;

	// Day
	dateStream << std::to_string(day + 1);

	// Month
	switch (month) {
	case 0:
		dateStream << " Jan ";
		break;
	case 1:
		dateStream << " Feb ";
		break;
	case 2:
		dateStream << " Mar ";
		break;
	case 3:
		dateStream << " Apr ";
		break;
	case 4:
		dateStream << " May ";
		break;
	case 5:
		dateStream << " Jun ";
		break;
	case 6:
		dateStream << " Jul ";
		break;
	case 7:
		dateStream << " Aug ";
		break;
	case 8:
		dateStream << " Sep ";
		break;
	case 9:
		dateStream << " Oct ";
		break;
	case 10:
		dateStream << " Nov ";
		break;
	case 11:
		dateStream << " Dec ";
		break;
	}

	// Year
	dateStream << std::to_string(year);
	dateStream << " ";

	// Hours
	if (hour == 0)
		dateStream << "12";
	else if (hour > 12)
		dateStream << std::to_string(hour - 12);
	else
		dateStream << std::to_string(hour);
	dateStream << ":";

	// Min
	if (min < 10) dateStream << "0";
	dateStream << std::to_string(min);

	dateStream << ((hour < 13) ? "AM" : "PM");

	return dateStream.str();
}

void NoteViewer::OpenItem()
{
	int cursorIndex = program.textY;
	if (!isRoot)
		cursorIndex--;

	if (isRoot && cursorIndex == -1)
	{
		// TODO: Send us back (..)
		return;
	}

	int counter = 0;
	for (int i = 0; i < paths.size(); i++)
	{
		if (!std::filesystem::is_directory(paths[i]))
			continue;
		
		if (counter == cursorIndex)
		{

#if _DEBUG
		std::cout << "Open Folder: " << paths[i].string() << std::endl;
#endif
	
			// TODO: open path[i] (folder)
			return;
		}
		counter++;
	}

	for (int i = 0; i < paths.size(); i++)
	{
		if (!std::filesystem::is_regular_file(paths[i]))
			continue;

		if (counter = cursorIndex)
		{

#if _DEBUG
		std::cout << "Open Note: " << paths[i].string() << std::endl;
#endif

			// open path[i] (note)
			return;
		}
		counter++;
	}
}

