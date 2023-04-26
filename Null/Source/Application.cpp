#include <iostream>

#include <NFD/nfd.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"
#include "Headers/CallBack.h"
#include "Headers/Uniforms.h"
#include "Headers/Program.h"

void Draw();

// Random objectives
// TODO: Redo font texture
//          - Fix alignment issues
// TODO: Redo all status bar messages
//          - Change use of it
//              - Ideas: Show cursor positions, actual info such as file opening and saving (temporary), etc
// TODO: Revamp input system
//          - Clean up TextEditor input methods

// TODO: Maybe don't use global variables
// TODO: Set after init for glfw and glew
Program program = Program(1800, 1100, 24, 4);

int main(void)
{
    /* Initialize NFD */
    NFD_Init();
    
    /* Initialize GLFW */
    GLFWwindow* window;

    if (!glfwInit())
    {
        NFD_Quit();
        return -1;
    }

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(program.width, program.height, "Null", NULL, NULL);
    if (!window)
    {
        NFD_Quit();
        glfwTerminate();
        return -2;
    }
    program.window = window;

    glfwMakeContextCurrent(window);

    /* Setup Callbacks */
    glfwSetKeyCallback(window, CallBack::KeyCallBack);
    glfwSetCharCallback(window, CallBack::CharCallBack);
    glfwSetFramebufferSizeCallback(window, CallBack::FrameBufferResizeCallBack);
    glfwSetScrollCallback(window, CallBack::ScrollCallBack);

    /* OpenGL Setup */
    if (glewInit())
    {
        NFD_Quit();
        glfwTerminate();
        return -3;
    }

    glClearColor(0.03, 0.05, 0.09, 0.85);

    program.GLInit();

    glUniform2f(program.openGL.u_idealRatio.location, (float) program.idealWidth / (float) program.width, (float) program.idealHeight / (float) program.height);
    glUniform1f(program.openGL.u_size.location, 0.001 * program.textSize);
    glUniform1i(program.openGL.u_leftMargin.location, program.area->leftMargin);
    glUniform1i(program.openGL.u_rowIndex.location, 0);
    glUniform1i(program.openGL.u_columnIndex.location, 0);
    glUniform1i(program.openGL.u_maxHeight.location, ((1.0 / (program.textSize * 0.001)) * ((float)program.height / (float)IDEAL_HEIGHT)) - 4);
    glUniform1i(program.openGL.u_maxWidth.location,  ((1.0 / (program.textSize * 0.001)) * ((float)program.width / (float)IDEAL_WIDTH)) - program.area->leftMargin);

    glUniform1i(program.openGL.u_tex.location, 0);
    glUniform4f(program.openGL.u_foreground.location, 1.0, 1.0, 1.0, 1.0);
    glUniform1i(program.openGL.u_cursorRow.location, 1);

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
        
        Draw();

        glfwSwapBuffers(window);

        glfwPollEvents();

        #if _DEBUG
            GetErrors();
        #endif

        if (program.shouldClose)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    /* Terminate */
    NFD_Quit();
    glfwTerminate();

    return 0;
}

void Draw()
{
    std::vector<Vertex> allVertices = program.GetVertices();

    int numVerticesLeft = allVertices.size();
    int startIndexV = 0;
    int startIndexI = 0;


    while (numVerticesLeft > 0) {
        int numVerticesToRender = std::min(numVerticesLeft, MAX_SQAURES_TO_RENDER * 4);
        int numIndicesToRender = numVerticesToRender / 4 * 6;

        program.openGL.vertexBuffer.SetData(&allVertices[0] + startIndexV, numVerticesToRender);
        program.openGL.indexBuffer.SetData(&program.indices[0] + startIndexI, numIndicesToRender);

        glDrawElements(GL_TRIANGLES, numIndicesToRender, GL_UNSIGNED_INT, nullptr);

        numVerticesLeft -= numVerticesToRender;
        startIndexV += numVerticesToRender;
        startIndexI += numIndicesToRender;
    }
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

void UpdateMaxHeightWidth()
{
    glUniform1i(program.openGL.u_maxHeight.location, ((1.0 / (program.textSize * 0.001)) * ((float)program.height / (float)IDEAL_HEIGHT)) - 4);
    glUniform1i(program.openGL.u_maxWidth.location, ((1.0 / (program.textSize * 0.001)) * ((float)program.width / (float)IDEAL_WIDTH)) - program.area->leftMargin);
}