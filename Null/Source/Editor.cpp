#include "Headers/Editor.h"

/* ====== Editor ====== */

Editor::Editor() 
	: currentScene(Scene::WelcomeScene()), commandRow(CommandRow()), statusBar(StatusBar("Null Loaded Successfully"))
{
	
}

/* ====== Command Row ====== */

CommandRow::CommandRow()
{

}

/* ====== TextRow ====== */

TextRow::TextRow()
{

}

/* ====== StatusBar ====== */

StatusBar::StatusBar(const char* initialMessage)
{

}

/* ====== Scene ====== */

Scene::Scene()
{

}

Scene Scene::WelcomeScene()
{

}

Scene Scene::TextEditor()
{

}

Scene Scene::NoteBook()
{

}

Scene Scene::SettingsScene()
{

}

Scene Scene::PropertiesPage()
{

}

