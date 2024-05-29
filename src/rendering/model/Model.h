//
// Created by Luca Warmenhoven on 21/05/2024.
//

#ifndef GRAPHICS_TEST_MODEL_H
#define GRAPHICS_TEST_MODEL_H

#include "glm/vec3.hpp"
#include "../VBO.h"

typedef struct
{
    GLuint textureId;
    vertex_t V1;
    vertex_t V2;
    vertex_t V3;
} Face;

/**
 * Class representing a model composed of faces.
 * This object can be moved, rotated and scaled.
 */
class Model : public Transformation
{
public:
    Face *faces;
    size_t facesCount;
    VBO mesh;

    static Model *loadObj(const char *filePath);

};


#endif //GRAPHICS_TEST_MODEL_H
