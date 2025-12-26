#pragma once
#include <string>

namespace shaders {

const std::string fullbright_vertex = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;

out vec2 fragTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  fragTexCoord = inTexCoord;
  gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
}
)";

const std::string fullbright_frag = R"(
#version 400 core
in vec2 fragTexCoord;
out vec4 fragColor;
uniform sampler2D uAlbedoMap;
uniform vec2 uUVScale;
uniform vec2 uUVOffset;

void main()
{
  vec2 uv = fragTexCoord / 0.8;
  uv = uv * uUVScale + uUVOffset;
  vec3 albedo = texture(uAlbedoMap, uv).rgb;
  fragColor = vec4(albedo, 1.0);
}
)";

  const std::string phong_vertex = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

flat out vec3 faceNormal;
out vec3 fragPos;
out vec2 fragTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  fragPos = vec3(uModel * vec4(inPosition, 1.0));
  faceNormal = normalize(mat3(transpose(inverse(uModel))) * inNormal);
  fragTexCoord = inTexCoord;
  gl_Position = uProjection * uView * vec4(fragPos, 1.0);
}
)";

//   const std::string phong_frag = R"(
// #version 400 core
// in vec3 fragPos;
// flat in vec3 faceNormal;
// in vec2 fragTexCoord;
//
// out vec4 fragColor;
//
// uniform sampler2D uAlbedoMap;
// uniform vec2 uUVScale;
// uniform vec2 uUVOffset;
// uniform vec3 uLightDir;
// uniform vec3 uLightColor;
// uniform vec3 uAmbientColor;
// uniform float uAmbientIntensity;
// uniform vec3 uViewPos;
//
// const float FACE_INTENSITY = 0.1;
// const float FACE_BOOST = 1.3;
//
// void main()
// {
//   vec2 uv = fragTexCoord / 0.8;
//   uv = uv * uUVScale + uUVOffset;
//   vec3 albedo = texture(uAlbedoMap, uv).rgb;
//
//   vec3 N = normalize(faceNormal);
//   vec3 L = normalize(-uLightDir);
//   vec3 V = normalize(uViewPos - fragPos);
//   vec3 R = reflect(-L, N);
//
//   float diff = max(dot(N,L),0.0);
//   float spec = pow(max(dot(V,R),0.0),16.0);
//
//   vec3 color = uAmbientIntensity*uAmbientColor + diff*uLightColor + spec*uLightColor*0.5;
//   color *= albedo;
//
//   float faceShade = 1.0 + FACE_INTENSITY*(dot(N, normalize(vec3(1.0))) - 0.5);
//   color *= faceShade * FACE_BOOST;
//
//   fragColor = vec4(color,1.0);
// }
// )";

  const std::string phong_frag = R"(
#version 400 core
in vec3 fragPos;
flat in vec3 faceNormal;
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D uAlbedoMap;
uniform vec2 uUVScale;
uniform vec2 uUVOffset;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;
uniform vec3 uViewPos;

const bool FAKE_SHADING_ENABLED = true;
const bool PHONG_ENABLED = true;

const float DIFFUSE_STRENGTH = 0.85;
const float SPECULAR_STRENGTH = 0.15;
const float SPECULAR_POWER = 24.0;
const float MAX_LIGHT = 1.2;

const float FACE_SHADES[6] = float[6](
  0.92,
  0.98,
  1.04,
  1.07,
  1.10,
  1.13
);

void main()
{
  vec2 uv = fragTexCoord / 0.8;
  uv = uv * uUVScale + uUVOffset;
  vec3 albedo = texture(uAlbedoMap, uv).rgb;

  vec3 N = normalize(faceNormal);
  vec3 color = albedo;

  if (PHONG_ENABLED)
  {
    vec3 L = normalize(-uLightDir);
    vec3 V = normalize(uViewPos - fragPos);
    vec3 R = reflect(-L, N);

    float diff = max(dot(N,L),0.0);
    float spec = pow(max(dot(V,R),0.0),SPECULAR_POWER);

    vec3 lighting =
      uAmbientIntensity * uAmbientColor +
      diff * uLightColor * DIFFUSE_STRENGTH +
      spec * uLightColor * SPECULAR_STRENGTH;

    lighting = min(lighting, vec3(MAX_LIGHT));
    color = lighting * albedo;
  }

  int axis;
  if (abs(N.x) > abs(N.y) && abs(N.x) > abs(N.z))
    axis = N.x > 0.0 ? 0 : 1;
  else if (abs(N.y) > abs(N.z))
    axis = N.y > 0.0 ? 2 : 3;
  else
    axis = N.z > 0.0 ? 4 : 5;

  color *= FACE_SHADES[axis];

  fragColor = vec4(color,1.0);
}
)";

}

