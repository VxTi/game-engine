#version 330 core

layout(location = 0) in vec3 position;

//out vec4 FragModelPos;
out vec4 FragPos;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 u_CameraPosition;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(position, 1.0);
    FragPos = u_ModelMatrix * vec4(position, 1.0);
    //FragModelPos = u_ModelMatrix * vec4(position, 1.0);
}