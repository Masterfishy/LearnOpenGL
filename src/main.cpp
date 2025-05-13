#include "shader.hpp"

// Emscripten includes
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>

// System includes
#include <iostream>
#include <math.h>

#define DRAW_TRIANGLE true

/////////////
// Shaders //
/////////////

const char *vertexShaderSource = "#version 300 es\n"
                                 "precision mediump float;\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "void main(){\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "   ourColor = aColor;\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 300 es\n"
                                   "precision mediump float;\n"
                                   "in vec3 ourColor;\n"
                                   "out vec4 FragColor;\n"
                                   "void main(){\n"
                                   "   FragColor = vec4(ourColor, 1.0);\n"
                                   "}\0";

////////////////
// GL Objects //
////////////////

GLFWwindow *window = nullptr;

Shader shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

/////////////
// Program //
/////////////

void mainLoop()
{
    if (window == NULL)
    {
        return;
    }

    // Start of frame
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw
    shader.use();

    glBindVertexArray(VAO);

#if DRAW_TRIANGLE
    glDrawArrays(GL_TRIANGLES, 0, 3);
#else
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
    glBindVertexArray(0);

    // End of frame
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main()
{
    ///////////////////
    // Create Window //
    ///////////////////

    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);

    /////////////////
    // Vertex Data //
    /////////////////

#if DRAW_TRIANGLE
    float vertices[] = {
        // positions        // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
    };
#else
    float vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
#endif

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

#if DRAW_TRIANGLE
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW); // Using static draw because the triangle doesn't move

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
#else
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
#endif

    /////////////////////
    // Run the program //
    /////////////////////

    emscripten_set_main_loop(&mainLoop, 0, true);

    //////////////
    // Shutdown //
    //////////////

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
