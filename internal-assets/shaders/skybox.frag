#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform samplerCube environmentMap;
uniform vec3 viewPos;
uniform bool flipY;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec3 viewDir = normalize(fragPosition - viewPos);
    if (flipY) viewDir.y = -viewDir.y;
    vec3 color = texture(environmentMap, viewDir).rgb;
    finalColor = vec4(color, 1.0);
}