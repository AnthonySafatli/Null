#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/Contents.h"
#include "Headers/CallBack.h"
#include "Headers/Uniforms.h"

Contents appContents = Contents(1800, 1100, 4, 0.3);

int main(void)
{
    /* Initialize GLFW */
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    int maxSquaresToRender = 1000;

    window = glfwCreateWindow(appContents.width, appContents.height, "Null", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    /* Setup Callbacks */
    glfwSetKeyCallback(window, CallBack::KeyCallBack);
    glfwSetCharCallback(window, CallBack::CharCallBack);
    glfwSetFramebufferSizeCallback(window, CallBack::FrameBuffeResizeCallBack);

    /* OpenGL Setup */
    if (glewInit())
        return -3;

    glClearColor(0.03, 0.05, 0.09, 0.85);
    // glClearColor(0.03, 0.05, 0.09, 0.05);

    appContents.GLInit();

    glUniform2f(appContents.u_idealRatio.location, (float) appContents.idealWidth / (float) appContents.width, (float) appContents.idealHeight / (float) appContents.height);
    glUniform1f(appContents.u_size.location, 0.1 * appContents.textSize);
    glUniform1i(appContents.u_sceneRowIndex.location, 0);
    glUniform1i(appContents.u_sceneColumnIndex.location, 0);
    glUniform1i(appContents.u_tex.location, 0);

    appContents.currentScene.TextInit();

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, appContents.indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    glfwTerminate();

    return 0;
}

void UpdateUniform2f(const unsigned int location, const float v1, const float v2)
{
    glUniform2f(location, v1, v2);
}

void Contents::SetRowIndex(const int index)
{
    glUniform1i(appContents.u_sceneRowIndex.location, index);
}

void Contents::SetColumnIndex(const int index)
{
    glUniform1i(appContents.u_sceneColumnIndex.location, index);
}
