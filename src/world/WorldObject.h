//
// Created by Luca Warmenhoven on 19/05/2024.
//

#ifndef GRAPHICS_TEST_WORLDOBJECT_H
#define GRAPHICS_TEST_WORLDOBJECT_H


#include "../rendering/Rendering.h"
#include "../rendering/VBO.h"

#include <glm/glm.hpp>

// Bounding box face structure
typedef struct
{
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
    glm::vec3 normal;
} BoundingBoxFace;

// Bounding box structure
typedef struct
{
    BoundingBoxFace *faces;
    unsigned int facesCount;
    float maxRadius;
    glm::vec3 position;
} BoundingBox;

/**
 * A world object that can be rendered
 */
class WorldObject : public Drawable
{
private:

    /** The bounding box of the world object */
    BoundingBox boundingBox;

public:
    std::vector<VBO> meshes;

    /**
     * Constructor for creating a new world object
     *
     * @param position The position of the world object
     * @param rotation The rotation of the world object
     * @param scale The scale of the world object
     */
    WorldObject(std::vector<VBO> meshes, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    /**
     * Constructor for creating a new world object
     *
     * @param mesh The mesh of the world object
     * @param position The position of the world object
     */
    WorldObject(VBO mesh, glm::vec3 position);

    /**
     * Constructor for creating a new world object with a bounding box
     *
     * @param mesh The mesh of the world object
     * @param position The position of the world object
     * @param boundingBox The bounding box of the world object
     */
    WorldObject(VBO mesh, glm::vec3 position, BoundingBox boundingBox);

    /**
     * Check if the world object collides with another bounding box
     *
     * @param other The other bounding box
     * @return Whether the world object collides with the other bounding box
     */
    bool collidesWith(BoundingBox other);

    /**
     * Check if the world object collides with a point
     *
     * @param point The point to check for collision
     * @return  Whether the world object collides with the point
     */
    bool collidesWith(glm::vec3 point);

    /**
     * Check if the world object collides with a sphere
     *
     * @param point The point to check for collision
     * @param radius The radius of the point
     * @return  Whether the world object collides with the sphere
     */
    bool collidesWith(glm::vec3 point, float radius);

    /**
     * Check if the world object collides with another world object
     *
     * @param other The other world object
     * @return Whether the world object collides with the other world object
     */
    bool collidesWith(WorldObject other);

    /**
     * Render the world object
     */
    void draw(float deltaTime) override;
};


#endif //GRAPHICS_TEST_WORLDOBJECT_H
