#pragma once

#include <vector>

class Editor
{
public:
	Editor();

	CommandRow commandRow;
	StatusBar statusBar;
	Scene currentScene;
};

class CommandRow
{
public:
	CommandRow();

	TextRow commandText;
};

class TextRow
{
public:
	TextRow();

	std::vector<char> text;
};

class StatusBar
{
public:
	StatusBar(const char* initialMessage);
};

class Scene
{
public:
	Scene();

	static Scene WelcomeScene();
	static Scene TextEditor();
	static Scene NoteBook();
	static Scene SettingsScene();
	static Scene PropertiesPage();

	std::vector<TextRow> textRows;
};