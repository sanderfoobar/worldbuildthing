#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <random>

#include "client/lib/globals.h"
#include "engine3d.h"

namespace gl {

void Engine3D::calculateSceneBoundsAndLightingAndAlsoPossiblyOtherThings() {
  // calc scene bounds
  glm::vec3 minb{FLT_MAX};
  glm::vec3 maxb{-FLT_MAX};

  for (const auto &obj : m_scene->gameObjects) {
    auto [objMin, objMax] = obj->getCachedWorldAABB();  // returns min and max of object in world space
    minb = glm::min(minb, objMin);
    maxb = glm::max(maxb, objMax);
  }

  m_scene_minBounds = minb;
  m_scene_maxBounds = maxb;

  m_sceneCenter = (m_scene_minBounds + m_scene_maxBounds) * 0.5f;

  // directional light, pointing at the scene center
  const float sun_angle = 45.0f;
  const float angle = glm::radians(sun_angle);
  m_sun_direction = glm::normalize(glm::vec3(0.0f, -sin(angle), -cos(angle)));

  const glm::vec3 sceneCenter = (m_scene_minBounds + m_scene_maxBounds) * 0.5f;
  m_sun_position = sceneCenter - m_sun_direction * m_shadowLightDistance;
  m_sun_position.x = -80;
}

bool Engine3D::toggleFreeLook() {
  // toggle free look mode with mouse capture
  m_freeLookEnabled = !m_freeLookEnabled;
  m_firstMouse = true; // Reset mouse when toggling
  if (m_freeLookEnabled) {
    std::cerr << "Free look mode ENABLED (Z to disable) - Mouse captured" << std::endl;
    m_mouseCaptured = true;
    return true;
  } else {
    std::cerr << "Free look mode DISABLED" << std::endl;
    m_mouseCaptured = false;
    return false;
  }
}

void Engine3D::handleKeyPress(QKeyEvent *_event) {
  if (_event->isAutoRepeat())
    return;

  int key = _event->key();

  switch (key) {
    case Qt::Key_W:
      m_keysPressed['W'] = m_keysPressed['w'] = true;
    break;
    case Qt::Key_S:
      m_keysPressed['S'] = m_keysPressed['s'] = true;
    break;
    case Qt::Key_A:
      m_keysPressed['A'] = m_keysPressed['a'] = true;
    break;
    case Qt::Key_D:
      m_keysPressed['D'] = m_keysPressed['d'] = true;
    break;
    case Qt::Key_Q:
      m_keysPressed['Q'] = m_keysPressed['q'] = true;
    break;
    case Qt::Key_E:
      m_keysPressed['E'] = m_keysPressed['e'] = true;
    break;
    default:
      break;
  }
}

void Engine3D::handleKeyRelease(QKeyEvent *_event) {
  if (_event->isAutoRepeat())
    return;

  int key = _event->key();

  switch (key) {
    case Qt::Key_W:
      m_keysPressed['W'] = m_keysPressed['w'] = false;
    break;
    case Qt::Key_S:
      m_keysPressed['S'] = m_keysPressed['s'] = false;
    break;
    case Qt::Key_A:
      m_keysPressed['A'] = m_keysPressed['a'] = false;
    break;
    case Qt::Key_D:
      m_keysPressed['D'] = m_keysPressed['d'] = false;
    break;
    case Qt::Key_Q:
      m_keysPressed['Q'] = m_keysPressed['q'] = false;
    break;
    case Qt::Key_E:
      m_keysPressed['E'] = m_keysPressed['e'] = false;
    break;
    default:
      break;
  }
}

void Engine3D::mouseMoveEvent(QMouseEvent *_event) {
  // int xpos = _event->position().x();
  // int ypos = _event->position().y();
  //
  // if (m_rightMouseDown) {
  //   int centerX = width() / 2;
  //   int centerY = height() / 2;
  //   if (m_firstMouse) {
  //     QPoint GL.globalCenter = mapToGlobal(QPoint(centerX, centerY));
  //     QCursor::setPos(globalCenter);
  //     m_firstMouse = false;
  //     return;
  //   }
  //   float xoffset = xpos - centerX;
  //   float yoffset = centerY - ypos;
  //
  //   // invert X offset to fix horizontal inversion
  //   m_camera->rotate(-xoffset, yoffset);
  //
  //   QPoint GL.globalCenter = mapToGlobal(QPoint(centerX, centerY));
  //   QCursor::setPos(globalCenter);
  //   return;
  // }
  //
  // if (m_freeLookEnabled) {
  //   if (m_firstMouse) {
  //     m_lastMouseX = xpos;
  //     m_lastMouseY = ypos;
  //     m_firstMouse = false;
  //     return;
  //   }
  //   float xoffset = xpos - m_lastMouseX;
  //   float yoffset = m_lastMouseY - ypos;
  //
  //   // invert X offset to fix horizontal inversion
  //   m_camera->rotate(-xoffset, yoffset);
  //
  //   m_lastMouseX = xpos;
  //   m_lastMouseY = ypos;
  //
  //   int centerX = width() / 2;
  //   int centerY = height() / 2;
  //   int margin = 50;
  //   bool shouldWrap = false;
  //   if (xpos < margin || xpos > width() - margin) shouldWrap = true;
  //   if (ypos < margin || ypos > height() - margin) shouldWrap = true;
  //   if (shouldWrap) {
  //     QPoint GL.globalPos = mapToGlobal(QPoint(centerX, centerY));
  //     QCursor::setPos(globalPos);
  //     m_lastMouseX = centerX;
  //     m_lastMouseY = centerY;
  //   }
  // }
}

void Engine3D::mouseReleaseEvent(QMouseEvent *_event) {
  if (_event->button() == Qt::RightButton) {
    m_rightMouseDown = false;
    // unsetCursor();

    QCursor::setPos(m_mouseReleaseRestorePos);

  }
}

void Engine3D::mousePressEvent(QMouseEvent *_event) {
  // if (_event->button() == Qt::LeftButton) {
  //   // left-click for picking
  //   int xpos = _event->position().x();
  //   int ypos = _event->position().y();
  //   performPicking(xpos, ypos);
  // }
  // if (_event->button() == Qt::RightButton) {
  //   m_rightMouseDown = true;
  //   m_firstMouse = true;
  //   setCursor(Qt::BlankCursor);
  //   // store position to restore after release
  //   m_mouseReleaseRestorePos = QCursor::pos();
  //   m_lastMouseX = width() / 2;
  //   m_lastMouseY = height() / 2;
  //   // warp mouse to center of widget
  //   QPoint center = mapToGlobal(QPoint(width() / 2, height() / 2));
  //   QCursor::setPos(center);
  // }
}

void Engine3D::updateCamera(float deltaTime) {
  if (m_keysPressed['W'] || m_keysPressed['w'])
    m_camera->moveForward(deltaTime, m_cameraSpeed);
  if (m_keysPressed['S'] || m_keysPressed['s'])
    m_camera->moveBackward(deltaTime, m_cameraSpeed);
  if (m_keysPressed['A'] || m_keysPressed['a'])
    m_camera->moveLeft(deltaTime, m_cameraSpeed);
  if (m_keysPressed['D'] || m_keysPressed['d'])
    m_camera->moveRight(deltaTime, m_cameraSpeed);
  if (m_keysPressed['E'] || m_keysPressed['e'])
    m_camera->moveUp(deltaTime, m_cameraSpeed);
  if (m_keysPressed['Q'] || m_keysPressed['q'])
    m_camera->moveDown(deltaTime, m_cameraSpeed);
}

void Engine3D::wheelEvent(QWheelEvent *event) {
  // increase or decrease camera speed
  int delta = event->angleDelta().y();
  if (delta > 0)
    m_cameraSpeed += 10.0f;
  else if (delta < 0)
    m_cameraSpeed = std::max(1.0f, m_cameraSpeed - 10.0f);
  std::cerr << "Camera speed: " << m_cameraSpeed << std::endl;
}

Engine3D::~Engine3D() {
  GL.glDeleteProgram(m_mainShaderID);
  GL.glDeleteProgram(m_pointViewShaderID);
  GL.glDeleteProgram(m_pickingShaderID);
  GL.glDeleteFramebuffers(1, &m_pickingFBO);
  GL.glDeleteTextures(1, &m_pickingTexture);

  delete m_lightInfo;
}

static ShaderlightInfo shaderlightInfo;

void Engine3D::commitLightInfo() const {
  int ambientLightNum = 0, directionalLightNum = 0, pointLightNum = 0, spotLightNum = 0;

  for (int i = 0; i < m_scene->directionalLights().size(); i++) {
    if (m_scene->directionalLights()[i]->enabled()) {
      shaderlightInfo.directionalLight[directionalLightNum].color = m_scene->directionalLights()[i]->color().toVector4D() * m_scene->directionalLights()[i]->intensity();
      shaderlightInfo.directionalLight[directionalLightNum].direction = m_scene->directionalLights()[i]->direction().toVector4D();
      directionalLightNum++;
    }
  }

  shaderlightInfo.ambientLightNum = ambientLightNum;
  shaderlightInfo.directionalLightNum = directionalLightNum;
  shaderlightInfo.pointLightNum = pointLightNum;
  shaderlightInfo.spotLightNum = spotLightNum;

  if (m_lightInfo == 0) {
    m_lightInfo = new OpenGLUniformBufferObject;
    m_lightInfo->create();
    m_lightInfo->bind();
    m_lightInfo->allocate(3, NULL, sizeof(ShaderlightInfo));
    m_lightInfo->release();
  }
  m_lightInfo->bind();
  m_lightInfo->write(0, &shaderlightInfo, sizeof(ShaderlightInfo));
  m_lightInfo->release();
}

void Engine3D::reshape(const QRect geometry) {
  resizeGL(geometry.width(), geometry.height());
}

void Engine3D::setAASamples(const int samples) {
  m_msaaSamples = std::max(0, samples);
  //  createMSAAFBO();
}

void Engine3D::printDebugValues() const {
  qDebug() << "SCENE====";
  qDebug() << "minBounds: " << m_scene_minBounds.x << m_scene_minBounds.y << m_scene_minBounds.z;
  qDebug() << "maxBounds: " << m_scene_maxBounds.x << m_scene_maxBounds.y << m_scene_maxBounds.z;
  qDebug() << "sceneCenter:" << m_sceneCenter.x << m_sceneCenter.y << m_sceneCenter.z;
  qDebug() << "m_sun_position:" << m_sun_position.x << m_sun_position.y << m_sun_position.z;
  qDebug() << "m_sun_direction:" << m_sun_direction.x << m_sun_direction.y << m_sun_direction.z;
  qDebug() << "m_shadowNearPlane:" << m_shadowNearPlane;
  qDebug() << "m_shadowFarPlane:" << m_shadowFarPlane;
  qDebug() << "sun_angle:" << 45;
  qDebug() << "m_shadowOrthoScale:" << m_shadowOrthoScale;
  qDebug() << "m_shadowLightDistance" << m_shadowLightDistance;
  qDebug() << "m_shadowBias" << m_shadowBias;
}

void Engine3D::resizeGL(const int _w, const int _h) {
  // m_width = _w * m_devicePixelRatio;
  // m_height = _h * m_devicePixelRatio;
  m_width = _w;
  m_height = _h;
  GL.glViewport(0, 0, _w, _h);

  // resize picking texture
  if (m_pickingTexture != 0) {
    GL.glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
    GL.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  }

  // resize picking depth renderbuffer
  if (m_pickingDepthRBO != 0) {
    GL.glBindRenderbuffer(GL_RENDERBUFFER, m_pickingDepthRBO);
    GL.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_width, m_height);
  }
}

std::vector<std::pair<int, int>> Engine3D::getSelectedFaces() const {
  std::vector<std::pair<int, int>> result;
  for (uint32_t pickingID: m_selectedPickingIDs) {
    int cubeIdx = pickingID / 6;
    int faceIdx = pickingID % 6;
    result.push_back({cubeIdx, faceIdx});
  }
  return result;
}
}


