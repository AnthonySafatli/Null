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

Scene::Scene(const bool editable, const LineNumType type) : rows(), type(type), editable(editable)
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

void Scene::WelcomeScene(Scene& scene)
{
    AddLine(scene.rows[0], "Welcome to Null - v1.0", 0, 2);
    AddLine(scene.rows[0], "~", 1, 0);
    AddLine(scene.rows[0], "~", 2, 0);
    AddLine(scene.rows[0], "~ Press Enter to continue", 3, 0);
    // TODO: Fix
}

void Scene::NotebookScene(Scene& scene)
{
    // TODO: Load Notebooks
}

void Scene::TextEditor(Scene& scene)
{
}

void Scene::SettingsScene(Scene& scene)
{
    // TODO: Print Settings
}

void Scene::PropertiesScene(Scene& scene)
{
    // TODO: Print Properties
}

void Scene::AddLine(TextRow &row, const std::string text, const int rowNumber, const int columnStart)
{
    for (int i = 0; i < text.size(); i++)
    {
        TexCoords coords = GetCoords(text.at(i));

        int column = columnStart + i;

        row.vertices.push_back(Vertex(0.0, 0.0, coords.u               , coords.v               , rowNumber, column, 0));
        row.vertices.push_back(Vertex(1.0, 0.0, coords.u + (1.0 / 10.0), coords.v               , rowNumber, column, 0));
        row.vertices.push_back(Vertex(1.0, 1.0, coords.u + (1.0 / 10.0), coords.v + (1.0 / 10.0), rowNumber, column, 0));
        row.vertices.push_back(Vertex(0.0, 1.0, coords.u               , coords.v + (1.0 / 10.0), rowNumber, column, 0));
    }
}
