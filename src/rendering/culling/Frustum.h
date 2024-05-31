//
// Created by Luca Warmenhoven on 31/05/2024.
//

#ifndef GRAPHICS_TEST_FRUSTUM_H
#define GRAPHICS_TEST_FRUSTUM_H

#include "../Transformation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define FRUSTUM_PLANES (6)

typedef struct
{
    float distance;
    glm::vec3 normal;
} Plane;

class Frustum {

private:
    Plane *planes;

    glm::mat4 viewProjectionMatrix;

    float distanceToNormal(glm::vec3 normal, glm::vec3 point);

    float distanceToPlane(Plane *plane, glm::vec3 point);


public:

    Transformation *source;

    /**
     * Constructor for creating a new Frustum
     * This constructor will generate the frustum based on the provided parameters,
     * which can then be used to check whether objects are visible from the source.
     * @param source The source of the frustum
     * @param viewMatrix The view matrix of the frustum
     * @param projectionMatrix The projection matrix of the frustum
     */
    Frustum(Transformation *source, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);


    /**
     * Function for updating the view and projection matrix of the frustum.
     * This function will update the view and projection matrix of the frustum.
     * @param viewMatrix The view matrix of the frustum
     * @param projectionMatrix The projection matrix of the frustum
     */
    void updateViewProjectionMatrix(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    /**
     * Function that checks whether the object is within the provided frustum.
     * @param referencePosition The position of the object to check
     * @return Whether the object is within the frustum
     */
    bool isWithin(glm::vec3 referencePosition);

    /**
     * Function that checks whether the object is within the provided frustum and
     * whether it is within the radius of the object.
     * @param referencePosition The position of the object to check
     * @param radius The radius of the object to check
     * @return Whether the object is within the frustum
     */
    bool isWithin(glm::vec3 referencePosition, float radius);

    ~Frustum();
};


#endif //GRAPHICS_TEST_FRUSTUM_H
