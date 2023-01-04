#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"
#include "Headers/Contents.h"

void ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW and Window */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    // int width = 1800;
    // int height = 1100;

    int idealWidth = 800;
    int idealHeight = 1400; // move to app contents
    
    int width = 800;
    int height = 1400; // move to app contents

    int maxSquaresToRender = 1000;

    window = glfwCreateWindow(width, height, "Null", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, ProcessKey);

    /* Initialize GLEW */
    if (glewInit())
        return -3;

    /* OpenGL Setup */
    glClearColor(0.03, 0.05, 0.09, 0.85);

    VertexBuffer vertexBuffer(maxSquaresToRender);

    VertexArray::EnableAttributes();

    IndexBuffer indexBuffer(maxSquaresToRender);

    ShaderProgram program(
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\vertex.glsl",
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\fragment.glsl"
    );

    Texture texture("C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Textures\\alphabet texture.png");

    Uniform u_idealRatio(program.handle, "idealRatio");
    Uniform u_size(program.handle, "size");
    Uniform u_tex(program.handle, "tex");

    glUniform2f(u_idealRatio.location, (float)width / (float)idealWidth, (float)height / (float)idealHeight);
    glUniform1f(u_size.location, Contents::textSize);
    glUniform1i(u_tex.location, 0);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Drawing */
        
        /* keyboard event

        appContents.AddCharacter(character);

        vertexBuffer.SetData(appContents.vertices);
        indexBuffer.SetData(appContents.indices);

        sceneChanged = true;

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, appContents.indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);


        TODO: 
        Event handling
        Resize
        Text Input
        Other character Input
        */
        
        vertexBuffer.SetData(Contents::vertices);
        indexBuffer.SetData(Contents::indices);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, Contents::indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    glfwTerminate();

    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

void ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != 1)
        Contents::ProcessKey(key, action, mods);
}
