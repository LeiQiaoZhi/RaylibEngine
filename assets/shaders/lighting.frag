#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
in mat3 TBN;

// Input uniform values
uniform sampler2D albedoMap;
uniform vec4 albedoColor;
uniform sampler2D normalMap;
uniform bool useNormalMap;

// Output fragment color
out vec4 finalColor;

// Lighting
#define MAX_LIGHTS 4
#define LIGHT_AMBIENT   0
#define LIGHT_DIRECTION 1
#define LIGHT_POINT     2

struct Light {
    int type;
    float intensity;
    vec3 color;
    vec3 direction; // For directional lights
    vec3 position; // For point lights
    float range;    // For point lights
};
uniform Light lights[MAX_LIGHTS];
uniform int lightsCount;

// Other uniforms
uniform vec3 viewPos;
uniform float iSpecularPower = 16.0;
uniform float iSpecularStrength = 0.2;
uniform vec2 iTiling = vec2(1.0);

void main()
{
    vec2 uv = vec2(fragTexCoord.x * iTiling.x, fragTexCoord.y * iTiling.y);

    finalColor = vec4(vec3(0.0), 1.0);
    vec4 baseColor = texture(albedoMap, uv) * albedoColor;

    // normal map
    vec3 normal = useNormalMap 
        ? normalize(TBN * (texture(normalMap, uv).xyz * 2.0 - 1.0))
        : fragNormal;

    for (int i = 0; i < lightsCount; i++) 
    {
        Light light = lights[i];
        if (light.type == LIGHT_AMBIENT) 
        {
            finalColor.rgb += light.color * light.intensity * baseColor.rgb;
        }
        else if (light.type == LIGHT_DIRECTION) 
        {
            float diffuse = max(dot(normal, -light.direction), 0.0);
            finalColor.rgb += light.color * light.intensity * diffuse * baseColor.rgb;

            vec3 toCamera = normalize(viewPos - fragPosition);
            vec3 reflected = reflect(light.direction, normal);
            float specular = pow(max(dot(toCamera, reflected), 0.0), iSpecularPower) * iSpecularStrength;
            finalColor.rgb += light.color * light.intensity * specular;
        }
        else if (light.type == LIGHT_POINT)
        {
            vec3 toLight = light.position - fragPosition;
            float d = length(toLight);
            toLight /= d;
            float attenuation = pow(max(0, 1 - pow(d / light.range, 4)), 2);

            float diffuse = max(dot(normal, toLight), 0.0);
            finalColor.rgb += light.color * light.intensity * diffuse * baseColor.rgb * attenuation;

            vec3 toCamera = normalize(viewPos - fragPosition);
            vec3 reflected = reflect(-toLight, normal);
            float specular = pow(max(dot(toCamera, reflected), 0.0), 16.0) * 0.2;
            finalColor.rgb += light.color * light.intensity * specular * attenuation;
        }
    }

    // finalColor = vec4(baseColor.rgb, 1.0);
    // finalColor = vec4(normal, 1.0);
    // finalColor = vec4(vec3(useNormalMap), 1.0);
}