#pragma once

#include <string>

#include "TextArea.h"

class TextEditor : public TextArea
{
public:
	TextEditor();
	TextEditor(const std::string text, const std::string directory, const std::string fileName, bool isNote);

	/* Callback Methods */
	void ProcessKey(int key, int action, int mods) override;
	void ProcessChar(unsigned int codepoint) override;

	/* Virtual Methods */
	void AddLeftMargin() override;
	void RemoveLeftMargin() override;

	/* Editor Methods */
	void IncreaseLeftMargin();
	/// <summary>
	/// If you move up or down to a shorter row, the cursor will move to the end of the shorter row rather than hang out in empty space 
	/// </summary>
	void UpdateCol();
	/// <summary>
	/// Make sure the cursor is visible by updating row / column Index
	/// </summary>
	void UpdateRowColVisual();
	std::string GetText();
	void SetPath(std::string path);

	/* Key Callback Methods */
	void MoveLeftCtrl();
	void MoveRightCtrl();

	/* Editor Data */
	std::string fileDirectory;
	std::string fileName;
	bool isNote;

private:
	inline bool isWhiteSpace(char c);
	inline bool isText(char c);

};