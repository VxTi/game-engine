//
// Created by Luca Warmenhoven on 17/05/2024.
//

#ifndef GRAPHICS_TEST_RENDERING_H
#define GRAPHICS_TEST_RENDERING_H

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OpenGL/gl3.h>
#include "GLFW/glfw3.h"
#include <vector>

#define RENDER_MODE_2D 0
#define RENDER_MODE_3D 1

class Rendering
{
private:
    /** The position and rotation of the object. */
    glm::vec4 position;
    glm::vec4 rotation;

    /** The model, view and projection matrices. */
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;

    unsigned char renderMode;

public:

    Rendering();

    /**
     * Function that prepares all the matrices and
     * the OpenGL context for rendering.
     */
    void computeMatrices(float fov, float zNear, float zFar, float width, float height);

    /**
     * Function that prepares the object for rendering.
     */
    void pushMatrices(GLuint programId);

    /**
     * Function that translates the object.
     * @param pos The pos to translate to.
     */
    void translate(glm::vec4 pos);

    /**
     * Function that translates the object.
     * @param x The x position to translate to.
     * @param y The y position to translate to.
     * @param z The z position to translate to.
     */
    void translate(float x, float y, float z);

    /**
     * Function that translates the object on the x axis.
     * @param x The x position to translate to.
     */
    void translateX(float x);

    /**
     * Function that translates the object on the y axis.
     * @param y The y position to translate to.
     */
    void translateY(float y);

    /**
     * Function that translates the object on the z axis.
     * @param z The z position to translate to.
     */
    void translateZ(float z);

    /**
     * Function that rotates the object.
     * @param rotation The rotation to rotate to.
     */
    void rotate(glm::vec4 rotation);

    /**
     * Function that rotates the object.
     * @param x The x rotation to rotate to.
     * @param y The y rotation to rotate to.
     * @param z The z rotation to rotate to.
     */
    void rotate(float x, float y, float z);

    /**
     * Function that rotates the object on the x axis.
     * @param radians The x rotation to rotate to.
     */
    void rotateX(float radians);

    /**
     * Function that rotates the object on the y axis.
     * @param radians The y rotation to rotate to.
     */
    void rotateY(float radians);

    /**
     * Function that rotates the object on the z axis.
     * @param radians The z rotation to rotate to.
     */
    void rotateZ(float radians);

    /**
     * Function that sets the model matrix.
     * @param model The model matrix to set.
     */
    void setModelMatrix(glm::mat4 model);

    /**
     * Function that sets the view matrix.
     * @param view The view matrix to set.
     */
    void setViewMatrix(glm::mat4 view);

    /**
     * Function that sets the projection matrix.
     * @param projection The projection matrix to set.
     */
    void setProjectionMatrix(glm::mat4 projection);

    /**
     * Function that sets the model, view and projection matrices.
     * @param model The model matrix to set.
     * @param view The view matrix to set.
     * @param projection The projection matrix to set.
     */
    void setModelViewProjectionMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    /**
     * Function that sets the render mode.
     * @param renderMode The render mode to set.
     */
    void setRenderMode(unsigned char renderMode);

    static const glm::mat4 IDENTITY;
};

typedef enum {
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

    /**
     * Load the shader from a source file.
     * @param source The source file.
     * @param type The type of shader.
     */
    void load(const char * source, ShaderType type);

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
     * Get the program ID of the shader.
     * @return The program ID.
     */
    GLuint getProgramId() const;

};

#define VBO_POSITION 1
#define VBO_COLOR 2
#define VBO_NORMAL 4
#define VBO_UV 8

#define VBO_POS_UV_NORMAL (VBO_POSITION | VBO_UV | VBO_NORMAL)

#define VBO_POSITION_INDEX 0
#define VBO_NORMAL_INDEX 1
#define VBO_UV_INDEX 2

#define VBO_FLAG_COUNT 4

/**
 * A struct representing a vertex.
 */
typedef struct {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
} Vertex;


/**
 * A Vertex Buffer Object.
 */
class VBO
{

private:

    /** The ID of the VAO */
    GLuint vaoId;

    /** Pointer to the array buffer of the VBO */
    GLuint vboBufferId;

    /** Pointer to the element buffer of the VBO */
    GLuint eboBufferId;

    /** The size of the VBO */
    unsigned long size;

    /** The enabled flags of this VBO */
    unsigned char flags;

public:

    /**
     * Constructor for creating a new VBO
     * @param flags The flags of the VBO. These
     * flags determine what kind of data the VBO
     * will hold, such as vertex positions and colors.
     */
    explicit VBO();

    /**
     * Method for supplying vector data to the VBO.
     * This method will pass the data onto the appropriate buffer,
     * depending on the provided
     * @param data The data to withVertices to the VBO.
     * @param segmentSize The size of each segment in the data.
     * @param vectorDataType The kind of data that is being supplied.
     */
    void withVertices(std::vector<Vertex> vertices);

    /**
     * Method for supplying index data to the VBO.
     * This method will pass the data onto the appropriate buffer,
     * depending on the provided
     * @param data The data to withVertices to the VBO.
     * @param segmentSize The size of each segment in the data.
     * @param vectorDataType The kind of data that is being supplied.
     */
    void withIndices(std::vector<int> indices);

    /**
     * Build the VBO.
     */
    void build();

    /**
     * Render the VBO onto the screen.
     */
    void render() const;

};


#endif //GRAPHICS_TEST_RENDERING_H
