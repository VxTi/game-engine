//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include "Rendering.h"

VBO::VBO() {
    glGenBuffers(1, &this->vboBufferId);
    glGenBuffers(1, &this->eboBufferId);
}

/** Supply the VBO with vertices */
void VBO::withVertices(std::vector<Vertex> vertices) {

    // Check if the data is valid
    if ( vertices.empty() )
        throw std::invalid_argument("Invalid data supplied to VBO");

    glBindBuffer(GL_ARRAY_BUFFER, this->vboBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (void*)vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/** Supply the VBO with indices */
void VBO::withIndices(std::vector<int> indices) {
    this->size = indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (void*)indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VBO::render() const
{
    glBindVertexArray(this->vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBufferId);
    glDrawArrays(GL_TRIANGLES, 0, this->size);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

