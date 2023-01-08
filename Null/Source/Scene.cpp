#include "Headers/Scene.h"

#include "Headers/GLAbstraction.h"
#include "Headers/Contents.h"

#include <vector>
#include <string>

/* ====== Scene ====== */

extern Contents appContents;

Scene::Scene() : rows(), type(NUMBERED)
{
    rows.push_back(TextRow());
}

void Scene::AddNumber()
{
    if (rows.size() == 99)
        appContents.IncrementBarrier();

    rows.push_back(TextRow());
    appContents.cursor.Move(DOWN);
    appContents.cursor.x = 0;

    std::string rowNumberString = std::to_string(rows.size());

    int i = 0;
    while (i < rowNumberString.size())
    {
        appContents.AddCharacter(rowNumberString.at(rowNumberString.size() - i - 1));
        appContents.cursor.x = 0;
        i++;
    }

    while (i < appContents.cursor.sceneLeftBarrier - 1)
    {
        appContents.AddCharacter(' ');
        i++;
    }

    appContents.cursor.x = appContents.cursor.sceneLeftBarrier - 1;
    appContents.AddCharacter(' ');
}

void Scene::AddTilda()
{
    rows.push_back(TextRow());
    appContents.cursor.Move(DOWN);
    appContents.cursor.x = 0;

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
    return Scene();
}

Scene Scene::NotebookScene()
{
    return Scene();
}

Scene Scene::TextEditor()
{
    return Scene();
}

Scene Scene::SettingsScene()
{
    return Scene();
}

Scene Scene::PropertiesScene()
{
    return Scene();
}
