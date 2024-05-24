//
// Created by Luca Warmenhoven on 20/05/2024.
//
#include "World.h"
#include "chunk/Chunk.h"
#include "SimplexNoise.h"
#include <iostream>
#include <random>


void world_generation_fn(World *world, Transformation &observationPoint)
{

    /*VBO sphere = VBO();
    std::vector<Vertex> points = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    int sphere_segments = 20;
    int index = 0;
    float radius = 2.0f;
    for ( float phi_vertical = 0; phi_vertical < 2 * M_PI; phi_vertical += M_PI / sphere_segments ) {
        for ( float phi_horizontal = 0; phi_horizontal < 2 * M_PI - abs(M_PI - phi_vertical); phi_horizontal += M_PI / sphere_segments ) {
            points.push_back({
                                     cos(phi_horizontal) * sin(phi_vertical) * radius,
                                     sin(phi_vertical) * radius,
                                     sin(phi_horizontal) * sin(phi_vertical) * radius
                             }
            );
            indices.push_back(index++);
        }
    }
    sphere.withIndices(indices);
    sphere.withVertices(points);
    sphere.build();

    world->drawables.push_back(&sphere);*/


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
    if (this->world_generation_thread)
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

    for (auto octave : Chunk::CHUNK_GENERATION_OCTAVES) {
        bias = ((float)random() / (float)INT_MAX) * CHUNK_GENERATION_RANDOM_BIAS_FACTOR;
        resultingHeight += SimplexNoise::noise((( x + bias) / octave[0]), (z - bias) / octave[0]) * octave[1];
    }
    resultingHeight = (resultingHeight) * CHUNK_GENERATION_MAX_HEIGHT;
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