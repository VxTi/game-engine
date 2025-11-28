//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_SHADER_H
#define GRAPHICS_TEST_SHADER_H

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

#define SHADER_LOG_BUFFER_SIZE (512)

typedef enum {
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER
} ShaderType;

typedef enum {
  SUCCESS = 1,
  FAILURE = 0,
} ShaderCompileResult;

class Shader {

private:
  GLuint programId;
  GLuint fragmentShaderId;
  GLuint vertexShaderId;

private:
  ShaderCompileResult loadSource(GLuint shaderId, const char *sourcePath);

public:
  /**
   * Generic constructor
   */
  Shader();

  ShaderCompileResult addFragmentShader(const char *path);
  ShaderCompileResult addVertexShader(const char *path);

  /**
   * Compiles the shader.
   */
  ShaderCompileResult compile();

  /**
   * Static method for creating a shader from a single source file.
   */
  static Shader *fromSource(const char *path);

  // Destructor
  ~Shader();

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
