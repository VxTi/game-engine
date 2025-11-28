//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_MESH_H
#define GRAPHICS_TEST_MESH_H


#include "../renderer.h"
#include "../vbo.h"

/**
 * Represents a Mesh that can be drawn.
 */
class Mesh
{
private:
    VBO *buffer;

public:
    /**
     * Constructor for creating a new Mesh object.
     * @param buffer
     * @param position
     * @param rotation
     * @param scale
     */
    Mesh(VBO *buffer, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    /*
     * Destructor for cleaning up the Mesh object.
     */
    ~Mesh();

    /**
     * Draws the Mesh.
     * @param deltaTime The time passed since the last frame in seconds.
     */
    void draw(float deltaTime);

};


#endif //GRAPHICS_TEST_MESH_H
