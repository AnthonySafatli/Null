#pragma once

#include "GLAbstraction.h"
#include "MainRows.h"
#include "Scene.h"
#include "CursorController.h"

#include <vector>

class Contents
{
public:
    Contents();

    void AddCharacter(char ch);

    std::vector<Vertex> GetVertices();
    std::vector<unsigned int> GetIndices();

    CommandRow command;
    StatusBar status;
    Scene currentScene;
    CursorController cursor;

    float textSize;
};
