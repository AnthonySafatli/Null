#pragma once

#include <string>

#include "TextArea.h"

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

	/* FileViewer Data */
};