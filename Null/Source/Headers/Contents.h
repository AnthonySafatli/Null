#pragma once

#include "GLFW/glfw3.h"

#include "GLAbstraction.h"
#include "MainRows.h"
#include "Scene.h"
#include "CursorController.h"

#include <vector>

#define ENTER  257
#define DEL    259
#define DELETE 261
#define HOME   268
#define END    321
#define SPACE  32

class Contents
{
private:
    Contents();

    static void AddCharacter(char ch);
    static void AddSpace();
    static void AddTab();
    static void RemoveCharacter(bool left);
    static void Return();

public:
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
