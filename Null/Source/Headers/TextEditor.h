#pragma once

#include <string>

#include "TextArea.h"

class TextEditor : public TextArea
{
public:
	TextEditor();
	TextEditor(const std::string text, const std::string directory, const std::string fileName);

	/* Callback Methods */
	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);

	/* Virtual Methods */
	void AddLeftMargin();
	void RemoveLeftMargin();

	/* Editor Methods */
	void IncreaseLeftMargin();
	void UpdateCol();
	void UpdateRowColVisual();
	void UpdateColVisual();
	std::string GetText();
	void SetPath(std::string path);

	/* Editor Data */
	std::string fileDirectory;
	std::string fileName;
};