//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include <engine/io/files.h>
#include <engine/renderer/shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

ShaderCompileResult Shader::addFragmentShader(const char *path) {
  this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  if (!this->fragmentShaderId) {
    std::cerr << "Shader Error - Failed to create fragment shader."
              << std::endl;
    return FAILURE;
  }

  return this->loadSource(this->fragmentShaderId, path);
}

ShaderCompileResult Shader::addVertexShader(const char *path) {
  this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

  if (!this->vertexShaderId) {
    std::cerr << "Shader Error - Failed to create vertex shader." << std::endl;
    return FAILURE;
  }

  return this->loadSource(this->vertexShaderId, path);
}

ShaderCompileResult Shader::loadSource(GLuint shaderId,
                                       const char *sourcePath) {
  std::string source = Files::read(sourcePath);

  if (source.empty()) {
    std::cerr << "Shader Error - Failed to read source file: " << sourcePath
              << std::endl;
    return FAILURE;
  }

  const char *sourceChar = source.c_str();
  glShaderSource(shaderId, 1, &sourceChar, NULL);
  glCompileShader(shaderId);
  GLint status;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
  if (!status) {
    GLchar errorLog[512];
    glGetShaderInfoLog(shaderId, 512, NULL, errorLog);
    std::cerr << "Shader Error - Compilation failed: " << errorLog << std::endl;
    return FAILURE;
  }
  return SUCCESS;
}

ShaderCompileResult Shader::compile() {
  this->programId = glCreateProgram();
  glAttachShader(this->programId, this->fragmentShaderId);
  glAttachShader(this->programId, this->vertexShaderId);
  glLinkProgram(this->programId);

  GLint linkStatus;
  GLint validateStatus;
  GLchar buffer[512];
  glGetProgramiv(this->programId, GL_LINK_STATUS, &linkStatus);

  if (!linkStatus) {
    glGetProgramInfoLog(this->programId, 512, NULL, buffer);
    std::cerr << "Shader Error - Linking failed: " << buffer << std::endl;
    return FAILURE;
  }

  glValidateProgram(this->programId);
  glGetProgramiv(this->programId, GL_VALIDATE_STATUS, &validateStatus);

  if (!validateStatus) {
    glGetProgramInfoLog(this->programId, 512, NULL, buffer);
    std::cerr << "Shader Error - Validation failed: " << buffer << std::endl;
    return FAILURE;
  }
  return SUCCESS;
}

void Shader::bind() { glUseProgram(this->programId); }

void Shader::unbind() { glUseProgram(0); }

GLuint Shader::getProgramId() const { return this->programId; }

void Shader::uniformFloat(const char *name, float value) const {
  glUniform1f(glGetUniformLocation(this->programId, name), value);
}

void Shader::uniformNFloat(const char *name, int count, float *value) const {
  glUniform1fv(glGetUniformLocation(this->programId, name), count, value);
}

void Shader::uniformVec2(const char *name, vec2 vector) const {
  Shader::uniformVec2(name, vector.x, vector.y);
}

void Shader::uniformVec2(const char *name, float x, float y) const {
  glUniform2f(glGetUniformLocation(this->programId, name), x, y);
}

void Shader::uniformVec3(const char *name, vec3 vector) const {
  Shader::uniformVec3(name, vector.x, vector.y, vector.z);
}

void Shader::uniformVec3(const char *name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(this->programId, name), x, y, z);
}

void Shader::uniformVec4(const char *name, vec4 vector) const {
  Shader::uniformVec4(name, vector.x, vector.y, vector.z, vector.w);
}

void Shader::uniformVec4(const char *name, float x, float y, float z,
                         float w) const {
  glUniform4f(glGetUniformLocation(this->programId, name), x, y, z, w);
}

void Shader::uniformMat4(const char *name, mat4 matrix) const {
  Shader::uniformMat4(name, value_ptr(matrix));
}

void Shader::uniformMat4(const char *name, float *value) const {
  glUniformMatrix4fv(glGetUniformLocation(this->programId, name), 1, GL_FALSE,
                     value);
}

Shader::~Shader() {
  glDeleteProgram(this->programId);
  glDeleteShader(this->vertexShaderId);
  glDeleteShader(this->fragmentShaderId);
}
