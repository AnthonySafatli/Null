#pragma once

#include "TextArea.h"

class TextViewer : public TextArea
{
public:
	TextViewer(const std::string text, const std::string pageName);

	/* Callback Methods */
	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);

	/* Virtual Methods */
	void AddLeftMargin();
	void RemoveLeftMargin();

	/* Viewer Methods */
	void UpdateText(const std::string text);
};