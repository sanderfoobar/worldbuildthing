#pragma once
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <string>

#include "shared/models/texture.h"

namespace gl {

  class Material {
  public:
    Material();
    Material(QSharedPointer<Texture> tex);
    Material(GLuint tex_id);
//    Material(glm::vec3 color);
//    Material(GLuint textureID);
//    Material(glm::vec3 color, GLuint textureID);

    GLuint getAlbedoTexture() const {
      int iowegw = 1;
      return m_albedoID;
    }
    void setAlbedoTexture(GLuint id) { m_albedoID = id; }

    GLuint getNormalTexture() const { return m_normalID; }
    void setNormalTexture(GLuint id) { m_normalID = id; }

    GLuint getMetallicTexture() const { return m_metallicID; }
    void setMetallicTexture(GLuint id) { m_metallicID = id; }

    GLuint getRoughnessTexture() const { return m_roughnessID; }
    void setRoughnessTexture(GLuint id) { m_roughnessID = id; }

    GLuint getAOTexture() const { return m_aoID; }
    void setAOTexture(GLuint id) { m_aoID = id; }

    // picking
    uint32_t getPickingID() const { return m_pickingID; }
    void setPickingID(uint32_t id) { m_pickingID = id; }

    void bind();
    void unbind();
    [[nodiscard]] bool is_bound() const { return m_bound; }

  private:
    bool m_bound = false;

    glm::vec3 m_color;
    glm::vec3 m_ambient;
    glm::vec3 m_specular;
    float m_shininess;

    GLuint m_albedoID = 0;
    GLuint m_normalID = 0;
    GLuint m_metallicID = 0;
    GLuint m_roughnessID = 0;
    GLuint m_aoID = 0;
    uint32_t m_pickingID;

    QSharedPointer<Texture> m_tex;
  };

} // namespace gl
