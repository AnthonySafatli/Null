#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW and Window */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(1800, 1100, "Hello World", NULL, NULL);
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
    glClearColor(0.025, 0.05, 0.09, 0.85);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Drawing */
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}