//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_VBO_H
#define GRAPHICS_TEST_VBO_H

#include "renderer.h"

/**
 * A struct representing a vertex.
 */
typedef struct
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
} vertex_t;

/**
 * Struct containing all mesh_data for a VBO Mesh.
 */
typedef struct {
    unsigned int indices_count;
    unsigned int vertices_count;
    unsigned int *indices;
    vertex_t *vertices;
    GLuint texture_id;
} vbo_data_t;

/**
 * A vertex_t Buffer Object.
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
    GLsizei size;

    /** Rendering mode of this VBO */
    unsigned int renderingMode = GL_TRIANGLES;

public:

    /**
     * Constructor for creating a new VBO
     * @param flags The flags of the VBO. These
     * flags determine what kind of mesh_data the VBO
     * will hold, such as vertex positions and colors.
     */
    explicit VBO();

    /**
     * Constructor for creating a VBO with a specific rendering mode
     */
    VBO(unsigned int renderingMode);

    /**
     * Destructor for the VBO.
     */
    ~VBO();

    /**
     * Method for supplying vector mesh_data to the VBO.
     * This method will pass the mesh_data onto the appropriate buffer,
     * depending on the provided
     *
     * @param data The mesh_data to withVertices to the VBO.
     * @param segmentSize The size of each segment in the mesh_data.
     * @param vectorDataType The kind of mesh_data that is being supplied.
     */
    void withVertices(std::vector<vertex_t> vertices);

    /**
     * Method for supplying vector mesh_data to the VBO.
     * This method will pass the mesh_data onto the appropriate buffer,
     * depending on the provided. Method is similar to `withVertices(std::vector<vertex_t> vertices)`
     *
     * @param data The mesh_data to withVertices to the VBO.
     * @param segmentSize The vertexCount of each segment in the mesh_data.
     * @param vectorDataType The kind of mesh_data that is being supplied.
     */
    void withVertices(vertex_t *vertices, unsigned long vertexCount);

    /**
     * Method for supplying index mesh_data to the VBO.
     * This method will pass the mesh_data onto the appropriate buffer,
     * depending on the provided
     * @param data The mesh_data to withVertices to the VBO.
     * @param segmentSize The size of each segment in the mesh_data.
     * @param vectorDataType The kind of mesh_data that is being supplied.
     */
    void withIndices(std::vector<unsigned int> indices);

    /**
     * Method for supplying index mesh_data to the VBO.
     * This method will pass the mesh_data onto the appropriate buffer,
     * depending on the provided. Method is similar to `withIndices(std::vector<unsigned int> indices)`
     *
     * @param data The mesh_data to withVertices to the VBO.
     * @param segmentSize The indicesCount of each segment in the mesh_data.
     * @param vectorDataType The kind of mesh_data that is being supplied.
     */
    void withIndices(unsigned int *indices, unsigned long indicesCount);

    /**
     * Build the VBO.
     */
    void build();

    /**
     * Render the VBO onto the screen.
     */
    void draw(float deltaTime);
};

#endif //GRAPHICS_TEST_VBO_H
