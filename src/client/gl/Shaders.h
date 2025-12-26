#pragma once
#include <string>

// Shadow map vertex shader
const std::string shadowVertexShader = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;

uniform mat4 uModel;
uniform mat4 uLightSpaceMatrix;

void main()
{
    gl_Position = uLightSpaceMatrix * uModel * vec4(inPosition, 1.0);
}
)";

// Shadow map fragment shader (depth only)
const std::string shadowFragmentShader = R"(
#version 400 core
void main() { }
)";

// Phong-style shader with baked UVs
const std::string phongVertexShader = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    flat uint pickingID;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform uint uPickingID;

void main()
{
    vec4 worldPos = uModel * vec4(inPosition, 1.0);
    vs_out.fragPos = vec3(worldPos);
    vs_out.normal = normalize(mat3(transpose(inverse(uModel))) * inNormal);
    vs_out.texCoord = inTexCoord; // UV already baked
    vs_out.pickingID = uPickingID;

    gl_Position = uProjection * uView * worldPos;
}
)";

// Phong-style shader with baked UVs
const std::string phongFragmentShader = R"(
#version 420 core
in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    flat uint pickingID;
} fs_in;

out vec4 fragColor;

uniform sampler2D uAlbedoMap;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;
uniform vec3 uViewPos;

uniform sampler2D uShadowMap;
uniform float uShadowBias;
uniform mat4 uLightSpaceMatrix;

uniform uint uSelectedIDs[36];
uniform int uSelectedCount;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

layout(std140, binding = 3) uniform LightInfo {
  int ambientLightNum;
  int directionalLightNum;
  int pointLightNum;
  int spotLightNum;
  vec3 padding0;
  DirectionalLight directionalLight[8];
};

vec3 calcDirectionalLight(int idx, vec3 normal, vec3 color) {
    vec3 L = normalize(-directionalLight[idx].direction);
    vec3 V = normalize(uViewPos - fs_in.fragPos);
    vec3 R = reflect(-L, normal);

    const float diffuseBoost = 0.7;
    const float specPower = 16.0;
    const float specStrength = 0.5;

    float diff = max(dot(normal, L), 0.0);
    diff = mix(diff, 1.0, diffuseBoost);

    float spec = pow(max(dot(V, R), 0.0), specPower);

    return diff * color + spec * directionalLight[idx].color * specStrength;
}

bool isSelected(uint pickingID)
{
    for(int i = 0; i < uSelectedCount; i++)
        if(uSelectedIDs[i] == pickingID) return true;
    return false;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) return 0.0;

    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    //float bias = max(uShadowBias * (1.0 - dot(normalize(fs_in.normal), normalize(-uLightDir))), 0.0001);
    float bias = 0.01;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    vec2 uv = fs_in.texCoord / 3.2;
    vec3 albedo = texture(uAlbedoMap, uv).rgb;

    vec3 N = normalize(fs_in.normal);
    vec3 V = normalize(uViewPos - fs_in.fragPos);

    vec3 color = uAmbientIntensity * albedo;

    for(int i = 0; i < directionalLightNum; i++)
    {
        vec3 lightColor = calcDirectionalLight(i, N, albedo);
        vec4 fragPosLightSpace = uLightSpaceMatrix * vec4(fs_in.fragPos, 1.0);
        float shadow = ShadowCalculation(fragPosLightSpace);
        color += lightColor * (1.0 - shadow);
    }

    if(isSelected(fs_in.pickingID))
        color = mix(color, vec3(1.0, 0.0, 0.0), 0.3);

    // simple gamma-correct
    // color = pow(color, vec3(1.0/2.2));
    fragColor = vec4(color, 1.0);
}
)";

// Picking shaders
const std::string pickingVertexShader = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
void main()
{
    gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
}
)";

const std::string pickingFragmentShader = R"(
#version 400 core
out vec4 fragColor;
uniform uint uPickingID;

void main()
{
    uint r = (uPickingID >> 0u) & 0xFFu;
    uint g = (uPickingID >> 8u) & 0xFFu;
    uint b = (uPickingID >> 16u) & 0xFFu;
    uint a = (uPickingID >> 24u) & 0xFFu;
    fragColor = vec4(float(r)/255.0, float(g)/255.0, float(b)/255.0, float(a)/255.0);
}
)";
