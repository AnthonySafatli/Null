#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>

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

    /* Loading Texture */
    stbi_set_flip_vertically_on_load(1);

    int texWidth, texHeight, numColourChannels;
    unsigned char* bytes = stbi_load("\\Resources\\Textures\\alphabet-texture.bmp", 
        &texWidth, &texHeight, &numColourChannels, 4);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* OpenGL Setup */
    glClearColor(0.03, 0.05, 0.09, 0.85);

    // TODO: OpenGL Stuff

    // Get vertices based on height and width

    VertexBuffer veretxBuffer;
    // Pass in data?

    IndexBuffer indexBuffer;
    // Pass in data?

    VertexArray vertexArray(veretxBuffer.handle);

    // Get shader source code

    // Create shader program object

    // Set uniforms

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Drawing */
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        // TODO: Event handling
        // Resize
        // Text Input
        // Other character Input

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture);

    return 0;
}