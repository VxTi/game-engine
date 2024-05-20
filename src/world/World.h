//
// Created by Luca Warmenhoven on 20/05/2024.
//

#ifndef GRAPHICS_TEST_WORLD_H
#define GRAPHICS_TEST_WORLD_H

#include "../rendering/Renderer.h"
#include "entity/Entity.h"

#define CHUNK_SIZE 16

class Chunk
{
private:
    VBO *meshVbo;
    union {
        glm::vec3 position;
        struct {
            float x;
            float y;
            float z;
        };
    };


public:

    /**
     * Constructor for creating a new chunk
     *
     * @param position The position of the chunk
     */
    Chunk(glm::vec3 position);

    // Destructor
    ~Chunk();

    /**
     * Render the chunk
     */
    void render();

    /**
     * Generate the mesh for the chunk
     */
    void generateMesh();
};

class World {

private:
    std::vector<Chunk*> chunks;
    std::vector<Entity*> entities;

public:

    /**
     * Destructor
     */
    ~World();

    void generate();

    void render(float deltaTime);

};


#endif //GRAPHICS_TEST_WORLD_H
