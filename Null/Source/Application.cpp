#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Headers/GLAbstraction.h"
#include "Headers/Contents.h"
#include "Headers/CallBack.h"

Contents appContents = Contents(1800, 1100, 4, 0.3, 0, 0);

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
    // CallBack::TargetContents = appContents;

    glfwSetKeyCallback(window, CallBack::KeyCallBack);
    glfwSetCharCallback(window, CallBack::CharCallBack);
    glfwSetFramebufferSizeCallback(window, CallBack::FrameBuffeResizeCallBack);

    /* OpenGL Setup */
    if (glewInit())
        return -3;

    glClearColor(0.03, 0.05, 0.09, 0.85);

    // TODO: move to app contents
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

    glUniform2f(u_idealRatio.location, appContents.idealWidth / 1800.0, appContents.idealHeight / 1100.0);
    glUniform1f(u_size.location, appContents.textSize);
    glUniform1i(u_tex.location, 0);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        vertexBuffer.SetData(appContents.vertices);
        indexBuffer.SetData(appContents.indices);

        glDrawElements(GL_TRIANGLES, appContents.indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    glfwTerminate();

    return 0;
}
