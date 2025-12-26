#pragma once
#include <string>

namespace shaders {
const std::string debug_normals_vertex = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

flat out vec3 faceNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  faceNormal = mat3(transpose(inverse(uModel))) * inNormal;
  gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
}
)";

const std::string debug_normals_frag = R"(
#version 400 core
flat in vec3 faceNormal;
out vec4 fragColor;
void main()
{
  vec3 N = normalize(faceNormal);
  fragColor = vec4(0.5 * (N + vec3(1.0)), 1.0);
}
)";

}