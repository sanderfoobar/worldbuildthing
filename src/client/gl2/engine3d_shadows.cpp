#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <random>

#include "client/lib/globals.h"
#include "engine3d.h"

namespace gl {
void Engine3D::generateShadows() {
  cb_generate_shadows();
}

void Engine3D::renderShadowMap(bool generate) {
  if (!m_shadowShaderID) return;
  if (!generate) return;

  glm::vec3 up = m_shadowUseYUp ? glm::vec3(0,1,0) : glm::vec3(0,0,1);
  glm::mat4 lightView = glm::lookAt(m_sun_position, m_sceneCenter, up);

  // compute tight ortho bounds in light space
  glm::vec3 lightMin{FLT_MAX}, lightMax{-FLT_MAX};
  for (auto &obj : m_scene->gameObjects) {
    auto [bmin, bmax] = obj->getCachedWorldAABB();
    glm::vec3 corners[8] = {
      {bmin.x,bmin.y,bmin.z},{bmin.x,bmin.y,bmax.z},{bmin.x,bmax.y,bmin.z},{bmin.x,bmax.y,bmax.z},
      {bmax.x,bmin.y,bmin.z},{bmax.x,bmin.y,bmax.z},{bmax.x,bmax.y,bmin.z},{bmax.x,bmax.y,bmax.z}
    };
    for (auto &c : corners) {
      glm::vec3 lc = glm::vec3(lightView * glm::vec4(c,1.0f));
      lightMin = glm::min(lightMin, lc);
      lightMax = glm::max(lightMax, lc);
    }
  }

  glm::mat4 lightProj = glm::ortho(lightMin.x * m_shadowOrthoScale, lightMax.x * m_shadowOrthoScale,
                                   lightMin.y * m_shadowOrthoScale, lightMax.y * m_shadowOrthoScale,
                                   -lightMax.z - 10.0f, -lightMin.z + 10.0f); // small z margin
  m_lightSpaceMatrix = lightProj * lightView;

  GL.glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
  GL.glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
  GL.glClear(GL_DEPTH_BUFFER_BIT);

  GL.glUseProgram(m_shadowShaderID);
  GL.glUniformMatrix4fv(GL.glGetUniformLocation(m_shadowShaderID,"uLightSpaceMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_lightSpaceMatrix));
  GL.glUniform1f(GL.glGetUniformLocation(m_shadowShaderID, "uShadowBias"), m_shadowBias);

  for (auto &obj : m_scene->gameObjects) {
    glm::mat4 model = obj->getModelMatrixCached();
    GL.glUniformMatrix4fv(GL.glGetUniformLocation(m_shadowShaderID,"uModel"),
                       1, GL_FALSE, glm::value_ptr(model));
    auto mesh = obj->getMesh();
    for (int face = 0; face < 6; face++)
      mesh->renderFace(face);
  }

  GL.glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GL.glViewport(0,0,m_width,m_height);

  m_generate_shadows = false;
}
}