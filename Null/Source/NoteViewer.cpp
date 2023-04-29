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

NoteViewer::NoteViewer(std::filesystem::path documentPath)
{
	if (!std::filesystem::is_directory(documentPath / "NullNotes"))
		std::filesystem::create_directory(documentPath / "NullNotes");

	SetLeftMargin(3);
	rows.push_back(std::string());
	AddLeftMargin();

	program.textX = 0;
	program.textY = 0;

	std::vector<std::filesystem::path> paths = GetAllPaths(documentPath.string() + "\\NullNotes");

#if _DEBUG
	std::cout << "Note Files:" << std::endl;
#endif

	if (paths.size() == 0)
	{
		// Display message for empty
	}

	for (int i = 0; i < paths.size(); i++)
	{

#if _DEBUG
		std::cout << paths[i].filename().string() << std::endl;
#endif

		PrintPath(paths[i]);
		Return();
	}
	RemoveCharacterFromLeft();

	program.textX = 0;
	program.textY = 0;

	program.HideCursor();
	program.showCursor = false;


	// select first row
	// add notes cursor (arrow in margin)

	UpdateArrow();
}

void NoteViewer::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
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
		// open file
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

void NoteViewer::PrintPath(std::filesystem::path path)
{
	std::string pathStr = path.filename().string();

	// AddCharacter(file or folder icon);

	for (int i = 0; i < pathStr.size(); i++)
		AddCharacter(pathStr[i]);
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

