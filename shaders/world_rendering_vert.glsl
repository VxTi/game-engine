#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 ioFragPos;  // The position of the fragment in world space
out vec3 ioNormal;   // The normal vector of the fragment
out vec3 ioPosition; // The position of the fragment in object space

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

uniform mat4 u_ModelViewProjectionMatrix;

uniform float u_time;

const float water_level = 0.0;
const float large_waveheight = 1.0f;
const float small_waveheight = 1.0f;

// random/hash function
float hash( float n )
{
    return fract(cos(n)*41415.92653);
}


// 3d noise function
float noise( in vec3 x )
{
    vec3 p  = floor(x);
    vec3 f  = smoothstep(0.0, 1.0, fract(x));
    float n = p.x + p.y*57.0 + 113.0*p.z;

    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}

// 2d noise function
float noise(vec2 p)
{
    return noise(vec3(p, 0.0));
}


mat3 m = mat3( 0.00,  1.60,  1.20, -1.60,  0.72, -0.96, -1.20, -0.96,  1.28 );

// Fractional Brownian motion
float fbm( vec3 p )
{
    float f = 0.5000*noise( p ); p = m*p*1.1;
    f += 0.2500*noise( p ); p = m*p*1.2;
    f += 0.1666*noise( p ); p = m*p;
    f += 0.0834*noise( p );
    return f;
}

mat2 m2 = mat2(1.6,-1.2,1.2,1.6);

// Fractional Brownian motion
float fbm( vec2 p )
{
    float f = 0.5000*noise( p ); p = m2*p;
    f += 0.2500*noise( p ); p = m2*p;
    f += 0.1666*noise( p ); p = m2*p;
    f += 0.0834*noise( p );
    return f;
}

float waveFunction( vec2 p )
{
    float height = 0;

    float t = u_time;

    vec2 shift1 = 0.001*vec2( t*160.0*2.0, t*120.0*2.0 );
    vec2 shift2 = 0.001*vec2( t*190.0*2.0, -t*130.0*2.0 );

    // coarse crossing 'ocean' waves...
    float wave = 0.0;
    wave += sin(p.x*0.021  + shift2.x)*4.5;
    wave += sin(p.x*0.0172+p.y*0.010 + shift2.x*1.121)*4.0;
    wave -= sin(p.x*0.00104+p.y*0.005 + shift2.x*0.121)*4.0;
    // ...added by some smaller faster waves...
    wave += sin(p.x*0.02221+p.y*0.01233+shift2.x*3.437)*5.0;
    wave += sin(p.x*0.03112+p.y*0.01122+shift2.x*4.269)*2.5 ;
    wave *= large_waveheight;
    wave -= fbm(p*0.004-shift2*.5)*small_waveheight*24.0;
    // ...added by some distored random waves (which makes the water looks like water :)

    float amp = 6.*small_waveheight;
    shift1 *= .3;
    for (int i=0; i<4; i++)
    {
        wave -= abs(sin((noise(p*0.01+shift1)-.5)*3.14))*amp;
        amp *= .51;
        shift1 *= 1.841;
        p *= m2*0.9331;
    }

    height += wave;
    return height;
}

void main()
{
    vec3 resultingPosition = position;
    ioNormal = normal;
    ioFragPos = vec3(u_ModelMatrix * vec4(position, 1.0));
    if ( position.y <= 0)
    {
        float y = waveFunction(vec2(position.x, position.z));
        float delta = .05;

        // Calculate the two vectors from the origin vector (delta V)
        float udy = waveFunction(vec2(position.x + delta, position.z)) - y;
        float vdy = waveFunction(vec2(position.x, position.z + delta)) - y;
        vec3 u = vec3(delta, udy, 0);
        vec3 v = vec3(0, vdy, delta);
        ioNormal = normalize(cross(u, v));
        ioNormal = normalize(vec3(waveFunction(position.xz-.1) - waveFunction(position.xz+.1), 1.0, waveFunction(position.xz-.1) - waveFunction(position.xz+.1)));
        resultingPosition.y = y;
    }

    gl_Position = u_ModelViewProjectionMatrix * vec4(resultingPosition, 1.0);
    ioPosition = position;
}