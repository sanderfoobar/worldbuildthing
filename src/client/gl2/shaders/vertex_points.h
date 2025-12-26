#pragma once
#include <string>

namespace shaders {

const std::string vertex_points_vertex = R"(
#version 400 core
layout(location = 0) in vec3 inPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
  gl_PointSize = 36.0;
}
)";

const std::string vertex_points_frag = R"(
#version 400 core
out vec4 fragColor;

void main()
{
  fragColor = vec4(1.0, 0.2, 0.2, 1.0);
}
)";

}