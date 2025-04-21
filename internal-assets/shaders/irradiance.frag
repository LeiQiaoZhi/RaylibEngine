#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform sampler2D equirectangularMap;
uniform samplerCube environmentMap;
uniform vec3 viewPos;
uniform bool flipY;

// Output fragment color
out vec4 finalColor;

#define PI 3.14159265358979323846

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

    vec3 forward = viewDir;
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, forward));
    up = normalize(cross(forward, right));

    vec3 irradiance = vec3(0.0);
    float sampleDelta = 0.025;
    int numSamples = 0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tanSpaceDir = vec3(
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta)
            );
            vec3 sampleDir = normalize(vec3(
                right * tanSpaceDir.x +
                up * tanSpaceDir.y +
                forward * tanSpaceDir.z
            ));
            irradiance += texture(environmentMap, sampleDir).rgb * cos(theta) * sin(theta);
            numSamples++;
        }
    }
    irradiance *= (PI / float(numSamples));

    finalColor = vec4(irradiance, 1.0);
}
