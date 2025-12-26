#include <array>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <random>

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>

#include "shared/lib/debounce.h"

#include "client/gl2/shaders/fullbright.h"
#include "client/gl2/shaders/vertex_points.h"
#include "client//lib/globals.h"
#include "client/lib/vmfpp/vmfpp.h"
#include "client/gl/Shaders.h"
#include "client/gl/frustum.h"
#include "client/gl/gl_functions.h"
#include "client/gl/texture_loader.h"
#include "client/gl/gl_uniform_buffer_object.h"

#include "engine3d.h"

#include "shaders/debug_normals.h"

namespace gl {

OpenGLUniformBufferObject *Engine3D::m_cameraInfo = nullptr;
OpenGLUniformBufferObject *Engine3D::m_lightInfo = nullptr;

Engine3D::Engine3D(QWidget *parent) : QObject(parent) {
  // auto cam_start_pos = glm::vec3(100.444,88.0918, 15);
  auto cam_start_pos = glm::vec3(0,8,15);
  m_camera = std::make_unique<Camera>(cam_start_pos);
  // m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 8.0f, 15.0f));

  cb_resize_debounce = QFunctionUtils::Debounce([this](int w, int h){
    resizeGL(w, h);
  }, 100);

  // initialize key tracking
  std::memset(m_keysPressed, 0, sizeof(m_keysPressed));

  cb_generate_shadows = QFunctionUtils::Debounce([this]{
    m_generate_shadows = true;
  }, 50);
  m_generate_shadows = true;

  connect(this, &Engine3D::sceneLoaded, [this] {
    calculateSceneBoundsAndLightingAndAlsoPossiblyOtherThings();
  });
}

void Engine3D::initializeGL() {
  initializeOpenGLFunctions();
  g_glFunctions = this;
  qDebug() << "OpenGL Version:" << reinterpret_cast<const char *>(GL.glGetString(GL_VERSION));

  m_devicePixelRatio = g::devicePixelRatio;

  GL.glClearColor(0.15f, 0.18f, 0.33f, 1.0f);

  GL.glEnable(GL_DEPTH_TEST);

  // shadow framebuffer
  GL.glGenFramebuffers(1, &m_shadowFBO);

  // shadow depth texture
  GL.glGenTextures(1, &m_shadowMap);
  GL.glBindTexture(GL_TEXTURE_2D, m_shadowMap);
  GL.glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  const float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GL.glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  GL.glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
  GL.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
  GL.glDrawBuffer(GL_NONE);
  GL.glReadBuffer(GL_NONE);
  if (GL.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cerr << "Shadow framebuffer not complete!\n";
  GL.glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // load shaders
  m_mainShaderID = loadShaderProgramFromDisk(
    "/home/dsc/CLionProjects/godot/texture_browser/src/client/gl2/shaders/main_vertex.glsl",
    "/home/dsc/CLionProjects/godot/texture_browser/src/client/gl2/shaders/main_frag.glsl");
  m_pointViewShaderID = loadShaderProgram(shaders::vertex_points_vertex, shaders::vertex_points_frag);

  //m_phongShaderID = loadShaderProgram(shaders::phong_vertex, shaders::phong_frag);
  //m_debugNormalsShaderID = loadShaderProgram(shaders::debug_normals_vertex, shaders::debug_normals_frag);
  //m_fullBrightID = loadShaderProgram(shaders::fullbright_vertex, shaders::fullbright_frag);
  //m_pbrShaderID = loadShaderProgram(phongVertexShader, phongFragmentShader);
  m_shadowShaderID = loadShaderProgram(shadowVertexShader, shadowFragmentShader);
  m_pickingShaderID = loadShaderProgram(pickingVertexShader, pickingFragmentShader);

  // picking framebuffer
  GL.glGenFramebuffers(1, &m_pickingFBO);
  GL.glGenTextures(1, &m_pickingTexture);
  GL.glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
  GL.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  GL.glBindFramebuffer(GL_FRAMEBUFFER, m_pickingFBO);
  GL.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);

  GL.glGenRenderbuffers(1, &m_pickingDepthRBO);
  GL.glBindRenderbuffer(GL_RENDERBUFFER, m_pickingDepthRBO);
  GL.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_width, m_height);
  GL.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_pickingDepthRBO);

  if (GL.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cerr << "Picking framebuffer not complete!\n";
  GL.glBindFramebuffer(GL_FRAMEBUFFER, 0);

  g::glTextureManager->init();

  m_init = true;
  emit initDone();
}

