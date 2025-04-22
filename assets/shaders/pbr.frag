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
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D occlusionMap;
uniform sampler2D heightMap;
uniform float iHeightScale = 0.1;
uniform bool useNormalMap;

// IBL maps
uniform samplerCube irradianceMap;

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
uniform vec2 iTiling = vec2(1.0);


#define PI 3.1415926535897932384626433832795

float distributionGGX(vec3 normal, vec3 halfway, float roughness) {
    float aSquare = pow(roughness, 4);
    float nDotH = max(dot(normal, halfway), 0.0);
    return aSquare / (PI * pow((pow(nDotH, 2) * (aSquare - 1.) + 1.), 2));
}

vec3 fresnelSchlick(vec3 halfway, vec3 view, vec3 f0) {
    return f0 + (1. - f0) * pow(1. - max(dot(halfway, view), 0.0), 5);
}

float geometryGGX(vec3 normal, vec3 view, float roughness) {
    float nDotV = max(dot(normal, view), 0.0);
    float k = pow(roughness + 1.0, 2) / 8;
    float denom = nDotV * (1.0 - k) + k;
    return nDotV / denom;
}

float geometrySmith(vec3 normal, vec3 view, vec3 toLight, float roughness) {
    return geometryGGX(normal, view, roughness) * geometryGGX(normal, toLight, roughness);
}

vec2 parallaxMapping(vec2 uv, vec3 viewDir, float heightScale) {
    float height = texture(heightMap, uv).r;
    vec2 p = viewDir.xy / viewDir.z * heightScale * height;
    return uv - p;
}

void main()
{
    vec3 view = normalize(viewPos - fragPosition);
    vec2 uv = vec2(fragTexCoord.x * iTiling.x, fragTexCoord.y * iTiling.y);
    vec3 tangentView = normalize(TBN * view);
    uv = parallaxMapping(uv, tangentView, iHeightScale);

    finalColor = vec4(vec3(0.0), 1.0);
    vec4 albedo = texture(albedoMap, uv) * albedoColor;
    float metallic = texture(metallicMap, uv).r;
    float roughness = texture(roughnessMap, uv).r;
    float ao = texture(occlusionMap, uv).r;
    vec3 f0 = mix(vec3(0.04), albedo.rgb, metallic);

    // normal map
    vec3 normal = useNormalMap 
        ? normalize(TBN * (texture(normalMap, uv).xyz * 2.0 - 1.0) )
        : fragNormal;

    for (int i = 0; i < lightsCount; i++) 
    {
        Light light = lights[i];
        if (light.type == LIGHT_AMBIENT) 
        {
            finalColor.rgb += light.color * light.intensity * albedo.rgb * ao;
        }
        else if (light.type == LIGHT_DIRECTION || light.type == LIGHT_POINT)
        {
            vec3 toLight = 
                light.type == LIGHT_DIRECTION ? -light.direction : light.position - fragPosition;
            float d = length(toLight);
            toLight /= d;
            vec3 halfway = normalize(toLight + view);

            // radiance
            float attenuation = 
                light.type == LIGHT_DIRECTION ? 1.0 : pow(max(0, 1 - pow(d / light.range, 4)), 2);
            vec3 radiance = light.color * light.intensity * attenuation;

            // specular
            vec3 F = fresnelSchlick(halfway, view, f0);
            float D = distributionGGX(normal, halfway, roughness);
            float G = geometrySmith(normal, view, toLight, roughness);
            float denom = 4.0 * max(dot(normal, view), 0.0) * max(dot(normal, toLight), 0.0) + 0.001;
            vec3 specular = (D * F * G) / denom;

            // diffuse
            vec3 kD = (1.0 - F) * (1.0 - metallic);
            vec3 diffuse = kD * albedo.rgb / PI;

            finalColor.rgb += 
                (diffuse + specular) * radiance * max(dot(normal, toLight), 0.0);
        }
    }

    // ambient IBL
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse = irradiance * albedo.rgb;
    // TODO: prefilter and brdf lut
    vec3 ambient = (diffuse) * ao;
    finalColor.rgb += ambient;

    // tonemapping
    finalColor.rgb = finalColor.rgb / (finalColor.rgb + vec3(1.0));

    // gamma correction
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0 / 2.2));


    // finalColor = vec4(vec3(height), 1.0);
    // finalColor = vec4(albedo.rgb, 1.0);
    // finalColor = vec4(normal, 1.0);
    // vec3 n = normalize(texture(normalMap, uv).xyz * 2.0 - 1.0);
    // finalColor.rgb = TBN[0];
    // finalColor.rgb = useNormalMap ? vec3(1.0) : vec3(0.0);
    // finalColor = vec4(vec3(useNormalMap), 1.0);
}