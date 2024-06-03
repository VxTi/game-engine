//
// Created by Luca Warmenhoven on 24/05/2024.
//

#include "mesh.h"

#include <iostream>

Mesh::Mesh(VBO *buffer, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->buffer = buffer;
}

void Mesh::draw(float deltaTime)
{
    this->buffer->draw(deltaTime);
}

Mesh::~Mesh()
{
    delete this->buffer;
}