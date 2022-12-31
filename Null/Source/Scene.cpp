#include "Headers/Scene.h"

#include "Headers/GLAbstraction.h"

#include <vector>

/* ====== Scene ====== */

Scene::Scene()
{

}

std::vector<Vertex> Scene::GetVertices()
{
    return std::vector<Vertex>();
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
