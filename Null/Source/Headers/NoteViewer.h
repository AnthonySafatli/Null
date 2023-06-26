#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "TextArea.h"

struct NoteItem
{
	bool isFile;
	std::filesystem::path path;

	NoteItem(bool isFile, std::filesystem::path path);
};

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
	std::vector<NoteItem> GetAllPaths(const std::string initPath);
	void PrintPath(std::filesystem::path path, bool isFile);
	void PrintPaths();
	void UpdateArrow();
	void OpenItem();

	/* FileViewer Data */
	std::vector<std::string> folderPath;
	std::vector<NoteItem> itemPaths;
	bool isRoot;
	bool locatingError;
};