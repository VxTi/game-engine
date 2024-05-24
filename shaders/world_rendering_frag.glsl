#version 330 core

in vec3 ioFragPos;
in vec3 ioNormal;
in vec3 ioPosition;

out vec4 FragColor;

uniform vec3 u_CameraPosition;


void main()
{
    vec3 objectColor = vec3(1, 1, .9);

    float lightScalar = 1.0 - (1.0 / sqrt(max(ioPosition.y, 0)));
    vec3 lightColor = vec3(lightScalar * .5, lightScalar * .6, lightScalar * .7);

    if ( lightScalar <= .1)
        lightColor = vec3(.15, .15, .3);

    vec3 lightPos = vec3(.07, .7, .7);

    vec3 normal = normalize(ioNormal);
    vec3 lightDir = normalize(lightPos - ioFragPos);
    vec3 viewDir = normalize(u_CameraPosition - ioFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    /** Ambient **/
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    /** Diffusion **/
    float diffuseIntensity = .5;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * lightColor;

    /** Specular **/
    float specularStrength = 1;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}