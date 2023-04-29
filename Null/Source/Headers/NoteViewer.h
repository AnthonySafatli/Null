#pragma once

#include <filesystem>
#include <string>

#include "TextArea.h"

class NoteViewer : public TextArea
{
public:
	NoteViewer(std::filesystem::path documentPath);

	/* Callback Methods */
	virtual void ProcessKey(int key, int action, int mods);

	/* Virtual Methods*/
	void AddLeftMargin() override;
	void RemoveLeftMargin() override;

	/* Static Methods */
	static std::filesystem::path GetDocumentsFolder();
 
	/* FileViewer Methods */
	std::vector<std::filesystem::path> GetAllPaths(const std::string initPath);
	void PrintPath(std::filesystem::path path);
	void UpdateArrow();

	/* FileViewer Data */
	std::vector<std::filesystem::path> paths;
};