
// Emscripten includes
#include <emscripten.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

// System includes
#include <iostream>

#define DRAW_TRIANGLE false

/////////////
// Shaders //
/////////////

const char* vertexShaderSource = "#version 300 es\n"
    "precision mediump float;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

////////////////
// GL Objects //
////////////////

GLFWwindow* gWindow = nullptr;

unsigned int shaderProgram;
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

/////////////
// Program //
/////////////

void mainLoop()
{
    if (gWindow == NULL)
    {
        return;
    }

    // Start of frame
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

#if DRAW_TRIANGLE
    glDrawArrays(GL_TRIANGLES, 0, 3);
#else
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
    glBindVertexArray(0);

    // End of frame
    glfwSwapBuffers(gWindow);
    glfwPollEvents();
}

int main()
{
    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gWindow = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (gWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(gWindow);
    glViewport(0, 0, 800, 600);

    /////////////////////
    // Compile shaders //
    /////////////////////

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int vertexShader_compileSuccess;
    char vertexShader_infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShader_compileSuccess);
    if (!vertexShader_compileSuccess)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexShader_infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexShader_infoLog << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int fragmentShader_compileSuccess;
    char fragmentShader_infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShader_compileSuccess);
    if (!fragmentShader_compileSuccess)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShader_infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShader_infoLog << std::endl;
        return EXIT_FAILURE;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int shaderProgram_linkSuccess;
    char shaderProgram_infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgram_linkSuccess);
    if (!shaderProgram_linkSuccess)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgram_infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << shaderProgram_infoLog << std::endl;
        return EXIT_FAILURE;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /////////////////
    // Vertex Data //
    /////////////////

    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    float rectangleVertices[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
    };
    unsigned int rectangleIndices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

#if DRAW_TRIANGLE
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); // Using static draw because the triangle doesn't move
#else
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
#endif

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /////////////////////
    // Run the program //
    /////////////////////

    emscripten_set_main_loop(&mainLoop, 0, true);

    //////////////
    // Shutdown //
    //////////////

    glfwDestroyWindow(gWindow);
    glfwTerminate();
    return EXIT_SUCCESS;
}
