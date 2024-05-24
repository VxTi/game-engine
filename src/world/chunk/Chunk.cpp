//
// Created by Luca Warmenhoven on 20/05/2024.
//

#include "Chunk.h"

#include "../SimplexNoise.h"
#include "../World.h"

using namespace glm;

Chunk::Chunk(vec3 position) : Drawable(position, glm::vec3(1), glm::vec3(0))
{
    this->position = position;
    this->meshVbo = new VBO();
}


Chunk::~Chunk()
{
    delete this->meshVbo;
}

void Chunk::generate()
{

    std::vector<Vertex> worldVertices;
    std::vector<unsigned int> worldIndices;
    int x0, z0;
    const int size = CHUNK_SIZE + 1;
    float px, py, pz, py_nx, py_nz;
    for ( int i = 0; i < size * size; i++ ) {
        x0 = ( i % size );
        z0 = ( i / size );

        px = this->position.x + (float)x0;
        pz = this->position.z + (float)z0;

        py = World::getTerrainHeight(px, pz);

        py_nx = World::getTerrainHeight(px + 1, pz);
        py_nz = World::getTerrainHeight(px, pz + 1);

        /**
         * Calculate the normal vector for the two faces.
         * Here u and v represent the vector from one side of the face.
         */
        vec3 u1 = vec3(px, py, pz) - vec3(px + 1, py_nx, pz);
        vec3 v1 = vec3(px, py, pz) - vec3(px, py_nz, pz + 1);
        vec3 normal = normalize(cross(u1, v1));
        worldVertices.push_back({
                        .x = ((float) px*4 ),
                        .y = py,
                        .z =((float) pz*4 ),
                        .nx = normal.x,
                        .ny = normal.y,
                        .nz = normal.z,
        });
        if ( x0 == size - 1 || z0 == size - 1 ) {
            continue;
        }
        worldIndices.push_back(i);
        worldIndices.push_back(i + 1);
        worldIndices.push_back(i + size);

        worldIndices.push_back(i + 1);
        worldIndices.push_back(i + size);
        worldIndices.push_back(i + size + 1);
    }

    this->meshVbo->withVertices(worldVertices);
    this->meshVbo->withIndices(worldIndices);
    this->meshVbo->build();
}

void Chunk::draw(float deltaTime)
{
    this->meshVbo->draw(0);
}