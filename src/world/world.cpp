//
// Created by Luca Warmenhoven on 20/05/2024.
//
#include <engine/world/noise.h>
#include <engine/world/world.h>
#include <iostream>

/** Global variables */
glm::vec3 World::sunPosition = glm::normalize(glm::vec3(0.0f, 1.0f, 2.0f));
glm::vec4 World::sunColor = glm::vec4(1.0f, 1.0f, .8f, 1.0f);

float World::sunIntensity = 1.0f;
float World::sunSize = 0.14f;
float World::sunAmbient = 0.1f;

float World::fogDensity = 0.0005f;

glm::vec4 World::fogColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
glm::vec3 World::fogFactors = glm::vec3(0.1f, 0.5f, 0.5f);

glm::vec4 World::skyBottomColor = glm::vec4(1);
glm::vec4 World::skyTopColor = glm::vec4(.23, .64, .97, 1.0);

void worldGenerationFn(World *world, Transform *observationPoint,
                       glm::vec3 *lastObservationPoint) {

  if (observationPoint == nullptr) {
    std::cerr << "No observation point provided, aborting world generation."
              << std::endl;
    return;
  }

  // Generate chunkMap around the observation point in a circular manner
  int32_t x, z, px, pz;
  // Wait until the thread is stopped
  std::chrono::nanoseconds interval(10);

  while (true) {
    if (pow(lastObservationPoint->x - observationPoint->position.x, 2) +
            pow(lastObservationPoint->z - observationPoint->position.z, 2) >=
        pow(CHUNK_COORDINATE_SCALAR, 2)) {
      *lastObservationPoint = observationPoint->position;
      /*if ( world->chunkMap->size() > 5000 )
          return;*/
      px = (((int32_t)observationPoint->position.x) /
            (int)(CHUNK_COORDINATE_SCALAR)) *
           CHUNK_SIZE;
      pz = (((int32_t)observationPoint->position.z) /
            (int)(CHUNK_COORDINATE_SCALAR)) *
           CHUNK_SIZE;

      for (x = -CHUNK_DRAW_DISTANCE; x < CHUNK_DRAW_DISTANCE; x++) {
        for (z = -CHUNK_DRAW_DISTANCE; z < CHUNK_DRAW_DISTANCE; z++) {
          std::this_thread::sleep_for(interval);
          world->generateChunk(px + x * CHUNK_SIZE, pz + z * CHUNK_SIZE);
        }
      }
    }
  }
}

/*
 * Start the world generation thread.
 * This method is called from the main thread and will startWorldGeneration
 * the world generation thread.
 */
void World::startWorldGeneration(Transform *observationPoint) {

  // If it's already started then just ... just don't.
  if (this->worldGenerationThread)
    return;

  World::lastGenerationPoint = observationPoint->position + vec3(10000, 0, 0);

  chunkMeshGenerationQueue = new std::queue<immature_chunk_data_t *>();
  worldGenerationThread = new std::thread(
      worldGenerationFn, this, observationPoint, &lastGenerationPoint);
}

inline bool shouldRenderChunk(chunk_t &chunk, Frustum *frustum) {
  // return frustum->isWithin(vec3(chunk.x + offset, 0, chunk.z + offset),
  // offset * 2);

  return frustum->isWithin(vec3(chunk.x, 0, chunk.z),
                           CHUNK_SIZE * CHUNK_COORDINATE_SCALING_FACTOR * 2);
}

/**
 * Render the world.
 * This function will render all the chunkMap that are within the frustum.
 */
void World::render(float deltaTime, Frustum *frustum) {
  int chunksRendered = 0;
  for (auto chunkPair : chunkMap) {
    if (!shouldRenderChunk(*chunkPair.second, frustum))
      continue;
    chunkPair.second->mesh->draw(deltaTime);
    chunksRendered++;
  }
  for (Drawable *drawable : drawables) {
    drawable->draw(0);
  }

  // If there's chunkMap that need their meshes to be generated, then do so.
  if (!chunkMeshGenerationQueue->empty()) {
    immature_chunk_data_t *chunk_mesh_data = chunkMeshGenerationQueue->front();
    generateChunkMesh(chunk_mesh_data->chunk, chunk_mesh_data->mesh_data);
    chunkMeshGenerationQueue->pop();
  }
}

