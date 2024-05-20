//
// Created by Luca Warmenhoven on 20/05/2024.
//
#include "World.h"
#include <iostream>

void World::generate()
{

    chunks = std::vector<Chunk *>();
    entities = std::vector<Entity *>();

    for ( int i = 0; i < 20; i++ ) {
        for ( int j = 0; j < 20; j++ ) {
            auto *chunk = new Chunk(glm::vec3(i * CHUNK_SIZE, 0, j * CHUNK_SIZE));
            chunk->generateMesh();
            chunks.push_back(chunk);
            std::cout << "Generating chunk at " + std::to_string(i * CHUNK_SIZE) + ", " + std::to_string(j * CHUNK_SIZE)
                      << std::endl;
        }
    }


}

void World::render(float deltaTime)
{
    for ( Chunk *chunk: chunks ) {
        chunk->render();
    }
}

World::~World()
{
    chunks.clear();
    entities.clear();
}