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

	/* FileViewer Data */
	std::vector<std::filesystem::path> paths;
};