/**
 * Simple hash function to get a (semi) unique hash for a chunk.
 */
size_t chunk_hash(int32_t x, int32_t z) {
  return ((x << 16) | (z & 0xFFFF)) ^ 0x9e3779b9;
}

/**
 * Get the biome noise height for a specific coordinate.
 * This can be used to scale certain biomes.
 */
static float getBiomeNoise(float x, float z) {
  return (SimplexNoise::noise((float)x / 100.0f, (float)z / 100.0f) + 1) / 2;
}

/**
 * Get the height of the chunk at a certain coordinate.
 */
static float getChunkHeight(float x, float z) {
  x /= 10.0f;
  z /= 10.0f;
  float biome_noise = getBiomeNoise(x, z);
  float biome_height_factor = World::BIOME_HEIGHT_SCALING_FACTORS[(
      int)(biome_noise * CHUNK_BIOME_COUNT)];
  biome_height_factor *= biome_noise;
  float resultingHeight = 0.0f;

  for (auto octave : World::CHUNK_GENERATION_OCTAVES) {
    resultingHeight +=
        SimplexNoise::noise(x / octave[0], z / octave[0]) * octave[1];
  }
  resultingHeight = (resultingHeight)*CHUNK_GENERATION_MAX_HEIGHT;
  return biome_height_factor * resultingHeight;
}

/**
 * Get the normal vector at a specific location.
 * This calculates the normal vector based on the adjacent
 * height points, which are calculated by the `getChunkHeight` function.
 */
static vec3 getNormalVector(float x, float z) {
  // Calculate the normal vectors
  float height1 = getChunkHeight(x - CHUNK_GENERATION_NORMAL_DELTA, z);
  float height2 = getChunkHeight(x + CHUNK_GENERATION_NORMAL_DELTA, z);
  float height3 = getChunkHeight(x, z - CHUNK_GENERATION_NORMAL_DELTA);
  float height4 = getChunkHeight(x, z + CHUNK_GENERATION_NORMAL_DELTA);

  return normalize(vec3(height1 - height2, 2.0f * CHUNK_GENERATION_NORMAL_DELTA,
                        height3 - height4));
}

/*
 * Generate the Mesh for a chunk.
 * This function will startWorldGeneration a Mesh with the provided vertices and
 * indices.
 */
void World::generateChunkMesh(chunk_t *chunk, vbo_data_t *vbo_data) const {
  VBO *mesh = new VBO();
  mesh->addVertices(vbo_data->vertices, vbo_data->vertices_count);
  mesh->addIndices(vbo_data->indices, vbo_data->indices_count);
  mesh->build();
  chunk->mesh = mesh;
  // Add chunk to world
  chunkMap.insert({chunk_hash(chunk->x, chunk->z), chunk});
  // Free old memory, it's been copied video memory.
  free(vbo_data->indices);
  free(vbo_data->vertices);
  free(vbo_data);
}

void World::update(float deltaTime) const {
  for (Entity *entities : worldObjects) {
    entities->update(deltaTime);
  }
}

/*
 * Generate a chunk at the given coordinates.
 * This function will startWorldGeneration both the height-map coordinates for
 * the chunk, and the Mesh mesh_data. This Mesh mesh_data can then be fed into
 * the GPU for rendering.
 */
