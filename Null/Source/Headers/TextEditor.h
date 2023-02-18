#pragma once

#include "TextArea.h"

class TextEditor : public TextArea
{
public:
	TextEditor();

	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);
};