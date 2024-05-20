#version 330 core

layout(location = 0) in vec3 position;

out vec4 fragPos;
out vec3 o_pos;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    o_pos = position;
    gl_Position = u_ModelViewProjectionMatrix * vec4(position, 1.0);
    fragPos = gl_Position;
}