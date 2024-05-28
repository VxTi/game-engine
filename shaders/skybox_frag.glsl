#version 330 core

in vec4 FragPos;

out vec4 FragColor;

uniform float u_SunSize;

uniform vec3 u_SunPosition;
uniform vec2 u_CameraRotation;
uniform vec2 u_Resolution;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;

const vec4 SUN_COLOR = vec4(1, 1, 1, 1);
const vec4 SKY_MAX_REFRACTION_COLOR = vec4(.23, .64, .97, 1.0);
const vec4 SKY_MIN_REFRACTION_COLOR = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{

    // Project the sun position to screen space
    vec4 sunPosView = u_ViewMatrix * u_ModelMatrix * vec4(u_SunPosition, 1.0);
    vec4 sunPosScreen = u_ProjectionMatrix * sunPosView;

    vec4 resultingColor = mix(SKY_MIN_REFRACTION_COLOR, SKY_MAX_REFRACTION_COLOR, gl_FragCoord.y / u_Resolution.y);

    sunPosScreen /= sunPosScreen.w;

    // Convert from normalized device coordinates to screen coordinates
    sunPosScreen.xy = sunPosScreen.xy * 0.5 + 0.5;

    // Get the fragment's screen space position
    vec2 fragCoord = gl_FragCoord.xy / u_Resolution; // Assuming a window size of 800x600, adjust as necessary

    // Compute distance from fragment to sun's screen position
    float dist = length(fragCoord - sunPosScreen.xy);

    // if sun nearby, draw sun :D
    if (dist < u_SunSize) {
        // now instead of radial gradient, use
        float intensity = 1.0 - pow(dist / u_SunSize, 2);
        resultingColor = mix(resultingColor, SUN_COLOR, intensity);
    }

    FragColor = resultingColor;
}