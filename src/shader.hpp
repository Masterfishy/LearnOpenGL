#pragma once

// Emscripten includes
#include <GLES3/gl3.h>

// System includes
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
  private:
    /// @brief The initialized state of the shader program.
    bool mInitialized;

  public:
    /// @brief The shader program ID.
    unsigned int ID;

    /// @brief Read and build the given shaders.
    /// @param vertexSourcePath     The path to the vertex shader source.
    /// @param fragmentSourcePath   The path to the fragment shader source.
    Shader(const char *vertexSourcePath, const char *fragmentSourcePath) : mInitialized(true), ID(0)
    {
        //////////////////////////
        // Retrieve Source Code //
        //////////////////////////

        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;

        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Open the files
            vertexShaderFile.open(vertexSourcePath);
            fragmentShaderFile.open(fragmentSourcePath);

            std::stringstream vertexShaderStream, fragmentShaderStream;

            // Read in the file buffers
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            // Close the files
            vertexShaderFile.close();
            fragmentShaderFile.close();

            // Convert streams to strings
            vertexCode = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
            mInitialized = false;
            return;
        }

        const char *vertexShaderCode = vertexCode.c_str();
        const char *fragmentShaderCode = fragmentCode.c_str();

        /////////////////////
        // Compile Shaders //
        /////////////////////

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            mInitialized = false;
        }

        // Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            mInitialized = false;
        }

        // Shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
            mInitialized = false;
        }

        // Delete shaders
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    /// @brief Use/activate the shader.
    void use()
    {
        if (mInitialized)
        {
            glUseProgram(ID);
        }
    }

    /// @brief Set the bool value of the given named uniform.
    /// @param name     The name of the uniform.
    /// @param value    The value to set the uniform.
    void setBool(const std::string &name, bool value) const
    {
        if (mInitialized)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
        }
    }

    /// @brief Set the int value of the given named uniform.
    /// @param name     The name of the uniform.
    /// @param value    The value to set the uniform.
    void setInt(const std::string &name, int value) const
    {
        if (mInitialized)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
    }

    /// @brief Set the float value of the given named uniform.
    /// @param name     The name of the uniform.
    /// @param value    The value to set the uniform.
    void setFoat(const std::string &name, float value) const
    {
        if (mInitialized)
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
    }
};
