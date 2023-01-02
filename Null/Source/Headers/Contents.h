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
    void RemoveCharacter();

    CommandRow command;
    StatusBar status;
    Scene currentScene;
    CursorController cursor;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float textSize;
};
