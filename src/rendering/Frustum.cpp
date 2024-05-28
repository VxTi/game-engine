//
// Created by Luca Warmenhoven on 21/05/2024.
//

#include "Rendering.h"

using namespace glm;

Frustum createFrustum(float zNear, float zFar, float fov, float aspect)
{
    float halfFov = fov / 2.0f;
    float tanFov = tan(halfFov);

    float halfFarHeight = 2.0f * ( zFar * tanFov );
    float halfFarWidth = aspect * halfFarHeight;
    float halfNearHeight = 2.0f * ( zNear * tanFov );
    float halfNearWidth = aspect * halfNearHeight;

    // Create the frustum planes
    Plane farPlane = { .normal = vec3(0, 0, -1) };
    Plane nearPlane = { .normal = vec3(0, 0, 1) };
    float cosHalfFov = cos(halfFov);
    float sinHalfFov = sin(halfFov);

    Plane top = { normalize(vec3(0, -cosHalfFov, sinHalfFov)) };
    Plane bottom = { normalize(vec3(0, cosHalfFov, sinHalfFov)) };
    Plane left = { normalize(vec3(cosHalfFov, 0, sinHalfFov)) };
    Plane right = { normalize(vec3(-cosHalfFov, 0, sinHalfFov)) };


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
    // Check whether the `referencePoint` is within the provided `frustum` object,
    //  seen from the `transformation` object.

    // Calculate the relative position of the reference point
    glm::vec3 relativePosition = referencePosition - transformation->position;

    // Check whether the reference point is within the frustum
    return
            glm::dot(frustum.top.normal, relativePosition) < 0 &&
            glm::dot(frustum.bottom.normal, relativePosition) < 0 &&
            glm::dot(frustum.left.normal, relativePosition) < 0 &&
            glm::dot(frustum.right.normal, relativePosition) < 0 &&
            glm::dot(frustum.near.normal, relativePosition) < 0 &&
            glm::dot(frustum.far.normal, relativePosition) < 0;
}
