//
// Created by Luca Warmenhoven on 20/05/2024.
//

#ifndef GRAPHICS_TEST_WORLD_H
#define GRAPHICS_TEST_WORLD_H

#include <thread>
#include <queue>
#include "../rendering/Rendering.h"
#include "entity/Entity.h"
#include "entity/Player.h"

#define CHUNK_SIZE (32)
#define CHUNK_GENERATION_MAX_HEIGHT (50)
#define CHUNK_GENERATION_RANDOM_BIAS_FACTOR (0.1f)
#define CHUNK_BASE_WATER_LEVEL (10.0f)
#define CHUNK_BIOME_COUNT (5)
#define CHUNK_GENERATION_NORMAL_DELTA (0.1f)

typedef struct {
    VBO *mesh;
    float *height_map; // Size is always CHUNK_SIZE^2
    int32_t x;
    int32_t z;
} chunk_t;

typedef struct {
    vbo_data_t *mesh_data;
    chunk_t *chunk;
} immature_chunk_data_t;

class World
{

private:
    /**
     * The thread that will generate the chunks of the world.
     * This thread is responsible for performing all the calculations
     * regarding the chunk generation.
     */
    std::thread *worldGenerationThread;

    /**
     * The queue of chunks that need their meshes to be generated.e
     */
    std::queue<immature_chunk_data_t *> chunkMeshGenerationQueue;

public:
    std::vector<chunk_t *> chunks;
    std::vector<Updatable *> worldObjects;
    std::vector<Drawable *> drawables;

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
     * The biome height scaling factors.
     * These factors are used to scale the height of the terrain based on the biome.
     */
    static constexpr float BIOME_HEIGHT_SCALING_FACTORS[CHUNK_BIOME_COUNT] = { 0.5f, 0.7f, 1.0f, 1.2f, 1.5f };



    /**
     * Destructor
     */
    ~World();

    void startWorldGeneration(Transformation *observationPoint);

    void render(float deltaTime, Transformation *transformation, Frustum frustum);

    void update(float deltaTime);

    /**
     * Function for generating a chunk at a certain position.
     *
     * @param position The location to startWorldGeneration the chunk at.
     * @return
     */
    void generateChunk(int32_t x, int32_t z);

    /**
     * Function for generating the mesh for a chunk.
     *
     * @param chunk The chunk to startWorldGeneration the mesh for.
     */
    void generateChunkMesh(chunk_t *chunk, vbo_data_t *vbo_data);
};


#endif //GRAPHICS_TEST_WORLD_H
