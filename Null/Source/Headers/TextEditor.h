#pragma once

#include <string>

#include "TextArea.h"

#define KEYCODE_RIGHT  262    
#define KEYCODE_LEFT   263
#define KEYCODE_UP     265
#define KEYCODE_DOWN   264
#define KEYCODE_HOME   327
#define KEYCODE_END    321

#define KEYCODE_ENTER  257
#define KEYCODE_TAB    258
#define KEYCODE_DEL    259
#define KEYCODE_DELETE 261

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
	void UpdateRowChange();
	std::string GetText();
	void SetPath(std::string path);

	/* Editor Data */
	std::string fileDirectory;
	std::string fileName;
};