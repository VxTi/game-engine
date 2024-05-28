//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_CHUNK_H
#define GRAPHICS_TEST_CHUNK_H

#include <glm/glm.hpp>
#include <vector>
#include "../../rendering/Rendering.h"
#include "../../rendering/VBO.h"

class Chunk : public Drawable
{
private:


public:
    VBO *meshVbo;

    union
    {
        glm::vec3 position;
        struct
        {
            float x;
            float y;
            float z;
        };
    };

    /**
     * Chunk generation octaves.
     * These octaves are a set of two numbers, the first one indicating the coordinate dividing
     * factor, the second one the height factor.
     * CHUNK_GENERATION_OCTAVES[0] - The coordinate scaling factor for which to retrieve the noise for
     * CHUNK_GENERATION_OCTAVES[1] - The height multiplier
     */
    static constexpr float CHUNK_GENERATION_OCTAVES[][2] = {
            { 200, 30 },
            { 4,   1 },
            { 4,   .1 },
            { 1,   .005 }
    };

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
    void draw(float deltaTime) override;

    /**
     * Generate the mesh for the chunk
     */
    void generate();
};

#endif //GRAPHICS_TEST_CHUNK_H
