//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include "VBO.h"

VBO::VBO() : Drawable(glm::vec3(0), glm::vec3(1), glm::vec3(0)){
    glGenBuffers(1, &this->vboBufferId);
    glGenBuffers(1, &this->eboBufferId);
}

VBO::~VBO() {
    glDeleteBuffers(1, &this->vboBufferId);
    glDeleteBuffers(1, &this->eboBufferId);
    glDeleteVertexArrays(1, &this->vaoId);
}

void VBO::withVertices(Vertex *vertices, unsigned long vertexCount)
{
    if ( vertexCount == 0 )
        throw std::invalid_argument("Invalid data supplied to VBO");
    glBindBuffer(GL_ARRAY_BUFFER, this->vboBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/** Supply the VBO with vertices */
void VBO::withVertices(std::vector<Vertex> vertices) {
    if ( vertices.empty() )
        throw std::invalid_argument("Invalid data supplied to VBO");
    glBindBuffer(GL_ARRAY_BUFFER, this->vboBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::withIndices(unsigned int *indices, unsigned long indicesCount)
{
    if ( indicesCount == 0 )
        throw std::invalid_argument("Invalid data supplied to VBO");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    this->size = indicesCount;
}

/** Supply the VBO with indices */
void VBO::withIndices(std::vector<unsigned int> indices) {
    this->withIndices(indices.data(), indices.size());
}


/**
 * Build the VBO using VAOs
 */
void VBO::build() {
    glGenVertexArrays(1, &this->vaoId);
    glBindVertexArray(this->vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboBufferId);

    glEnableVertexAttribArray(VBO_POSITION_INDEX);
    glVertexAttribPointer(VBO_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, x));

    glEnableVertexAttribArray(VBO_NORMAL_INDEX);
    glVertexAttribPointer(VBO_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, nx));

    glEnableVertexAttribArray(VBO_UV_INDEX);
    glVertexAttribPointer(VBO_UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, u));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBufferId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VBO::draw(float deltaTime)
{
    glBindVertexArray(this->vaoId);
    glDrawElements(GL_TRIANGLES, this->size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

