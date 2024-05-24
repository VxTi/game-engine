//
// Created by Luca Warmenhoven on 19/05/2024.
//

#include "WorldObject.h"

WorldObject::WorldObject(std::vector<VBO> meshes, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Drawable(position, scale, rotation)
{
    this->meshes = meshes;
}

WorldObject::WorldObject(VBO mesh, glm::vec3 position) : Drawable(position, glm::vec3(1), glm::vec3(0))
{
    this->meshes = std::vector<VBO>();
    this->meshes.emplace_back(mesh);
    this->position = position;
    this->scale = glm::vec3(1);
    this->rotation = glm::vec3(0);
}

void WorldObject::draw(float deltaTime)
{
    for (VBO mesh : meshes)
        mesh.draw(0);

}