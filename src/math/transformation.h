//
// Created by Luca Warmenhoven on 31/05/2024.
//

#ifndef GRAPHICS_TEST_TRANSFORMATION_H
#define GRAPHICS_TEST_TRANSFORMATION_H

#include "glm/vec3.hpp"

class Transformation
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    union
    {
        glm::vec3 rotation;
        struct
        {
            float pitch;
            float yaw;
            float roll;
        };
    };
};

#endif //GRAPHICS_TEST_TRANSFORMATION_H
