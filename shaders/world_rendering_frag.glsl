#version 330 core

in vec3 ioFragPos;
in vec3 ioNormal;
in vec3 ioPosition;

uniform vec3 u_SunPosition;

out vec4 FragColor;

uniform vec3 u_CameraPosition;

const vec3 LIGHT_COLOR = vec3(1.0, 1.0, 1.0);

void main()
{
    vec3 objectColor = vec3(1, 1, .9);

    float opacity = 1.0f;

    if (ioPosition.y <= 1)
    {
        objectColor = vec3(0.2, 0.25, 0.3);
        opacity = .8;
    }

    vec3 normal = normalize(ioNormal);
    vec3 lightDir = normalize(u_SunPosition - ioFragPos);
    vec3 viewDir = normalize(u_CameraPosition - ioFragPos);

/** Ambient **/
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * LIGHT_COLOR;

/** Diffusion **/
    float diffuseIntensity = .5;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * LIGHT_COLOR;

/** Specular **/
    float specularStrength = .9;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * LIGHT_COLOR;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, opacity);
}