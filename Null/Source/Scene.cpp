#include "Headers/Scene.h"

#include "Headers/GLAbstraction.h"
#include "Headers/NullEditor.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "Headers/Character.h"

/* ====== Scene ====== */

extern NullEditor appContents;

Scene::Scene(const SceneType sceneType) : rows(), editable(false), lineType(TILDA)
{
    rows.push_back(TextRow());

    if (sceneType == EDITOR)
        editable = true;

    /*switch (sceneType)
    {
    case TEXT:
        break;

    case EDITOR:
        editable = true;
        break;

    case FILES:
        break;

    case SETTINGS:
        break;
    }*/
}

void Scene::AddNumber()
{
    if ((unsigned int)std::log10((double)rows.size() + 1.0) + 2 > (unsigned int)appContents.cursor.sceneLeftBarrier)
        appContents.IncrementBarrier();

    rows.push_back(TextRow());
    appContents.cursor.Move(DOWN);
    appContents.cursor.textX = 0;

    std::string rowNumberString = std::to_string(rows.size());

    int i = 0;
    while (i < rowNumberString.size())
    {
        appContents.AddCharacter(rowNumberString.at(rowNumberString.size() - i - 1));
        appContents.cursor.textX = 0;
        i++;
    }

    while (i < appContents.cursor.sceneLeftBarrier - 1)
    {
        appContents.AddCharacter(' ');
        i++;
    }

    appContents.cursor.textX = appContents.cursor.sceneLeftBarrier - 1;
    appContents.AddCharacter(' ');
}

void Scene::AddTilda()
{
    rows.push_back(TextRow());
    appContents.cursor.Move(DOWN);
    appContents.cursor.textX = 0;

    appContents.AddCharacter('~');
    appContents.AddCharacter(' ');
}

void Scene::TextInit()
{
    if (lineType == NUMBERED)
    {
        appContents.AddCharacter(' ');
        appContents.AddCharacter('1');
        appContents.AddCharacter(' ');
    }
    else if (lineType == TILDA)
    {
        appContents.AddCharacter('~');
        appContents.AddCharacter(' ');
    }

    ToWelcomeScene();
}

void Scene::Return()
{
    if (lineType == NUMBERED)
        AddNumber();
    else if (lineType = TILDA)
        AddTilda();
}

void Scene::ToWelcomeScene()
{
    AddLine("Welcome to Null - v1.0");
    appContents.Return();
    appContents.Return();
    AddLine("Press Enter to Continue");
}

void Scene::ToNotebookScene()
{
    // TODO: Load Notebooks
}

void Scene::ToTextEditor()
{
}

void Scene::ToSettingsScene()
{
    // TODO: Print Settings
}

void Scene::ToPropertiesScene()
{
    // TODO: Print Properties
}

void Scene::AddLine(const std::string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        appContents.AddCharacter(text.at(i));
    }
}
