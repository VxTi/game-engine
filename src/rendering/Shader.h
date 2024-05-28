//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_SHADER_H
#define GRAPHICS_TEST_SHADER_H

#include <OpenGL/gl3.h>
#include <string>

typedef enum
{
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
} ShaderType;

class Shader
{

private:

    GLuint programId;
    GLuint fragmentShaderId;
    GLuint vertexShaderId;


public:
    Shader();

    Shader(std::string sourcePath, std::string sourceName);

    ~Shader();

    /**
     * Load the shader from a source file.
     * @param source The source file.
     * @param type The type of shader.
     */
    void load(const char *source, ShaderType type);

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
     * Uploads a floating point value to the shader
     */
    void uniformFloat(const char *name, float value);

    /**
     * Uploads an array of floating point numbers to the shader
     */
    void uniformNFloat(const char *name, int count, float *value);

    /**
     * Uploads a vec2 (2 floating point values) to the shader
     */
    void uniformVec2(const char *name, float x, float y);

    /**
     * Uploads a vec3 (3 floating point values) to the shader
     */
    void uniformVec3(const char *name, float x, float y, float z);

    /**
     * Uploads a vec4 (4 floating point values) to the shader
     */
    void uniformVec4(const char *name, float x, float y, float z, float w);

    /**
     * Uploads a mat4 (4x4 matrix) to the shader
     */
    void uniformMat4(const char *name, float *value);

    /**
     * Get the program ID of the shader.
     * @return The program ID.
     */
    GLuint getProgramId() const;

};

#endif //GRAPHICS_TEST_SHADER_H