void Engine3D::paintGL() {
  GL.glClearColor(0.15f, 0.18f, 0.33f, 1.0f);
  GL.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (m_scene.isNull())
    return;

  // face culling
  GL.glEnable(GL_CULL_FACE);
  GL.glCullFace(GL_BACK);
  GL.glFrontFace(GL_CW);

  // set main shader
  if (g::renderModes.has(g::RenderMode::POINTS)) {
    m_currentShader = m_pointViewShaderID;
  } else {
    m_currentShader = m_mainShaderID;
  }

  const auto currentTime = std::chrono::high_resolution_clock::now();
  m_deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
  m_lastFrameTime = currentTime;

  updateCamera(m_deltaTime);

  GL.glUseProgram(m_currentShader);

  const glm::vec3 viewPos = m_camera->getPosition();
  glm::mat4 view = m_camera->getViewMatrix();
  glm::mat4 projection = m_camera->getProjectionMatrix(float(m_width)/float(m_height));

  GL.glUniform1i(GL.glGetUniformLocation(m_currentShader, "uPhongEnabled"), g::renderModes.has(g::RenderMode::PHONG));
  GL.glUniform1i(GL.glGetUniformLocation(m_currentShader, "uFakeShadingEnabled"), g::renderModes.has(g::RenderMode::FAKE_SHADING));
  GL.glUniform1i(GL.glGetUniformLocation(m_currentShader, "uDebugNormalsEnabled"), g::renderModes.has(g::RenderMode::DEBUG_NORMALS));

  // light
  glm::vec3 lightColor(5.0f, 5.0f, 4.5f);
  glm::vec3 ambientColor(0.5f, 0.5f, 0.5f);
  float ambientIntensity = 1.8f;

  GL.glUniform3f(GL.glGetUniformLocation(m_currentShader, "uLightPos"), m_sun_position.x, m_sun_position.y, m_sun_position.z);
  GL.glUniform3f(GL.glGetUniformLocation(m_currentShader, "uLightDirPhong"), m_sun_direction.x, m_sun_direction.y, m_sun_direction.z);
  GL.glUniform3f(GL.glGetUniformLocation(m_currentShader, "uLightDir"), m_sun_direction.x, m_sun_direction.y, m_sun_direction.z);
  GL.glUniform3f(GL.glGetUniformLocation(m_currentShader, "uLightColor"), lightColor.x, lightColor.y, lightColor.z);
  GL.glUniform3f(GL.glGetUniformLocation(m_currentShader, "uAmbientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
  GL.glUniform1f(GL.glGetUniformLocation(m_currentShader, "uAmbientIntensity"), ambientIntensity);
  GL.glUniform3f(GL.glGetUniformLocation(m_currentShader, "uViewPos"), viewPos.x, viewPos.y, viewPos.z);

  GL.glUniformMatrix4fv(GL.glGetUniformLocation(m_currentShader, "uView"), 1, GL_FALSE, glm::value_ptr(view));
  GL.glUniformMatrix4fv(GL.glGetUniformLocation(m_currentShader, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

  if (g::renderModes.has(g::RenderMode::POINTS)) {
    GL.glEnable(GL_PROGRAM_POINT_SIZE);
    GL.glDisable(GL_DEPTH_TEST);

    for (const auto &gameObject: m_scene->gameObjects) {
      const auto mesh = gameObject->getMesh();
      if (!mesh->is_bound())
        mesh->uploadToGPU();

      glm::mat4 model = gameObject->getModelMatrix();
      GL.glUniformMatrix4fv(GL.glGetUniformLocation(m_currentShader, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

      mesh->render();
    }

    return;
  }

  GL.glEnable(GL_DEPTH_TEST);

  GL.glUniform1i(GL.glGetUniformLocation(m_currentShader, "uAlbedoMap"), 0);

  // draw AABB wireframe in the shader
  GL.glUniform1i(GL.glGetUniformLocation(m_currentShader, "uDrawAABB"), g::renderModes.has(g::RenderMode::DEBUG_AABB));

  // frustum
  glm::mat4 vp = projection * view;
  Frustum frustum{};
  frustum.extract(vp);

  for (auto &gameObject: m_scene->gameObjects) {
    auto mesh = gameObject->getMesh();
    if (!mesh->is_bound())
      mesh->uploadToGPU();

    // culling
    auto [fst, snd] = gameObject->getCachedWorldAABB();
    if (!frustum.isBoxVisible(fst, snd)) {
      continue;
    }

    glm::mat4 model = gameObject->getModelMatrix();
    GL.glUniformMatrix4fv(GL.glGetUniformLocation(m_currentShader, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

    if (g::renderModes.has(g::RenderMode::DEBUG_AABB)) {
      auto [min, max] = gameObject->getCachedWorldAABB();
      mesh->renderAABB(min, max);
      continue;
    }

    for (int face = 0; face < 6; ++face) {
      const auto &mat = mesh->getFaceMaterial(face);

      const glm::vec2 uvScale = mesh->getFaceUVScale(face);
      const glm::vec2 uvOffset = mesh->getFaceUVOffset(face);
      GL.glUniform2f(GL.glGetUniformLocation(m_currentShader, "uUVScale"), uvScale.x, uvScale.y);
      GL.glUniform2f(GL.glGetUniformLocation(m_currentShader, "uUVOffset"), uvOffset.x, uvOffset.y);

      mesh->bind();

      GL.glActiveTexture(GL_TEXTURE0);
      GL.glBindTexture(GL_TEXTURE_2D, mat->getAlbedoTexture());
      mesh->renderFace(face);
    }
  }

  GL.glActiveTexture(GL_TEXTURE0);
}

}

