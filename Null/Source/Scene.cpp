#include "Headers/Scene.h"

#include "Headers/GLAbstraction.h"
#include "Headers/NullEditor.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

/* ====== Scene ====== */

extern NullEditor appContents;

Scene::Scene(const bool editable) : rows(), type(NUMBERED), editable(editable)
{
    rows.push_back(TextRow());
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
    if (type == NUMBERED)
    {
        appContents.AddCharacter(' ');
        appContents.AddCharacter('1');
        appContents.AddCharacter(' ');
    }
    else if (type == TILDA)
    {
        appContents.AddCharacter('~');
        appContents.AddCharacter(' ');
    }
}

void Scene::Return()
{
    if (type == NUMBERED)
        AddNumber();
    else if (type = TILDA)
        AddTilda();
}

Scene Scene::WelcomeScene()
{
    return Scene(false);
}

Scene Scene::NotebookScene()
{
    return Scene(false);
}

Scene Scene::TextEditor()
{
    return Scene(true);
}

Scene Scene::SettingsScene()
{
    return Scene(false);
}

Scene Scene::PropertiesScene()
{
    return Scene(false);
}
