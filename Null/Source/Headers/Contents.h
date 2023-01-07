#pragma once

#include "GLFW/glfw3.h"

#include "GLAbstraction.h"
#include "MainRows.h"
#include "Scene.h"
#include "CursorController.h"

#include <vector>

#define KEYCODE_ENTER  257
#define KEYCODE_TAB    258
#define KEYCODE_DEL    259
#define KEYCODE_DELETE 261
#define KEYCODE_RIGHT  262    
#define KEYCODE_LEFT   263
#define KEYCODE_UP     265
#define KEYCODE_DOWN   264
#define KEYCODE_HOME   268
#define KEYCODE_END    321

#define KEYCODE_SPACE  32

class Contents
{
private:

    void SaveChar(char ch);

public:
    Contents(const int width, const int height, const int tabAmount, const float textSize, const int startX, const int startY);

    void AddCharacter(char ch);
    void RemoveCharacterFromLeft();
    void RemoveCharacterFromRight();
    void AddTab();
    void Return();

    void ProcessKey(int key, int action, int mods);
    void ProcessChar(unsigned int codepoint);
    void OnResize(int width, int height);

    // CommandRow command;
    // StatusBar status;
    Scene currentScene;
    CursorController cursor;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    int tabAmount;
    float textSize;

    int width;
    int height;
    const int idealWidth;
    const int idealHeight;
};
