//
// Created by Luca Warmenhoven on 20/05/2024.
//

#include "../World.h"
#include "../SimplexNoise.h"

Chunk::Chunk(glm::vec3 position)
{
    this->position = position;
    this->meshVbo = new VBO();
}


Chunk::~Chunk()
{
    delete this->meshVbo;
}

float getNoiseHeight(float x, float z)
{
    x /= 10.f;
    z /= 10.f;
    return (SimplexNoise::noise((float) x / 10.f, (float) z / 10.f ) * 10.f
           + SimplexNoise::noise((float) x * 2.5f, (float) z * 2.5f) / 4.f
           + SimplexNoise::noise((float) x / 2.f, (float) z / 2.f) / 2.f) * 10.f;
}

void Chunk::generateMesh()
{

    std::vector<Vertex> worldVertices;
    std::vector<unsigned int> worldIndices;
    int x0, z0;
    float px, pz;
    for ( int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++ ) {
        x0 = ( i % CHUNK_SIZE );
        z0 = ( i / CHUNK_SIZE );

        px = this->position.x + (float)x0;
        pz = this->position.z + (float)z0;

        worldVertices.push_back({
                        ((float) px * 5 ),
                        getNoiseHeight(px, pz),
                        ((float) pz * 5 )
        });
        if ( x0 == CHUNK_SIZE - 1 || z0 == CHUNK_SIZE - 1 ) {
            continue;
        }
        worldIndices.push_back(i);
        worldIndices.push_back(i + 1);
        worldIndices.push_back(i + CHUNK_SIZE);

        worldIndices.push_back(i + 1);
        worldIndices.push_back(i + CHUNK_SIZE);
        worldIndices.push_back(i + CHUNK_SIZE + 1);
    }

    this->meshVbo->withVertices(worldVertices);
    this->meshVbo->withIndices(worldIndices);
    this->meshVbo->build();
}

void Chunk::render()
{
    this->meshVbo->render();
}