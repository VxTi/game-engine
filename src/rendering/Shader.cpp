//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include <fstream>
#include "Shader.h"
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

void Shader::uniformFloat(const char *name, float value) {
    glUniform1f(glGetUniformLocation(this->programId, name), value);
}

void Shader::uniformNFloat(const char *name, int count, float *value) {
    glUniform1fv(glGetUniformLocation(this->programId, name), count, value);
}

void Shader::uniformVec3(const char *name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(this->programId, name), x, y, z);
}

void Shader::uniformVec4(const char *name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(this->programId, name), x, y, z, w);
}

void Shader::uniformMat4(const char *name, float *value) {
    glUniformMatrix4fv(glGetUniformLocation(this->programId, name), 1, GL_FALSE, value);
}

Shader::Shader() {}

Shader::~Shader() {
    glDeleteProgram(this->programId);
    glDeleteShader(this->vertexShaderId);
    glDeleteShader(this->fragmentShaderId);
}

