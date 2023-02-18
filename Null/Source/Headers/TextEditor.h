#pragma once

#include "TextArea.h"

#define KEYCODE_RIGHT  262    
#define KEYCODE_LEFT   263
#define KEYCODE_UP     265
#define KEYCODE_DOWN   264
#define KEYCODE_HOME   327
#define KEYCODE_END    321

class TextEditor : public TextArea
{
public:
	TextEditor();

	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);
};