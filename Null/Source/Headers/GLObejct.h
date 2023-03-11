#pragma once

#include "GLAbstraction.h"

class GLObject
{
public:
    GLObject();

    void Init();

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexArray vertexArray;
    ShaderProgram shaderProgram;
    Texture texture;
    Uniform u_idealRatio;
    Uniform u_leftMargin;
    Uniform u_size;
    Uniform u_sceneRowIndex;
    Uniform u_sceneColumnIndex;
    Uniform u_tex;
    Uniform u_foreground;
};