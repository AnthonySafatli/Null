#pragma once

#include "GLAbstraction.h"
#include "TextRow.h"

#include <vector>
#include <string>

enum LineNumType { NUMBERED, TILDA, NONE };
enum SceneType { TEXT, EDITOR, FILES, SETTINGS };

class Scene
{
private:

	void AddNumber();
	void AddTilda();

public:
	Scene(const SceneType sceneType);

	void TextInit();
	void Return();

	void ToWelcomeScene();
	void ToNotebookScene();
	void ToTextEditor();
	void ToSettingsScene();
	void ToPropertiesScene();

	SceneType sceneType;

	LineNumType lineType;
	bool editable;
	std::vector<TextRow> rows;

	void AddLine(const std::string text);
};

