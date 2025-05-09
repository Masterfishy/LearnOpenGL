
// Emscripten includes
#include <emscripten.h>
#include <GLFW/glfw3.h>

// System includes
#include <iostream>

GLFWwindow* gWindow = nullptr;

void mainLoop()
{
    if (gWindow == NULL)
    {
        return;
    }

    // Start of frame
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // End of frame
    glfwSwapBuffers(gWindow);
    glfwPollEvents();
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

    emscripten_set_main_loop(&mainLoop, 0, true);

    glfwDestroyWindow(gWindow);
    glfwTerminate();
    return 0;
}
