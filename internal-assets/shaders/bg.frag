#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 resolution;
uniform float zoom;
uniform vec2 cameraPos;

// Output fragment color
out vec4 finalColor;

int celDim = 200;
void main()
{
    // celDim = int(celDim * zoom);
    vec2 fragCoord = (gl_FragCoord.xy - 2. * resolution / 2.) / zoom + vec2(cameraPos.x, - cameraPos.y) * 2.;
    vec2 outUV = floor(fragCoord / celDim);
    vec2 inUV = fragCoord - outUV * celDim;
    inUV /= celDim;

    float color = 0.21;
    if (mod(int(outUV.x) + int(outUV.y), 2) == 0)
    {
        color = 0.2;
    }

    float d = distance(fragCoord, cameraPos);
    // color = d < 100 ? 0.0 : 1.0;


    finalColor = vec4(vec3(color), 1.0);

    // finalColor = vec4(fragCoord / (2. * resolution), 0.0, 1.0);
}