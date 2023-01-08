#pragma once

#include "GLAbstraction.h"
#include "TextRow.h"

#include <vector>

class Scene
{
public:
	Scene();

	static Scene WelcomeScene();
	static Scene NotebookScene();
	static Scene TextEditor();
	static Scene SettingsScene();
	static Scene PropertiesScene();

	std::vector<TextRow> rows;
};

