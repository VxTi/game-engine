//
// Created by Luca Warmenhoven on 19/05/2024.
//

#ifndef GRAPHICS_TEST_WORLDOBJECT_H
#define GRAPHICS_TEST_WORLDOBJECT_H


#include "../rendering/Drawable.h"
#include "../rendering/Rendering.h"

#include <glm/glm.hpp>

/**
 * A world object that can be rendered
 */
class WorldObject : public Drawable
{
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    /**
     * Constructor for creating a new world object
     *
     * @param position The position of the world object
     * @param rotation The rotation of the world object
     * @param scale The scale of the world object
     */
    WorldObject(std::vector<VBO> meshes, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    /**
     * Default constructor for creating a new world object
     */
    WorldObject();

    /**
     * Render the world object
     */
    void render() override;
};


#endif //GRAPHICS_TEST_WORLDOBJECT_H
