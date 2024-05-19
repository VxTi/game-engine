#version 330 core

layout(location = 0) in vec3 position;

out vec4 fragPos;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(position, 1.0);
    //gl_Position = projection * view * model * vec4(position, 1.0);
    fragPos = gl_Position;
}