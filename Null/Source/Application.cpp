#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"
#include "Headers/Contents.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW and Window */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    // int width = 1800;
    // int height = 1100;

    int width = 800;
    int height = 1400;

    int maxSquaresToRender = 1000;

    window = glfwCreateWindow(width, height, "Null", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    /* Initialize GLEW */
    if (glewInit())
        return -3;

    /* Null Setup */
    Contents appContents;

    /* OpenGL Setup */
    glClearColor(0.03, 0.05, 0.09, 0.85);

    VertexBuffer vertexBuffer(maxSquaresToRender);

    VertexArray::EnableAttributes();

    IndexBuffer indexBuffer(maxSquaresToRender);

    ShaderProgram program(
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\shader.vert",
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\shader.frag"
    );

    Texture texture("C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Textures\\alphabet texture.png");

    Uniform u_dimensions(program.handle, "dimensions");
    Uniform u_idealDimensions(program.handle, "idealDimensions");
    Uniform u_size(program.handle, "size");
    Uniform u_tex(program.handle, "tex");

    glUniform2f(u_dimensions.location, (float)width, (float)height);
    glUniform2f(u_idealDimensions.location, (float)width, (float)height);
    glUniform1f(u_size.location, appContents.textSize);
    glUniform1i(u_tex.location, 0);

    {
        // Initial Render

        appContents.AddCharacter('>');
        appContents.AddSpace();
        appContents.AddCharacter('a');
        appContents.AddCharacter('b');
        appContents.AddCharacter('c');

        vertexBuffer.SetData(appContents.vertices);
        indexBuffer.SetData(appContents.indices);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, appContents.indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    bool sceneChanged = false;

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Drawing */
        
        /* keyboard event

        appContents.AddCharacter(character);

        vertexBuffer.SetData(appContents.vertices);
        indexBuffer.SetData(appContents.indices);

        sceneChanged = true;

        */

        if (sceneChanged)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawElements(GL_TRIANGLES, appContents.indices.size(), GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window);


            /* TODO: Event handling
            * Resize
            * Text Input
            * Other character Input
            */
        }

        glfwPollEvents();

        sceneChanged = false;
    }

    glfwTerminate();

    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}