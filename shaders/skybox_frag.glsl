#version 330 core

in vec4 FragPos;

out vec4 FragColor;

/** Sun parameters */
uniform float u_SunSize;
uniform float u_SunIntensity;
uniform vec4 u_SunColor;
uniform vec3 u_SunPosition;

uniform vec2 u_CameraRotation;
uniform vec2 u_Resolution;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;

const vec4 skyMaxRefractionColor = vec4(.23, .64, .97, 1.0);
const vec4 skyMinRefractionColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{

    // Project the sun position to screen space
    vec4 sunPosView = u_ViewMatrix * u_ModelMatrix * vec4(u_SunPosition, 1.0);
    vec4 sunPosScreen = u_ProjectionMatrix * sunPosView;

    vec4 resultingColor = mix(skyMinRefractionColor, skyMaxRefractionColor, gl_FragCoord.y / u_Resolution.y);

    sunPosScreen /= sunPosScreen.w;

    // Convert from normalized device coordinates to screen coordinates
    sunPosScreen.xy = sunPosScreen.xy * 0.5 + 0.5;

    // Get the fragment's screen space position
    vec2 fragCoord = gl_FragCoord.xy / u_Resolution; // Assuming a window size of 800x600, adjust as necessary

    // Compute distance from fragment to sun's screen position
    float dist = length(fragCoord - sunPosScreen.xy);

    // if sun nearby, draw sun :D
    if (dist < u_SunSize && sunPosScreen.z < 1.0 && sunPosScreen.z > 0.0) {
        // now instead of radial gradient, use

        float intensity = (1.0 - dist / u_SunSize) * 1.5;

        resultingColor = vec4(
                min(1, max(0, resultingColor.r + (u_SunColor.r - resultingColor.r) * intensity)),
                min(1, max(0, resultingColor.g + (u_SunColor.g - resultingColor.g) * intensity)),
                min(1, max(0, resultingColor.b + (u_SunColor.b - resultingColor.b) * intensity)),
                1.0
        );
    }

    FragColor = resultingColor;
}