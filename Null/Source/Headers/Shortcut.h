#pragma once

class Shortcut {
private: 
	Shortcut();

	static bool ctrlPressed();
	static bool shftPressed();

	static void Save();
	static void Open();

public:
	static void UniversalShortcuts(const int key, const int action);
	static void CommandShortcuts(const int key, const int action);
	static void TextEditorShortcuts(const int key, const int action);
};