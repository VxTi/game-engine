//
// Created by Luca Warmenhoven on 19/05/2024.
//

#include "WorldObject.h"

WorldObject::WorldObject(std::vector<VBO> meshes, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->meshes = meshes;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

void WorldObject::render()
{
    for (VBO mesh : meshes)
        mesh.render();

}