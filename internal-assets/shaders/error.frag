#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D mainTex;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    // magenta
    finalColor = vec4(1.0, 0.0, 1.0, 1.0);
}