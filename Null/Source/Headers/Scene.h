#pragma once

#include "GLAbstraction.h"
#include "TextRow.h"

#include <vector>
#include <string>

enum LineNumType { NUMBERED, TILDA, NONE };

class Scene
{
private:

	void AddNumber();
	void AddTilda();

public:
	Scene(const bool editable, const LineNumType type);

	void TextInit();
	void Return();

	static Scene WelcomeScene();
	static Scene NotebookScene();
	static Scene TextEditor();
	static Scene SettingsScene();
	static Scene PropertiesScene();

	LineNumType type;
	const bool editable;
	std::vector<TextRow> rows;

	static void AddLine(TextRow row, const std::string text, const int rowNumber);
};

