#pragma once

#include "MainRows.h"
#include "Scene.h"

#include <vector>

class Contents
{
public:
    Contents();

    CommandRow command;
    StatusBar status;
    Scene currentScene;
};
