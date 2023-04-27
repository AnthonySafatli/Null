#include "Headers/FileViewer.h"

#include <filesystem>
#include <string>
#include <vector>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"
#include "Headers/Character.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

FileViewer::FileViewer(const std::string path)
{
	std::vector<std::filesystem::path> paths = GetAllPaths(""); // TODO: Where are notes stored

	for (int i = 0; i < paths.size(); i++)
	{
		// print all notes
	}

	// select first row
	// add notes cursor (arrow in margin)

	// add paths to rows
}

void FileViewer::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
		return;

	switch (key)
	{
	case GLFW_KEY_UP:
		// move cursor up
		break;
	case GLFW_KEY_DOWN:
		// move cursor down
		break;
	case GLFW_KEY_ENTER:
		// open file
		break;
	}
}

void FileViewer::AddLeftMargin()
{
	AddCharacterToMargin('~', -1, MARGIN);
}

void FileViewer::RemoveLeftMargin()
{
	program.marginVertices.resize(program.marginVertices.size() - (4 * (leftMargin - 1)));
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

