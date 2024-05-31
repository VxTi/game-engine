//
// Created by Luca Warmenhoven on 21/05/2024.
//

#include "Frustum.h"

void Frustum::updateViewProjectionMatrix(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glm::mat4 VP = projectionMatrix * viewMatrix;
    this->viewProjectionMatrix = VP;

    if ( !this->planes )
        this->planes = (Plane *) malloc(FRUSTUM_PLANES * sizeof(Plane));

    // Extract the planes from the VP matrix
    // Left
    this->planes[ 0 ].normal.x = VP[ 0 ][ 3 ] + VP[ 0 ][ 0 ];
    this->planes[ 0 ].normal.y = VP[ 1 ][ 3 ] + VP[ 1 ][ 0 ];
    this->planes[ 0 ].normal.z = VP[ 2 ][ 3 ] + VP[ 2 ][ 0 ];
    this->planes[ 0 ].distance = VP[ 3 ][ 3 ] + VP[ 3 ][ 0 ];

    // Right
    this->planes[ 1 ].normal.x = VP[ 0 ][ 3 ] - VP[ 0 ][ 0 ];
    this->planes[ 1 ].normal.y = VP[ 1 ][ 3 ] - VP[ 1 ][ 0 ];
    this->planes[ 1 ].normal.z = VP[ 2 ][ 3 ] - VP[ 2 ][ 0 ];
    this->planes[ 1 ].distance = VP[ 3 ][ 3 ] - VP[ 3 ][ 0 ];

    // Bottom
    this->planes[ 2 ].normal.x = VP[ 0 ][ 3 ] + VP[ 0 ][ 1 ];
    this->planes[ 2 ].normal.y = VP[ 1 ][ 3 ] + VP[ 1 ][ 1 ];
    this->planes[ 2 ].normal.z = VP[ 2 ][ 3 ] + VP[ 2 ][ 1 ];
    this->planes[ 2 ].distance = VP[ 3 ][ 3 ] + VP[ 3 ][ 1 ];

    // Top
    this->planes[ 3 ].normal.x = VP[ 0 ][ 3 ] - VP[ 0 ][ 1 ];
    this->planes[ 3 ].normal.y = VP[ 1 ][ 3 ] - VP[ 1 ][ 1 ];
    this->planes[ 3 ].normal.z = VP[ 2 ][ 3 ] - VP[ 2 ][ 1 ];
    this->planes[ 3 ].distance = VP[ 3 ][ 3 ] - VP[ 3 ][ 1 ];

    // Near
    this->planes[ 4 ].normal.x = VP[ 0 ][ 3 ] + VP[ 0 ][ 2 ];
    this->planes[ 4 ].normal.y = VP[ 1 ][ 3 ] + VP[ 1 ][ 2 ];
    this->planes[ 4 ].normal.z = VP[ 2 ][ 3 ] + VP[ 2 ][ 2 ];
    this->planes[ 4 ].distance = VP[ 3 ][ 3 ] + VP[ 3 ][ 2 ];

    // Far
    this->planes[ 5 ].normal.x = VP[ 0 ][ 3 ] - VP[ 0 ][ 2 ];
    this->planes[ 5 ].normal.y = VP[ 1 ][ 3 ] - VP[ 1 ][ 2 ];
    this->planes[ 5 ].normal.z = VP[ 2 ][ 3 ] - VP[ 2 ][ 2 ];
    this->planes[ 5 ].distance = VP[ 3 ][ 3 ] - VP[ 3 ][ 2 ];

    for ( int i = 0; i < FRUSTUM_PLANES; i++ ) {
        Plane &plane = this->planes[ i ];
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }
}

Frustum::Frustum(Transformation *source, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    this->source = source;
    this->planes = (Plane *) malloc(FRUSTUM_PLANES * sizeof(Plane));
    this->updateViewProjectionMatrix(viewMatrix, projectionMatrix);
}

// Calculate the distance to a normal of a plane
float Frustum::distanceToNormal(glm::vec3 normal, glm::vec3 point)
{
    return glm::dot(normal, point);
}

// Calculate the distance to a plane
float Frustum::distanceToPlane(Plane *plane, glm::vec3 point)
{
    return glm::dot(plane->normal, point) + plane->distance;
}

bool Frustum::isWithin(glm::vec3 referencePosition)
{
    return true; // IMPLEMENT !!
}

bool Frustum::isWithin(glm::vec3 referencePoint, float radius)
{
    // Step 1: Transform provided point to relative space
    // Transform the point to be within the frustum plane
    glm::vec3 relativePosition = referencePoint - this->source->position;

    // Calculate rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-this->source->yaw), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-this->source->pitch), glm::vec3(1, 0, 0));

    for ( int i = 0; i < FRUSTUM_PLANES; i++ ) {
        float distance = glm::dot(
                glm::vec3(rotationMatrix * glm::vec4(planes[ i ].normal, 1.0f)), relativePosition)
                         + planes[ i ].distance;
        if ( distance < -radius ) {
            return false;
        }
    }
    return true;
}

Frustum::~Frustum()
{
    free(this->planes);
}