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

Scene Scene::WelcomeScene()
{
    Scene welcome = Scene(false, TILDA);

    AddLine(welcome.rows[0], "Welcome to Null - v1.0", 0);

    return welcome;
}

Scene Scene::NotebookScene()
{
    return Scene(false, TILDA);
}

Scene Scene::TextEditor()
{
    return Scene(true, NUMBERED);
}

Scene Scene::SettingsScene()
{
    return Scene(false, TILDA);
}

Scene Scene::PropertiesScene()
{
    return Scene(false, TILDA);
}

// TODO: Pass as reference or pointer
void Scene::AddLine(TextRow row, const std::string text, const int rowNumber)
{
    for (int i = 0; i < text.size(); i++)
    {
        TexCoords coords = GetCoords(text.at(i));

        row.vertices.push_back(Vertex(0.0, 0.0, coords.u               , coords.v               , rowNumber, i, 0));
        row.vertices.push_back(Vertex(1.0, 0.0, coords.u + (1.0 / 10.0), coords.v               , rowNumber, i, 0));
        row.vertices.push_back(Vertex(1.0, 1.0, coords.u + (1.0 / 10.0), coords.v + (1.0 / 10.0), rowNumber, i, 0));
        row.vertices.push_back(Vertex(0.0, 1.0, coords.u               , coords.v + (1.0 / 10.0), rowNumber, i, 0));
    }
}
