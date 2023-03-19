#include "Headers/FileViewer.h"

#include <iostream>
#include <filesystem>
#include <string>

FileViewer::FileViewer(const std::string path)
{
	paths = GetAllPaths("");
}

void FileViewer::ProcessKey(int key, int action, int mods)
{
	// TODO
}

void FileViewer::ProcessChar(unsigned int codepoint)
{
	// TODO
}

void FileViewer::OnResize(int width, int height)
{
	// TODO
}

void FileViewer::AddLeftMargin()
{
	// TODO
}

void FileViewer::RemoveLeftMargin()
{
	// TODO
}

// TODO: Test
std::vector<std::filesystem::path> FileViewer::GetAllPaths(const std::string initPath)
{
	std::vector<std::filesystem::path> paths;

	for (const auto& entry : std::filesystem::directory_iterator(initPath)) 
	{
		paths.push_back(entry.path());
	}

	for (int i = paths.size() - 1; i <= 0; i--)
	{
		std::vector<std::filesystem::path> dirPaths = GetAllPaths(paths[i].generic_string());
		paths.insert(paths.begin() + i, dirPaths.begin(), dirPaths.end());
	}

	return paths;
}

