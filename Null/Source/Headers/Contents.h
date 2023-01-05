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
    Contents();

public:
    static void AddSpace();
    static void AddCharacter(char ch);
    static void AddTab();
    static void RemoveCharacter(bool left);
    static void Return();

    static void ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void ProcessChar(GLFWwindow* window, unsigned int codepoint);
    static void OnResize(GLFWwindow* window, int width, int height);

    static CommandRow command;
    static StatusBar status;
    static Scene currentScene;
    static CursorController cursor;

    static std::vector<Vertex> vertices;
    static std::vector<unsigned int> indices;

    static int tabAmount;
    static float textSize;
};
