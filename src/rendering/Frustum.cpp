//
// Created by Luca Warmenhoven on 21/05/2024.
//

#include "Rendering.h"

using namespace glm;

Frustum createFrustum(float zNear, float zFar, float fov, float aspect)
{
    float halfFov = fov / 2.0f;
    float tanFov = tan(halfFov);

    float halfFarHeight = 2.0f * ( zFar * tanFov);
    float halfFarWidth = aspect * halfFarHeight;
    float halfNearHeight = 2.0f * ( zNear * tanFov);
    float halfNearWidth = aspect * halfNearHeight;

    // Create the frustum planes
    Plane farPlane = { .normal = vec3(0, 0, -1) };
    Plane nearPlane = { .normal = vec3(0, 0, 1) };
    float cosHalfFov = cos(halfFov);
    float sinHalfFov = sin(halfFov);

    Plane top = { normalize(vec3(0, -cosHalfFov, sinHalfFov)) };
    Plane bottom = { normalize(vec3(0, cosHalfFov, sinHalfFov)) };
    Plane left = { normalize(vec3(cosHalfFov, 0, sinHalfFov)) };
    Plane right = { normalize(vec3(-cosHalfFov, 0, sinHalfFov))};


    return (Frustum) {
            .top = top,
            .bottom = bottom,
            .left = left,
            .right = right,
            .near = nearPlane,
            .far = farPlane,
            .zFar = zFar,
            .zNear = zNear,
            .aspect = aspect,
            .fov = fov
    };
}

bool isWithinFrustum(Transformation *transformation, Frustum frustum, glm::vec3 referencePosition)
{
    // Translate the reference point to the camera's position
    /*glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(referencePosition));
    modelMatrix = glm::rotate(modelMatrix, -referenceCamera->rotation.z, glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, -referenceCamera->rotation.y, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, -referenceCamera->rotation.x, glm::vec3(1, 0, 0));

    glm::vec3 position = glm::vec3(modelMatrix * glm::vec4(referenceCamera->position, 1.0));
*/
    // Check whether the point is within the frustum
    return true;/*glm::dot(relativePosition, frustum.top.normal) > 0 &&
           glm::dot(relativePosition, frustum.bottom.normal) > 0 &&
           glm::dot(position, frustum.left.normal) > 0; &&
           glm::dot(relativePosition, frustum.right.normal) > 0 &&
           glm::dot(relativePosition, frustum.near.normal * frustum.zNear) > 0 &&
           glm::dot(relativePosition, frustum.far.normal * frustum.zFar) > 0;*/
}
