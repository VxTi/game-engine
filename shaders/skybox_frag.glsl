#version 330 core

in float ioFragHeight;

out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(1, 1, 1, 1), vec4(.23, .64, .97, 1), ioFragHeight);
}