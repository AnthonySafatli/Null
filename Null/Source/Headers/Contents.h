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
#define KEYCODE_HOME   327
#define KEYCODE_END    321

#define KEYCODE_SPACE  32

class Contents
{
private:

    /* Private */
    void SaveChar(char ch);

public:
    /* Constructor */
    Contents(const int width, const int height, const int tabAmount, const float textSize);

    /* Text Methods */
    void IncrementBarrier();
    void AddCharacter(char ch);
    void RemoveCharacterFromLeft();
    void RemoveCharacterFromRight();
    void AddTab();
    void Return();

    /* CallBack Methods */
    void ProcessKey(int key, int action, int mods);
    void ProcessChar(unsigned int codepoint);
    void OnResize(int width, int height);
    
    /* OpenGL Methods */
    void GLInit();
    void SetData();
    void SetRowIndex(const int index);
    void SetColumnIndex(const int index);


    /* OpenGL Variables */
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexArray vertexArray;
    ShaderProgram shaderProgram;
    Texture texture;
    Uniform u_idealRatio;
    Uniform u_size;
    Uniform u_sceneRowIndex;
    Uniform u_sceneColumnIndex;
    Uniform u_tex;

    /* Text Variables */
    CommandRow command;
    Scene currentScene;
    // StatusBar status;
    CursorController cursor;

    /* Vectors */
    std::vector<unsigned int> indices;

    /* Text Properties */
    int tabAmount;
    float textSize;
    int rowIndex;
    int columnIndex;

    /* Window Properties */
    int width;
    int height;

    /* Constants */
    const int idealWidth;
    const int idealHeight;
};
