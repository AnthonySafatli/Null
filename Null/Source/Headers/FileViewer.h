#pragma once

#include <filesystem>
#include <string>

#include "TextArea.h"

#define KEYCODE_UP   265
#define KEYCODE_DOWN 264

class FileViewer : public TextArea
{
public:
	FileViewer(const std::string path);

	/* Callback Methods */
	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);

	/* Virtual Methods */
	void AddLeftMargin();
	void RemoveLeftMargin();

	/* FileViewer Methods */
	std::vector<std::filesystem::path> GetAllPaths(const std::string initPath);
	void PrintDir();

	/* FileViewer Data */
	std::vector<std::filesystem::path> paths;
};