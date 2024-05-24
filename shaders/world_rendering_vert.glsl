#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 ioFragPos;  // The position of the fragment in world space
out vec3 ioNormal;   // The normal vector of the fragment
out vec3 ioPosition; // The position of the fragment in object space

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

uniform mat4 u_ModelViewProjectionMatrix;

uniform float u_time;

float waveFunction(float x, float z, float t)
{
    return sin(x + t * 4 - z);
}

void main()
{
    vec3 resultingPosition = position;
    ioNormal = normal;
    ioFragPos = vec3(u_ModelMatrix * vec4(position, 1.0));

    if ( position.y <= 0)
    {
        float y = waveFunction(position.x, position.z, u_time);
        float delta = .05;

        // Calculate the two vectors from the origin vector (delta V)
        float udy = waveFunction(position.x + delta, position.z, u_time) - y;
        float vdy = waveFunction(position.x, position.z + delta, u_time) - y;
        vec3 u = vec3(delta, udy, 0);
        vec3 v = vec3(0, vdy, delta);
        ioNormal = normalize(cross(u, v));
        resultingPosition.y = y;
    }

    gl_Position = u_ModelViewProjectionMatrix * vec4(resultingPosition, 1.0);
    ioPosition = position;


}