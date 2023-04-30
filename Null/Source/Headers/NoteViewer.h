#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "TextArea.h"

class NoteViewer : public TextArea
{
public:
	NoteViewer(std::filesystem::path documentPath);
	NoteViewer(std::filesystem::path documentPath, std::vector<std::string> folders);

	/* Callback Methods */
	virtual void ProcessKey(int key, int action, int mods);

	/* Virtual Methods*/
	void AddLeftMargin() override;
	void RemoveLeftMargin() override;

	/* Static Methods */
	static std::filesystem::path GetDocumentsFolder();
 
	/* FileViewer Methods */
	std::vector<std::filesystem::path> GetAllPaths(const std::string initPath);
	void PrintPath(std::filesystem::path path, bool isFile);
	void UpdateArrow();
	std::string DateToString(int month, int day, int year, int hour, int min);
	void OpenItem();

	/* FileViewer Data */
	std::vector<std::string> folderPath;
	std::vector<std::filesystem::path> itemPaths;
	bool isRoot;
};