//
// Created by Luca Warmenhoven on 20/05/2024.
//

#ifndef GRAPHICS_TEST_WORLD_H
#define GRAPHICS_TEST_WORLD_H

#include <thread>
#include "../rendering/Rendering.h"
#include "entity/Entity.h"
#include "entity/Player.h"

#define CHUNK_SIZE (32)
#define CHUNK_GENERATION_MAX_HEIGHT (8)
#define CHUNK_GENERATION_RANDOM_BIAS_FACTOR (0.1f)
#define CHUNK_BASE_WATER_LEVEL (10.0f)

class World
{

private:
    std::thread *world_generation_thread;

public:
    std::vector<Updatable *> updatableObjects;
    std::vector<Drawable *> drawables;

    /**
     * Destructor
     */
    ~World();

    void generate(Transformation &observationPoint);

    void render(float deltaTime, Transformation *transformation, Frustum frustum);

    static float getTerrainHeight(float x, float z);

};


#endif //GRAPHICS_TEST_WORLD_H
