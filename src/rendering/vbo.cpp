//
// Created by Luca Warmenhoven on 15/05/2024.
//

#include <engine/renderer/vbo.h>
#include <engine/util/result.h>
#include <iostream>

VBO::VBO() {
  glGenBuffers(1, &this->vertexBufferId);
  glGenBuffers(1, &this->indexBufferId);
}

VBO::~VBO() {
  glDeleteBuffers(1, &this->vertexBufferId);
  glDeleteBuffers(1, &this->indexBufferId);
  glDeleteVertexArrays(1, &this->vaoId);
}

Result VBO::addVertices(vertex_t *vertices, unsigned long vertexCount) {
  if (vertexCount == 0) {
    return Result::ERROR;
  }
  glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vertex_t), vertices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return Result::OK;
}

/** Supply the VBO with vertices */
Result VBO::addVertices(std::vector<vertex_t> vertices) {
  return this->addVertices(vertices.data(), vertices.size());
}

Result VBO::addIndices(unsigned int *indices, unsigned long indicesCount) {
  if (indicesCount == 0) {
    return Result::ERROR;
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  this->size = (GLsizei)indicesCount;

  return Result::OK;
}

/** Supply the VBO with indices */
Result VBO::addIndices(std::vector<unsigned int> indices) {
  return this->addIndices(indices.data(), indices.size());
}

/**
 * Build the VBO using VAOs
 */
Result VBO::build() {
  if (this->vaoId) {
    return Result::OK; // Prevents double building
  }
  glGenVertexArrays(1, &this->vaoId);
  glBindVertexArray(this->vaoId);

  glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);

  glEnableVertexAttribArray(VBO_POSITION_INDEX);
  glVertexAttribPointer(VBO_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex_t), (GLvoid *)offsetof(vertex_t, x));

  glEnableVertexAttribArray(VBO_NORMAL_INDEX);
  glVertexAttribPointer(VBO_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex_t), (GLvoid *)offsetof(vertex_t, nx));

  glEnableVertexAttribArray(VBO_UV_INDEX);
  glVertexAttribPointer(VBO_UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                        (GLvoid *)offsetof(vertex_t, u));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);

  glBindVertexArray(0);
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cerr << "OpenGL error: " << err << std::endl;
    return Result::ERROR;
  }
  return Result::OK;
}

void VBO::draw(float deltaTime) {
  glBindVertexArray(this->vaoId);
  glDrawElements(GL_TRIANGLES, this->size, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
