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

    int maxSquaresToRender = 1000;

    window = glfwCreateWindow(Contents::width, Contents::height, "Null", NULL, NULL);
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

    // move to app contents
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

    glUniform2f(u_idealRatio.location, IDEAL_WIDTH / 1800.0, IDEAL_HEIGHT / 1100.0);
    glUniform1f(u_size.location, Contents::textSize);
    glUniform1i(u_tex.location, 0);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        vertexBuffer.SetData(Contents::vertices);
        indexBuffer.SetData(Contents::indices);

        glDrawElements(GL_TRIANGLES, Contents::indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    glfwTerminate();

    return 0;
}
