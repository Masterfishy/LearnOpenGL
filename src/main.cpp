
// Emscripten includes
#include <emscripten.h>
#include <GLFW/glfw3.h>

// System includes
#include <iostream>

GLFWwindow* gWindow;

void mainLoop()
{
    if (gWindow == NULL)
    {
        return;
    }

    while (!glfwWindowShouldClose(gWindow))
    {
        glfwSwapBuffers(gWindow);
        glfwPollEvents();
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gWindow = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (gWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(gWindow);

    glViewport(0, 0, 800, 600);

    emscripten_set_main_loop(mainLoop, 0, false);

    glfwTerminate();
    return 0;
}