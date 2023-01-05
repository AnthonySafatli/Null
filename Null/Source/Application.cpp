#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"
#include "Headers/Contents.h"

int main(void)
{
    /* Initialize GLFW */
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    // int width = 1800;
    // int height = 1100;

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

    glfwSetKeyCallback(window, Contents::ProcessKey);
    glfwSetCharCallback(window, Contents::ProcessChar);
    glfwSetFramebufferSizeCallback(window, Contents::OnResize);

    /* OpenGL Setup */
    if (glewInit())
        return -3;

    glClearColor(0.03, 0.05, 0.09, 0.85);

    VertexBuffer vertexBuffer(maxSquaresToRender);

    VertexArray::EnableAttributes();

    IndexBuffer indexBuffer(maxSquaresToRender);

    ShaderProgram program(
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\vertex.glsl",
        "C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\fragment.glsl"
    );
    
    Texture texture("C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Textures\\font texture.png");

    Uniform u_idealRatio(program.handle, "idealRatio");
    Uniform u_size(program.handle, "size");
    Uniform u_tex(program.handle, "tex");

    glUniform2f(u_idealRatio.location, 1.0, 1.0);
    glUniform1f(u_size.location, Contents::textSize);
    glUniform1i(u_tex.location, 0);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        // TODO: Resize event
        
        vertexBuffer.SetData(Contents::vertices);
        indexBuffer.SetData(Contents::indices);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, Contents::indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    glfwTerminate();

    return 0;
}
