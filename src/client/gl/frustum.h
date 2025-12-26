#pragma once
#include <glm/glm.hpp>
#include <array>

class Frustum {
public:
  // Each plane: (normal.xyz, distance)
  std::array<glm::vec4, 6> planes;

  // Extract planes from view-projection matrix
  void extract(const glm::mat4 &vp) {
    // Left
    planes[0] = glm::vec4(
        vp[0][3] + vp[0][0],
        vp[1][3] + vp[1][0],
        vp[2][3] + vp[2][0],
        vp[3][3] + vp[3][0]);
    // Right
    planes[1] = glm::vec4(
        vp[0][3] - vp[0][0],
        vp[1][3] - vp[1][0],
        vp[2][3] - vp[2][0],
        vp[3][3] - vp[3][0]);
    // Bottom
    planes[2] = glm::vec4(
        vp[0][3] + vp[0][1],
        vp[1][3] + vp[1][1],
        vp[2][3] + vp[2][1],
        vp[3][3] + vp[3][1]);
    // Top
    planes[3] = glm::vec4(
        vp[0][3] - vp[0][1],
        vp[1][3] - vp[1][1],
        vp[2][3] - vp[2][1],
        vp[3][3] - vp[3][1]);
    // Near
    planes[4] = glm::vec4(
        vp[0][3] + vp[0][2],
        vp[1][3] + vp[1][2],
        vp[2][3] + vp[2][2],
        vp[3][3] + vp[3][2]);
    // Far
    planes[5] = glm::vec4(
        vp[0][3] - vp[0][2],
        vp[1][3] - vp[1][2],
        vp[2][3] - vp[2][2],
        vp[3][3] - vp[3][2]);
    // Normalize
    for (auto &p: planes) {
      float len = glm::length(glm::vec3(p));
      p /= len;
    }
  }

  // AABB test
  bool isBoxVisible(const glm::vec3 &min, const glm::vec3 &max) const {
    for (const auto &plane: planes) {
      glm::vec3 n = glm::vec3(plane);
      float d = plane.w;
      // Positive vertex
      glm::vec3 p = min;
      if (n.x >= 0)
        p.x = max.x;
      if (n.y >= 0)
        p.y = max.y;
      if (n.z >= 0)
        p.z = max.z;
      if (glm::dot(n, p) + d < -1e-5f)
        return false;
    }
    return true;
  }
};
