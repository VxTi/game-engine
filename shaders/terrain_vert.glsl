#version 330 core

layout(location = 0) in vec3 position;

out vec4 screenPos;
out vec3 vertexPosition;
uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    vertexPosition = position;
    gl_Position = u_ModelViewProjectionMatrix * vec4(position, 1.0);
    screenPos = gl_Position;
}