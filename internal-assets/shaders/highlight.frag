#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec3 color = texture(texture0, fragTexCoord).rgb;
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    color = mix(color, yellow, 0.5);
    finalColor = vec4(color, 1.0);
}