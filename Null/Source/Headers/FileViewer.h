#pragma once

#include <filesystem>
#include <string>

#include "TextArea.h"

class FileViewer : public TextArea
{
public:
	FileViewer(const std::string path);

	/* FileViewer Methods */
	std::vector<std::filesystem::path> GetAllPaths(const std::string initPath);
	void PrintDir();

	/* FileViewer Data */
	std::vector<std::filesystem::path> paths;
};