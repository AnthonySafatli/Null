#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW and Window */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    int width = 1800;
    int height = 1100;

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

    /* OpenGL Setup */
    glClearColor(0.03, 0.05, 0.09, 0.85);

    Texture texture("\\Resources\\Textures\\alphabet-texture.bmp");

    // TODO: Get vertices based on height and width

    VertexBuffer veretxBuffer;
    // Pass in data?

    IndexBuffer indexBuffer;
    // Pass in data?

    VertexArray vertexArray(veretxBuffer.handle);

    // TODO: Remove absolute paths
    ShaderProgram program("C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\shader.vert",
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\shader.frag");

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Drawing */
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        // TODO: Event handling
        /* 
        * Resize
        * Text Input
        * Other character Input 
        */

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}