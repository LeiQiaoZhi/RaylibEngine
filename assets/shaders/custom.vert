#version 330

// Input vertex attributes, in object space
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp; // obect => world => view => projection (clip space)
uniform mat4 matModel; // object => world
uniform mat4 matNormal; // object => world 

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out mat3 TBN; // Tangent, Bitangent, Normal, for normal mapping

void main()
{
    // Send vertex attributes to fragment shader
    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));

    vec3 fragTangent = normalize(vec3(matNormal*vec4(vertexTangent, 1.0)));
    fragTangent = normalize(fragTangent - dot(fragTangent, fragNormal)*fragNormal);
    vec3 vertexBinormal = cross(fragNormal, fragTangent);
    vec3 fragBinormal = normalize(vec3(matNormal*vec4(vertexBinormal, 1.0)));
    fragBinormal = cross(fragNormal, fragTangent);
    TBN = mat3(fragTangent, fragBinormal, fragNormal);


    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}