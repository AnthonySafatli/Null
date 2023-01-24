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

	static void WelcomeScene(Scene &scene);
	static void NotebookScene(Scene& scene);
	static void TextEditor(Scene& scene);
	static void SettingsScene(Scene& scene);
	static void PropertiesScene(Scene& scene);

	LineNumType type;
	const bool editable;
	std::vector<TextRow> rows;

	static void AddLine(TextRow &row, const std::string text, const int rowNumber, const int columnStart);
};

