#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"
#include "Headers/NullEditor.h"
#include "Headers/CallBack.h"
#include "Headers/Uniforms.h"
#include "Headers/Program.h"

#define DEBUG 1

// TODO: Maybe don't use global variables
Program program = Program(1800, 1100, 24, 4);

int main(void)
{
    /* Initialize GLFW */
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // TODO: Fix Transparent Window
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(program.width, program.height, "Null", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    /* Setup Callbacks */
    glfwSetKeyCallback(window, CallBack::KeyCallBack);
    glfwSetCharCallback(window, CallBack::CharCallBack);
    glfwSetFramebufferSizeCallback(window, CallBack::FrameBufferResizeCallBack);
    glfwSetScrollCallback(window, CallBack::ScrollCallBack);

    /* OpenGL Setup */
    if (glewInit())
        return -3;

    glClearColor(0.03, 0.05, 0.09, 0.85);

    program.GLInit();

    glUniform2f(program.openGL.u_idealRatio.location, (float) program.idealWidth / (float) program.width, (float) program.idealHeight / (float) program.height);
    glUniform1i(program.openGL.u_leftMargin.location, program.area->leftMargin);
    glUniform1f(program.openGL.u_size.location, 0.001 * program.textSize);
    glUniform1i(program.openGL.u_sceneRowIndex.location, 0);
    glUniform1i(program.openGL.u_sceneColumnIndex.location, 0);
    glUniform1i(program.openGL.u_tex.location, 0);
    glUniform4f(program.openGL.u_foreground.location, 1.0, 1.0, 1.0, 1.0);

    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        program.Update(deltaTime);
        
        std::vector<Vertex> allVertices = program.GetVertices();

        int j = 0;
        int ii = 0;
        for (int i = 0; i + (MAX_SQAURES_TO_RENDER * 4) < allVertices.size(); i += MAX_SQAURES_TO_RENDER * 4)
        {
            std::vector<Vertex> batchVertices = std::vector<Vertex>(allVertices.begin() + i, allVertices.begin() + i + (MAX_SQAURES_TO_RENDER * 4));
            std::vector<unsigned int> batchIndices = std::vector<unsigned int>(program.indices.begin() + j, program.indices.begin() + j + (MAX_SQAURES_TO_RENDER * 6));

            program.openGL.vertexBuffer.SetData(batchVertices);
            program.openGL.indexBuffer.SetData(batchIndices);

            glDrawElements(GL_TRIANGLES, batchIndices.size(), GL_UNSIGNED_INT, nullptr);

            ii += MAX_SQAURES_TO_RENDER * 4;
            j += MAX_SQAURES_TO_RENDER * 6;
        }
        // TODO: Figure out why not working :(((
        std::vector<Vertex> batchVertices = std::vector<Vertex>(allVertices.begin() + ii, allVertices.end());
        std::vector<unsigned int> batchIndices = std::vector<unsigned int>(program.indices.begin() + j, program.indices.end());

        program.openGL.vertexBuffer.SetData(batchVertices);
        program.openGL.indexBuffer.SetData(batchIndices);

        glDrawElements(GL_TRIANGLES, batchIndices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();

#if DEBUG
        GetErrors();
#endif

        if (program.shouldClose)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glfwTerminate();

    return 0;
}

void UpdateUniform2f(const unsigned int location, const float v1, const float v2)
{
    glUniform2f(location, v1, v2);
}

void UpdateUniform4f(const unsigned int location, const float v1, const float v2, const float v3, const float v4)
{
    glUniform4f(location, v1, v2, v3, v4);
}

void UpdateUniform1f(const unsigned int location, const float v1)
{
    glUniform1f(location, v1);
}

void UpdateUniform1i(const unsigned int location, const int v1)
{
    glUniform1i(location, v1);
}

void UpdateBackground(const float r, const float g, const float b, const float a) 
{
    glClearColor(r, g, b, a);
}