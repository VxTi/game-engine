//
// Created by Luca Warmenhoven on 20/05/2024.
//

#ifndef GRAPHICS_TEST_WORLD_H
#define GRAPHICS_TEST_WORLD_H

#include <thread>
#include <queue>
#include <engine/renderer/renderer.h>
#include <engine/world/entity/player.h>
#include <engine/world/entity/entity.h>
#include <engine/renderer/culling/frustum.h>
#include <engine/renderer/vbo.h>

#define CHUNK_RENDER_DISTANCE (20)
#define CHUNK_DRAW_DISTANCE (15)
#define CHUNK_SIZE (64)
#define CHUNK_BASE_WATER_LEVEL (10.0f)
#define CHUNK_COORDINATE_SCALING_FACTOR (20.0f)
#define CHUNK_COORDINATE_SCALAR (CHUNK_COORDINATE_SCALING_FACTOR * CHUNK_SIZE)

#define CHUNK_GENERATION_MAX_HEIGHT (25)
#define CHUNK_BIOME_COUNT (5)
#define CHUNK_GENERATION_NORMAL_DELTA (0.1f)

typedef struct chunk_t {
    VBO *mesh;
    float *height_map; // Size is always CHUNK_SIZE^2
    int32_t x;
    int32_t z;

    // For checking whether the chunk is the same.
    // This is always the case if the coordinates are the same due
    // to how world generation works.
    bool operator==(const chunk_t &reference) const
    {
        return x == reference.x && z == reference.z;
    }
} chunk_t;

typedef struct {
    vbo_data_t *mesh_data;
    chunk_t *chunk;
} immature_chunk_data_t;

class World
{

private:
    /**
     * The thread that will generate the chunkMap of the world.
     * This thread is responsible for performing all the calculations
     * regarding the chunk generation.
     */
    std::thread *worldGenerationThread;

    /**
     * Mutex for locking the generation of chunkMap to prevent
     * duplicate chunk generation.
     */
    std::mutex worldGenerationMutex;

    /**
     * The queue of chunkMap that need their meshes to be generated.e
     */
    std::queue<immature_chunk_data_t *> *chunkMeshGenerationQueue;

    /**
     * The location where the last chunk generation was performed.
     * This is used to determine when the chunk generation thread should
     * continue generating chunks depending on the distance of the player to this point.
     */
    glm::vec3 lastGenerationPoint;

public:

    static glm::vec3 sunPosition;
    static glm::vec4 sunColor;
    static float sunIntensity;
    static float sunAmbient;
    static float sunSize;

    static float fogDensity;

    static glm::vec4 fogColor;
    static glm::vec3 fogFactors;
    static glm::vec4 skyBottomColor;
    static glm::vec4 skyTopColor;


public:
    std::unordered_map<std::size_t, chunk_t *> *chunkMap;
    std::vector<Entity *> *worldObjects;
    std::vector<Drawable *> *drawables;

    /**
     * Chunk generation octaves.
     * These octaves are a set of two numbers, the first one indicating the coordinate dividing
     * factor, the second one the height factor.
     * CHUNK_GENERATION_OCTAVES[0] - The coordinate scaling factor for which to retrieve the noise for
     * CHUNK_GENERATION_OCTAVES[1] - The height multiplier
     */
    static constexpr float CHUNK_GENERATION_OCTAVES[][2] = {
            { 500, 30 },
            { 100,   1 },
            /*{ 4,   .1 },*/
            /*{ 1,   .005 }*/
    };
    /**
     * The biome height scaling factors.
     * These factors are used to scale the height of the terrain based on the biome.
     */
    static constexpr float BIOME_HEIGHT_SCALING_FACTORS[CHUNK_BIOME_COUNT] = { 0.5f, 0.7f, 1.0f, 1.2f, 1.3f };

    /**
     * Destructor
     */
    ~World();

    void startWorldGeneration(Transform *observationPoint);

    void render(float deltaTime, Frustum *frustum);

    void update(float deltaTime) const;

    /**
     * Function for generating a chunk at a certain position.
     *
     * @param position The location to startWorldGeneration the chunk at.
     * @return
     */
    void generateChunk(int32_t x, int32_t z);

    /**
     * Function for generating the Mesh for a chunk.
     *
     * @param chunk The chunk to startWorldGeneration the Mesh for.
     */
    void generateChunkMesh(chunk_t *chunk, vbo_data_t *vbo_data) const;
};


#endif //GRAPHICS_TEST_WORLD_H