void World::generateChunk(int32_t x, int32_t z) {
  worldGenerationMutex.lock();

  // Check if the chunk already exists
  if (chunkMap.find(chunk_hash(x, z)) != chunkMap.end()) {
    worldGenerationMutex.unlock();
    return;
  }

  worldGenerationMutex.unlock();
  auto *data_points = (float *)malloc(sizeof(float) * CHUNK_SIZE * CHUNK_SIZE);

  int32_t chunk_x, chunk_z, i, j;

  float cx, cy, cz;

  // Memory for Mesh
  int mesh_width = CHUNK_SIZE + 1;
  const int indices_count = CHUNK_SIZE * CHUNK_SIZE * 6;

  int top_left, bottom_left, top_right, bottom_right;

  int indices_index = 0;
  int vertices_index = 0;

  // Vector chunk_mesh_data that will be passed to the main thread.
  auto *indices = (unsigned int *)malloc(sizeof(unsigned int) * indices_count);
  auto *vertices =
      (vertex_t *)malloc(sizeof(vertex_t) * mesh_width * mesh_width);

  glm::vec3 normal;

  float delta = 0.5f;

  // Generate height chunk_mesh_data points
  for (i = 0; i <= CHUNK_SIZE; i++) {
    for (j = 0; j <= CHUNK_SIZE; j++) {
      // Calculate height for chunk at specific coordinate
      chunk_x = x + i;
      chunk_z = z + j;
      cx = (((float)chunk_x) - delta) * CHUNK_COORDINATE_SCALING_FACTOR;
      cz = (((float)chunk_z) - delta) * CHUNK_COORDINATE_SCALING_FACTOR;
      cy = getChunkHeight(cx, cz);

      normal = getNormalVector(cx, cz);
      // Add regular vertex
      vertices[vertices_index++] = {cx, cy, cz, normal.x, normal.y, normal.z};

      // Generate indices for the Mesh
      if (i < CHUNK_SIZE && j < CHUNK_SIZE) {

        data_points[i * CHUNK_SIZE + j] = cy;

        top_left = i * mesh_width + j;
        bottom_left = (i + 1) * mesh_width + j;
        top_right = i * mesh_width + (j + 1);
        bottom_right = (i + 1) * mesh_width + (j + 1);

        indices[indices_index++] = top_left;
        indices[indices_index++] = top_right;
        indices[indices_index++] = bottom_left;

        indices[indices_index++] = top_right;
        indices[indices_index++] = bottom_right;
        indices[indices_index++] = bottom_left;
      }
    }
  }

  // Create chunk object
  auto *generated = (chunk_t *)malloc(sizeof(chunk_t));
  generated->height_map = data_points;
  generated->x = x * CHUNK_COORDINATE_SCALING_FACTOR;
  generated->z = z * CHUNK_COORDINATE_SCALING_FACTOR;

  // Store chunk_mesh_data in destination. The memory can be freed
  // after the Mesh has been generated.
  auto *chunk_mesh_data =
      (immature_chunk_data_t *)malloc(sizeof(immature_chunk_data_t));
  chunk_mesh_data->mesh_data = (vbo_data_t *)malloc(sizeof(vbo_data_t));
  chunk_mesh_data->mesh_data->indices = indices;
  chunk_mesh_data->mesh_data->vertices = vertices;
  chunk_mesh_data->mesh_data->indices_count = indices_count;
  chunk_mesh_data->mesh_data->vertices_count = mesh_width * mesh_width;
  chunk_mesh_data->chunk = generated;

  // Add to Mesh generation queue
  chunkMeshGenerationQueue->push(chunk_mesh_data);
}

World::World(Entity observer)
    : observer(std::move(observer)), drawables(std::vector<Drawable *>()),
      worldObjects(std::vector<Entity *>()) {
  this->worldObjects.push_back(&observer);
  this->drawables = std::vector<Drawable *>();
  this->chunkMap = std::unordered_map<std::size_t, chunk_t *>();
  this->worldGenerationThread = nullptr;
}

World::~World() {
  // Delete content of queue
  immature_chunk_data_t *data;

  // Clear all memory from the queue
  while (!chunkMeshGenerationQueue->empty()) {
    data = chunkMeshGenerationQueue->front();
    free(data->mesh_data->indices);
    free(data->mesh_data->vertices);
    free(data->mesh_data);
    free(data->chunk);
    chunkMeshGenerationQueue->pop();
  }

  // Clear the chunk map
  for (auto entry : chunkMap) {
    free(entry.second->height_map);
    delete entry.second->mesh;
    free(entry.second);
  }

  for (auto drawable : this->drawables)
    free(drawable);
  drawables.clear();

  worldObjects.clear();
  chunkMap.clear();
  if (worldGenerationThread)
    worldGenerationThread->detach();

  delete worldGenerationThread;
}
