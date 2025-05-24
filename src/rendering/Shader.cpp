#include "Shader.h"
#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader* Shader::currentShader = nullptr;

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    unsigned int vertexShaderId = createShader(vertexShaderPath, GL_VERTEX_SHADER);
    unsigned int fragmentShaderId = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertexShaderId);
    glAttachShader(m_programId, fragmentShaderId);

    glLinkProgram(m_programId);
    GLint program_linked;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(m_programId, 1024, &log_length, message);
        std::cout << "ERROR: Shader program linking error: " << message << std::endl;
    }

    glValidateProgram(m_programId);
    GLint programValidated;
    glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &programValidated);
    if (programValidated != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(m_programId, 1024, &log_length, message);
        std::cout << "ERROR: Shader program validation error: " << message << std::endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void Shader::useShader() {
    currentShader = this;
    glUseProgram(m_programId);
}

void Shader::setUniformMatrix4f(const char* uniform, float* value) {
    glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, value);
}

void Shader::setUniformMatrix4fv(const char* uniform, int count, float* value) {
    glUniformMatrix4fv(getUniformLocation(uniform), count, GL_FALSE, value);
}

void Shader::setUniform3fv(const char* uniform, int count, float* value) {
    glUniform3fv(getUniformLocation(uniform), count, value);
}

void Shader::setUniform2i(const char* uniform, int value1, int value2) {
    glUniform2i(getUniformLocation(uniform), value1, value2);
}

void Shader::setUniform2f(const char* uniform, float value1, float value2) {
    glUniform2f(getUniformLocation(uniform), value1, value2);
}

int Shader::getUniformLocation(const char* uniform) {
    auto locationSearch = uniformLocations.find(uniform);
    if(locationSearch == uniformLocations.end()) {
        int location = glGetUniformLocation(m_programId, uniform);
        uniformLocations.insert(std::pair<const char*, int>(uniform, location));
        return location;
    }

    return locationSearch->second;
}

std::string Shader::getShaderSource(const char* shaderPath) {
    std::ifstream file(shaderPath);
    if(!file.is_open()) {
        std::cout << "Failed to open file" << shaderPath << std::endl;
        return "";
    }

    std::stringstream fileBuffer;
    fileBuffer << file.rdbuf();

    return fileBuffer.str();
}

unsigned int Shader::createShader(const char* shaderPath, int shaderType) {
    unsigned int shaderId = glCreateShader(shaderType);

    std::string shaderSourceStr = getShaderSource(shaderPath);
    const char* shaderSource = shaderSourceStr.c_str();
    int length = shaderSourceStr.size();
    glShaderSource(shaderId, 1, &shaderSource, &length);

    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderId, 1024, &log_length, message);

        std::cout << "ERROR: Shader compilation error: " << message << std::endl;
    }

    return shaderId;
}
