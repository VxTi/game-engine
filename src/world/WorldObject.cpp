//
// Created by Luca Warmenhoven on 19/05/2024.
//

#include "WorldObject.h"

WorldObject::WorldObject(std::vector<VBO> meshes, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Drawable(
        position, scale, rotation)
{
    this->meshes = meshes;
}

WorldObject::WorldObject(VBO mesh, glm::vec3 position) : Drawable(position, glm::vec3(1), glm::vec3(0))
{
    this->meshes = std::vector<VBO>();
    this->meshes.emplace_back(mesh);
}

WorldObject::WorldObject(VBO mesh, glm::vec3 position, BoundingBox boundingBox) : Drawable(position, glm::vec3(1),
                                                                                           glm::vec3(0))
{
    this->meshes = std::vector<VBO>();
    this->meshes.emplace_back(mesh);
    this->boundingBox = boundingBox;
}

void WorldObject::draw(float deltaTime)
{
    for ( VBO mesh: meshes )
        mesh.draw(0);
}

bool isAbovePlane(BoundingBoxFace face, glm::vec3 point)
{
    return glm::dot(face.normal, point - (face.v1 + face.v2 + face.v3) / 3.0f) > 0;
}

float distSq(vec3 first, vec3 second)
{
    return ( first.x - second.x ) * ( first.x - second.x ) +
           ( first.y - second.y ) * ( first.y - second.y ) +
           ( first.z - second.z ) * ( first.z - second.z );
}

bool isWithinRadius(BoundingBox boundingBox, glm::vec3 point)
{
    return distSq(boundingBox.position, point) < boundingBox.maxRadius * boundingBox.maxRadius;
}

bool WorldObject::collidesWith(glm::vec3 point)
{
    // Check if D(P, this)^2 > R^2
    if (!isWithinRadius(this->boundingBox, point))
        return false;

    for ( int i = 0; i < this->boundingBox.facesCount; i++ )
        if ( isAbovePlane(this->boundingBox.faces[ i ], point))
            return false;

    return true;
}

bool WorldObject::collidesWith(BoundingBox other)
{
    // Check if D(other, this)^2 > (R1 + R2)^2
    if (distSq(this->boundingBox.position, other.position) > ( this->boundingBox.maxRadius + other.maxRadius ) *
                                                              ( this->boundingBox.maxRadius + other.maxRadius ))
        return false;

    return false;

}