//
// Created by Luca Warmenhoven on 20/05/2024.
//
#include "World.h"
#include "chunk/Chunk.h"
#include "SimplexNoise.h"
#include "../rendering/model/Mesh.h"
#include <iostream>
#include <random>


void world_generation_fn(World *world, Transformation &observationPoint)
{
    for ( int i = 0; i < 20; i++ ) {
        for ( int j = 0; j < 20; j++ ) {
            auto *chunk = new Chunk(glm::vec3(i * CHUNK_SIZE, 0, j * CHUNK_SIZE));
            chunk->generate();
            world->drawables.push_back(chunk);
            std::cout << "Generating chunk at " + std::to_string(i * CHUNK_SIZE) + ", " + std::to_string(j * CHUNK_SIZE)
                      << std::endl;
        }
    }
}

void World::generate(Transformation &observationPoint)
{

    // Clearly already running.
    if ( this->world_generation_thread )
        return;

    //world_generation_thread = new std::thread(world_generation_fn, this, observationPoint);

    drawables = std::vector<Drawable *>();
    updatableObjects = std::vector<Updatable *>();

    world_generation_fn(this, observationPoint);
}

float World::getTerrainHeight(float x, float z)
{
    float resultingHeight = 0.0f;
    float bias;

    for ( auto octave: Chunk::CHUNK_GENERATION_OCTAVES ) {
        bias = ((float) random() / (float) INT_MAX ) * CHUNK_GENERATION_RANDOM_BIAS_FACTOR;
        resultingHeight += SimplexNoise::noise((( x + bias ) / octave[ 0 ] ), ( z - bias ) / octave[ 0 ]) * octave[ 1 ];
    }
    resultingHeight = ( resultingHeight ) * CHUNK_GENERATION_MAX_HEIGHT;
    return resultingHeight;
}

void World::render(float deltaTime, Transformation *transformation, Frustum frustum)
{
    for ( Drawable *drawable: drawables ) {

        // TODO: Implement
        /*if ( !isWithinFrustum(transformation, frustum, drawable->position)) {
            continue;
        }*/

        drawable->draw(0);
    }
}

World::~World()
{
    drawables.clear();
    updatableObjects.clear();
    delete this->world_generation_thread;
}