#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "mesh.h"

namespace gl {

  class GameObject {
  public:
    void computeWorldAABB();
    [[nodiscard]] std::pair<glm::vec3, glm::vec3> getCachedWorldAABB() const { return m_cachedWorldAABB; }

  private:
    std::pair<glm::vec3, glm::vec3> m_cachedWorldAABB;

  public:
    explicit GameObject(const std::shared_ptr<Mesh> &mesh);

    // Transform
    void setPosition(glm::vec3 pos) {
      m_position = pos;
      m_transformDirty = true;
    }

    void setRotation(glm::vec3 rot) {
      m_rotation = rot;
      m_transformDirty = true;
    }

    void setScale(glm::vec3 scale) {
      m_scale = scale;
      m_transformDirty = true;
    }

    [[nodiscard]] glm::vec3 getPosition() const { return m_position; }
    [[nodiscard]] glm::vec3 getRotation() const { return m_rotation; }
    [[nodiscard]] glm::vec3 getScale() const { return m_scale; }

    // Model matrix
    glm::mat4 getModelMatrix();

    // Mesh access
    std::shared_ptr<Mesh> getMesh() { return m_mesh; }
    [[nodiscard]] const std::shared_ptr<Mesh> getMesh() const { return m_mesh; }

    // Rendering
    void render();

    glm::mat4 getModelMatrixCached() const;

    // Returns world-space AABB (min, max)
    [[nodiscard]] std::pair<glm::vec3, glm::vec3> getWorldAABB() const {
      auto localAABB = m_mesh->getAABB();
      const glm::mat4 model = const_cast<GameObject *>(this)->getModelMatrix();
      // Transform all 8 corners
      glm::vec3 minV = glm::vec3(model * glm::vec4(localAABB.first, 1.0f));
      glm::vec3 maxV = minV;
      for (int i = 0; i < 8; ++i) {
        glm::vec3 corner = localAABB.first;
        if (i & 1)
          corner.x = localAABB.second.x;
        if (i & 2)
          corner.y = localAABB.second.y;
        if (i & 4)
          corner.z = localAABB.second.z;
        glm::vec3 worldCorner = glm::vec3(model * glm::vec4(corner, 1.0f));
        minV = glm::min(minV, worldCorner);
        maxV = glm::max(maxV, worldCorner);
      }
      return {minV, maxV};
    }

  private:
    std::shared_ptr<Mesh> m_mesh;
    glm::vec3 m_position;
    glm::vec3 m_rotation; // Euler angles in degrees
    glm::vec3 m_scale;
    glm::mat4 m_modelMatrix;
    bool m_transformDirty;

    void updateModelMatrix();
  };

} // namespace gl
