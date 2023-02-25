#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/NullEditor.h"
#include "Headers/CallBack.h"
#include "Headers/Uniforms.h"
#include "Headers/Program.h"

// TODO: Maybe don't use global variables
Program program = Program(1800, 1100, 0.24, 4);

int main(void)
{
    /* Initialize GLFW */
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // TODO: Fix Transparent Window
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    int maxSquaresToRender = 1000;

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

    /* OpenGL Setup */
    if (glewInit())
        return -3;

    glClearColor(0.03, 0.05, 0.09, 0.85);

    program.GLInit();

    glUniform2f(program.openGL.u_idealRatio.location, (float) program.idealWidth / (float) program.width, (float) program.idealHeight / (float) program.height);
    glUniform1i(program.openGL.u_leftMargin.location, program.area->leftMargin);
    glUniform1f(program.openGL.u_size.location, 0.1 * program.textSize);
    glUniform1i(program.openGL.u_sceneRowIndex.location, 0);
    glUniform1i(program.openGL.u_sceneColumnIndex.location, 0);
    glUniform1i(program.openGL.u_tex.location, 0);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, program.indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwWaitEvents();

        // GetErrors();
    }

    glfwTerminate();

    return 0;
}

void UpdateUniform2f(const unsigned int location, const float v1, const float v2)
{
    glUniform2f(location, v1, v2);
}

void UpdateUniform1i(const unsigned int location, const int i1)
{
    glUniform1i(location, i1);
}

void NullEditor::SetRowIndex(const int index)
{
    glUniform1i(program.openGL.u_sceneRowIndex.location, index);
    SetData();
}

void NullEditor::SetColumnIndex(const int index)
{
    glUniform1i(program.openGL.u_sceneColumnIndex.location, index);
    SetData();
}