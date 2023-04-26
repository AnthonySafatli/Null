#pragma once

class Shortcut {
private: 
	Shortcut();

	static void SaveAs();
	static void Save();
	static void Open();
	static void OpenNew();
	static void Copy();
	static void Paste();
	static void Cut();
	static void Undo();
	static void Redo();
	static void Duplicate();
	static void Refresh();
	static void ScrollUp();
	static void ScrollDown();
	static void SizeUp();
	static void SizeDown();
	static void Help();
	static void Settings();

public:
	static void UniversalShortcuts(int key, int action , int mods);
	static void CommandShortcuts(int key, const int action, int mods);
	static void TextEditorShortcuts(int key, int action, int mods);
};