#include "Headers/FileViewer.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"
#include "Headers/Character.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

FileViewer::FileViewer(const std::string path)
{
	paths = GetAllPaths(path);

	SetLeftMargin(1);
	rows.push_back(std::string());

	program.textX = 0;
	program.textY = 0;

	AddCharacter('N');
	AddCharacter('o');
	AddCharacter('t');
	AddCharacter('e');
	AddCharacter('s');
	AddCharacter(':');
	Return();

	PrintDir();

	program.HideCursor();
	program.showCursor = false;

	program.RenderStatus("Notes loaded Successfully");
}

std::vector<std::filesystem::path> FileViewer::GetAllPaths(const std::string initPath)
{
	if (!std::filesystem::is_directory(initPath))
		return std::vector<std::filesystem::path> ();

	std::vector<std::filesystem::path> paths;

	for (const auto& entry : std::filesystem::directory_iterator(initPath)) 
	{
		paths.push_back(entry.path());
	}

	for (int i = paths.size() - 1; i >= 0; i--)
	{
		std::vector<std::filesystem::path> dirPaths = GetAllPaths(paths[i].generic_string());
		paths.insert(paths.begin() + i, dirPaths.begin(), dirPaths.end());
	}

	return paths;
}

void FileViewer::PrintDir()
{
	// TODO: Update
	int partsBase = 8;

	for (int i = paths.size() - 1; i >= 0; i--)
	{
		std::string fullPath = paths[i].string();
		std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

		int parts = Split(fullPath, '/').size();

		for (int i = partsBase; i < parts; i++) AddTab();
		AddCharacter(127); AddCharacter(' ');

		std::string pathStr = paths[i].filename().string();

		for (char c : pathStr)
		{
			AddCharacter(c);
		}

		Return();
	}

	RemoveCharacterFromLeft();
}

