#version 330 core

in vec4 fragPos;
in vec3 o_pos;

out vec4 FragColor;

void main()
{
    float mixAmount = 1.0 - (1.0 / sqrt(dot(o_pos, vec3(0))));
    FragColor = mix(vec4(0.0, 0.0, 1.0, 1.0), vec4(.2, 0.9, 0.21, 1.0), mixAmount);
}