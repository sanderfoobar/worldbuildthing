#include "gameobject.h"

namespace gl {
  GameObject::GameObject(const std::shared_ptr<Mesh> &mesh) :
    m_mesh(mesh),
    m_position(0.0f),
    m_rotation(0.0f),
    m_scale(1.0f),
    m_transformDirty(true) {
  }

  void GameObject::updateModelMatrix() {
    if (!m_transformDirty)
      return;

    m_modelMatrix = glm::mat4(1.0f);
    m_modelMatrix = glm::translate(m_modelMatrix, m_position);

    // Apply rotation (X, Y, Z)
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

    m_transformDirty = false;
  }

  glm::mat4 GameObject::getModelMatrixCached() const {
    return m_modelMatrix;
  }

  glm::mat4 GameObject::getModelMatrix() {
    updateModelMatrix();
    return m_modelMatrix;
  }

  void GameObject::render() {
    if (m_mesh) {
      m_mesh->render();
    }
  }

  void GameObject::computeWorldAABB() {
    auto localAABB = m_mesh->getAABB();
    glm::mat4 model = getModelMatrix();
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
    m_cachedWorldAABB = {minV, maxV};
  }

} // namespace gl
