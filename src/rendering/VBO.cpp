//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include "VBO.h"

VBO::VBO() : Drawable(glm::vec3(0), glm::vec3(1), glm::vec3(0))
{
    glGenBuffers(1, &this->vboBufferId);
    glGenBuffers(1, &this->eboBufferId);
}

VBO::VBO(unsigned int renderingMode) : Drawable(glm::vec3(0), glm::vec3(1), glm::vec3(0))
{
    glGenBuffers(1, &this->vboBufferId);
    glGenBuffers(1, &this->eboBufferId);
    this->renderingMode = renderingMode;
}

VBO::~VBO()
{
    glDeleteBuffers(1, &this->vboBufferId);
    glDeleteBuffers(1, &this->eboBufferId);
    glDeleteVertexArrays(1, &this->vaoId);
}

void VBO::withVertices(vertex_t *vertices, unsigned long vertexCount)
{
    if ( vertexCount == 0 )
        throw std::invalid_argument("Invalid mesh_data supplied to VBO");
    glBindBuffer(GL_ARRAY_BUFFER, this->vboBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vertex_t), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/** Supply the VBO with vertices */
void VBO::withVertices(std::vector<vertex_t> vertices)
{
    this->withVertices(vertices.data(), vertices.size());
}

void VBO::withIndices(unsigned int *indices, unsigned long indicesCount)
{
    if ( indicesCount == 0 )
        throw std::invalid_argument("Invalid mesh_data supplied to VBO");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    this->size = (GLsizei) indicesCount;
}

/** Supply the VBO with indices */
void VBO::withIndices(std::vector<unsigned int> indices)
{
    this->withIndices(indices.data(), indices.size());
}


/**
 * Build the VBO using VAOs
 */
void VBO::build()
{
    glGenVertexArrays(1, &this->vaoId);
    glBindVertexArray(this->vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboBufferId);

    glEnableVertexAttribArray(VBO_POSITION_INDEX);
    glVertexAttribPointer(VBO_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid *) offsetof(vertex_t, x));

    glEnableVertexAttribArray(VBO_NORMAL_INDEX);
    glVertexAttribPointer(VBO_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid *) offsetof(vertex_t, nx));

    glEnableVertexAttribArray(VBO_UV_INDEX);
    glVertexAttribPointer(VBO_UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid *) offsetof(vertex_t, u));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBufferId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VBO::draw(float deltaTime)
{
    glBindVertexArray(this->vaoId);
    glDrawElements(this->renderingMode, this->size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

