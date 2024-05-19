//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include <fstream>
#include "Rendering.h"
#include "../io/Files.h"
#include <iostream>

void Shader::load(const char * source, ShaderType type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed: " << infoLog << std::endl;
        exit(1);
    }
    if (type == FRAGMENT) {
        this->fragmentShaderId = shader;
    } else {
        this->vertexShaderId = shader;
    }
}

void Shader::compile() {
    this->programId = glCreateProgram();
    glAttachShader(programId, this->fragmentShaderId);
    glAttachShader(programId, this->vertexShaderId);
    glLinkProgram(programId);
    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cout << "Shader linking failed: " << infoLog << std::endl;
        return;
    }
}

void Shader::bind() {
    glUseProgram(this->programId);
}

void Shader::unbind() {
    glUseProgram(0);
}

Shader::Shader(std::string sourcePath, std::string sourceName)
{
    std::basic_string vertexSrc = Files::readFile((sourcePath + "/" + sourceName + "_vert.glsl").c_str());
    std::basic_string fragSrc = Files::readFile((sourcePath + "/" + sourceName + "_frag.glsl").c_str());
    this->load(vertexSrc.c_str(), VERTEX);
    this->load(fragSrc.c_str(), FRAGMENT);
    this->compile();
}

GLuint Shader::getProgramId() const {
    return this->programId;
}

Shader::Shader() {}

