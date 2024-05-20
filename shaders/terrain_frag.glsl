#version 330 core

in vec4 screenPos;
in vec3 vertexPosition;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexPosition, 1.0);
}