#pragma once

#include "GLAbstraction.h"

class GLObject
{
public:
    GLObject();

    bool init;

    void Init();

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexArray vertexArray;
    ShaderProgram shaderProgram;
    Texture texture;
    Uniform u_idealRatio;
    Uniform u_size;
    Uniform u_leftMargin;
    Uniform u_rowIndex;
    Uniform u_columnIndex;
    Uniform u_maxHeight;
    Uniform u_maxWidth;

    Uniform u_tex;
    Uniform u_foreground;
};