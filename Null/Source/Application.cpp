#include <iostream>

#include <NFD/nfd.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>

#include "Headers/GLAbstraction.h"
#include "Headers/CallBack.h"
#include "Headers/Uniforms.h"
#include "Headers/Program.h"
#include "Headers/Platform.h"

void Draw();

Program program = Program(1800, 1100, 24, 4);

int main(void)
{
    Platform::HideConsole();

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

    window = glfwCreateWindow(program.width, program.height, "Welcome - Null", NULL, NULL);
    if (!window)
    {
        NFD_Quit();
        glfwTerminate();
        return -2;
    }
    program.window = window;

    glfwMakeContextCurrent(window);

    int width, height, numColourChannels;
    unsigned char* iconBytes = stbi_load("res\\icon.png", &width, &height, &numColourChannels, 4);
 
    GLFWimage icon;
    icon.width = width;
    icon.height = height;
    icon.pixels = iconBytes;

    glfwSetWindowIcon(window, 1, &icon);

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
    program.LoadSettingsFile();

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

// END: Add max amount of vertices so computer don't explode
void Draw()
{
    std::vector<Vertex> vertices = program.GetVertices();

    program.openGL.vertexBuffer.SetData(&vertices[0], vertices.size());
    program.openGL.indexBuffer.SetData(&program.indices[0], program.indices.size());

    glDrawElements(GL_TRIANGLES, program.indices.size(), GL_UNSIGNED_INT, nullptr);
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