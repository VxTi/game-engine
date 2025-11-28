//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_SHADER_H
#define GRAPHICS_TEST_SHADER_H

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

typedef enum {
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER
} ShaderType;

class Shader {

private:
  // Paths to the sources of both fragment and vertex files
  char *vertexSourcePath;
  char *fragmentSourcePath;

  GLuint programId;
  GLuint fragmentShaderId;
  GLuint vertexShaderId;

private:
  void loadSource(GLuint shaderId, const char *sourcePath);

public:
  /**
   * Constructor for creating a shader with
   * @param fragmentSourcePath
   * @param vertexSourcePath
   */
  Shader(char *fragmentSourcePath, char *vertexSourcePath);
  Shader();

  // Destructor
  ~Shader();

  /**
   * Sets the source of the shader program.
   */
  void setSourcePath(char *sourcePath, ShaderType shaderType);

  /**
   * Compiles the shader.
   */
  void compile();

  /**
   * Binds the shader to the current OpenGL context.
   */
  void bind();

  /**
   * Unbinds the shader from the current OpenGL context.
   */
  void unbind();

  /**
   * Sends a floating point value to the shader
   */
  void uniformFloat(const char *name, float value) const;

  /**
   * Sends an array of floating point numbers to the shader
   */
  void uniformNFloat(const char *name, int count, float *value) const;

  /**
   * Sends a vec2 (2 floating point values) to the shader
   */
  void uniformVec2(const char *name, float x, float y) const;
  void uniformVec2(const char *name, glm::vec2 vector) const;

  /**
   * Sends a vec3 (3 floating point values) to the shader
   */
  void uniformVec3(const char *name, float x, float y, float z) const;
  void uniformVec3(const char *name, glm::vec3 vector) const;

  /**
   * Sends a vec4 (4 floating point values) to the shader
   */
  void uniformVec4(const char *name, float x, float y, float z, float w) const;
  void uniformVec4(const char *name, glm::vec4 vector) const;

  /**
   * Sends a mat4 (4x4 matrix) to the shader
   */
  void uniformMat4(const char *name, float *value) const;
  void uniformMat4(const char *name, glm::mat4 matrix) const;

  /**
   * Sends a texture to the shader.
   */
  void uniformTexture(const char *name, GLuint textureId);

  /**
   * Get the program ID of the shader.
   * @return The program ID.
   */
  GLuint getProgramId() const;
};

#endif // GRAPHICS_TEST_SHADER_H
