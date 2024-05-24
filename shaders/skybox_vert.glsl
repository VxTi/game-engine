#version 330 core

layout(location = 0) in vec3 position;

out float ioFragHeight;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 u_CameraPosition;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(position, 1.0);
    ioFragHeight = position.y;
}