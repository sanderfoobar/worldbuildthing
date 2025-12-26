#include "material.h"
#include <GL/gl.h>

#include "../lib/globals.h"
#include "client/gl/gl_functions.h"

#define GL (*gl::g_glFunctions)

namespace gl {

// constructors
Material::Material() {

}

Material::Material(QSharedPointer<Texture> tex) {
  m_tex = tex;
}

Material::Material(GLuint tex_id) {
  m_albedoID = tex_id;
  m_bound = true;
}


// bind all PBR textures; fall back to defaults if missing
void Material::bind() {
  GL.glActiveTexture(GL_TEXTURE0);
  GL.glBindTexture(GL_TEXTURE_2D, m_albedoID ? m_albedoID : g::texDefaultWhite);
  GL.glActiveTexture(GL_TEXTURE1);
  GL.glBindTexture(GL_TEXTURE_2D, m_normalID ? m_normalID : g::texDefaultNormal);
  GL.glActiveTexture(GL_TEXTURE2);
  GL.glBindTexture(GL_TEXTURE_2D, m_metallicID ? m_metallicID : g::texDefaultWhite);
  GL.glActiveTexture(GL_TEXTURE3);
  GL.glBindTexture(GL_TEXTURE_2D, m_roughnessID ? m_roughnessID : g::texDefaultWhite);
  GL.glActiveTexture(GL_TEXTURE4);
  GL.glBindTexture(GL_TEXTURE_2D, m_aoID ? m_aoID : g::texDefaultWhite);
  m_bound = true;
}

// unbind all
void Material::unbind() {
  for (int i = 0; i <= 4; ++i) {
    GL.glActiveTexture(GL_TEXTURE0 + i);
    GL.glBindTexture(GL_TEXTURE_2D, 0);
  }
  GL.glActiveTexture(GL_TEXTURE0);
  m_bound = false;
}

} // namespace gl
