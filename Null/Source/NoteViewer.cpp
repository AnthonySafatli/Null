#include "Headers/NoteViewer.h"

#include <filesystem>
#include <sstream>
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

// TODO: Make sure to check if user deletes of changes files after validation

NoteViewer::NoteViewer(std::filesystem::path documentPath) : isRoot(true), folderPath()
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	ConstructorStart(3, false);

	itemPaths = GetAllPaths(documentPath.string() + "\\NullNotes");

	for (int i = itemPaths.size() - 1; i >= 0; --i)
	{
		if (!std::filesystem::is_regular_file(itemPaths[i]))
			continue;

		if (!FileNameValidation(itemPaths[i]))
			itemPaths.erase(itemPaths.begin() + i);
	}

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

	ConstructorEnd();

	UpdateArrow();
}

NoteViewer::NoteViewer(std::filesystem::path documentPath, std::vector<std::string> folders) : isRoot(false), folderPath(folders)
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

	std::stringstream folderNameStream = std::stringstream();
	folderNameStream << "\\NullNotes";
	for (std::string folder : folders) folderNameStream << "\\" << folder;
	itemPaths = GetAllPaths(documentPath.string() + folderNameStream.str());

	AddCharacter('.');
	AddCharacter('.');
	Return();

	for (int i = itemPaths.size() - 1; i >= 0; --i)
	{
		if (!std::filesystem::is_regular_file(itemPaths[i]))
			continue;

		if (!FileNameValidation(itemPaths[i]))
			itemPaths.erase(itemPaths.begin() + i);
	}

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
	std::string displayName;

	if (!isFile)
		displayName = pathStr;
	else
	{
		std::string name;
		int month, day, year;
		int hour, min;

		hour = pathStr[0] - 65;
		min = ((pathStr[1] - 65) * 10) + (pathStr[2] - 65);

		month = pathStr[pathStr.size() - 6] - 65;
		
		day = pathStr[pathStr.size() - 5] - 48;
		if (day > 9) day = pathStr[pathStr.size() - 5] - 55;

		year = ((pathStr[pathStr.size() - 4] - 65) * 1000);
		year += ((pathStr[pathStr.size() - 3] - 65) * 100);
		year += ((pathStr[pathStr.size() - 2] - 65) * 10);
		year += ((pathStr[pathStr.size() - 1] - 65));

		name = pathStr.substr(3, pathStr.size() - 9);

		if (!FileNameValidation(path))
			return;

		displayName = name + "    | " + DateToString(month, day, year, hour, min);
		// TODO: Determine length of textarea and base displayName on that
	}

	// TODO: Fix Icons
	/*if (isFile)
		AddCharacter(FILE_ICON);
	else
		AddCharacter(FOLDER_ICON);
	AddCharacter(SPACE);*/

	if (!isFile)
	{
		AddCharacter(FOLDER_ICON_START);
		AddCharacter(FOLDER_ICON_END);
	}


	for (int i = 0; i < displayName.size(); i++)
		AddCharacter(displayName[i]);
	Return();
	return;
}

bool NoteViewer::FileNameValidation(std::filesystem::path path)
{
	std::string pathStr = path.filename().string();

	std::string name;
	int month, day, year;
	int hour, min;

	hour = pathStr[0] - 65;
	if (hour > 23 || hour < 0) return false;

	min = ((pathStr[1] - 65) * 10) + (pathStr[2] - 65);
	if (min < 0 || min > 59) return false;

	month = pathStr[pathStr.size() - 6] - 65;
	if (month < 0 || month > 11) return false;

	day = pathStr[pathStr.size() - 5] - 48;
	if (day > 9) day = pathStr[pathStr.size() - 5] - 55;
	if (day < 0 || day > 30) return false;

	year = ((pathStr[pathStr.size() - 4] - 65) * 1000);
	year += ((pathStr[pathStr.size() - 3] - 65) * 100);
	year += ((pathStr[pathStr.size() - 2] - 65) * 10);
	year += ((pathStr[pathStr.size() - 1] - 65));
	if (year < 1000) return false;

	name = pathStr.substr(3, pathStr.size() - 9);
	if (name.size() < 1) return false;

	return true;
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

	if (!isRoot && (cursorIndex < 0))
	{

#if _DEBUG
		std::cout << "Go back" << std::endl;
#endif

		folderPath.pop_back();
		if (folderPath.size() == 0)
			program.OpenNoteViewer();
		else
			program.OpenNoteViewer(folderPath);

		return;
	}

	int counter = 0;
	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (!std::filesystem::is_directory(itemPaths[i]))
			continue;
		
		if (counter == cursorIndex)
		{

#if _DEBUG
			std::cout << "Open Folder: " << itemPaths[i].string() << std::endl;
#endif
	
			folderPath.push_back(itemPaths[i].filename().string());
			program.OpenNoteViewer(folderPath);
			return;
		}
		counter++;
	}

	for (int i = 0; i < itemPaths.size(); i++)
	{
		if (!std::filesystem::is_regular_file(itemPaths[i]))
			continue;

		if (counter = cursorIndex)
		{

#if _DEBUG
			std::cout << "Open Note: " << itemPaths[i].string() << std::endl;
#endif

			program.OpenNote(itemPaths[i], itemPaths[i].filename().string().substr(3, itemPaths[i].filename().string().size() - 9));
			
			return;
		}
		counter++;
	}
}

