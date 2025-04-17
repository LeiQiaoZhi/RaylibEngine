#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform sampler2D equirectangularMap;
uniform vec3 viewPos;
uniform bool flipY;

// Output fragment color
out vec4 finalColor;

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main()
{
    vec3 viewDir = normalize(fragPosition - viewPos);
    if (flipY) viewDir.y = -viewDir.y;
    vec2 uv = SampleSphericalMap(viewDir);

    // Fetch color from texture map
    vec3 color = texture(equirectangularMap, uv).rgb;

    // Calculate final fragment color
    finalColor = vec4(color, 1.0);

    // reinhard 
    finalColor = vec4(color / (color + vec3(1.0)), 1.0);
}
