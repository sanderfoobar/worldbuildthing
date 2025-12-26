#include "../gl2/engine3d.h"

using namespace gl;

void Engine3D::performPicking(int mouseX, int mouseY) {
  mouseX *= m_devicePixelRatio;
  mouseY *= m_devicePixelRatio;

  // initialize OpenGL functions if not already done
  initializeOpenGLFunctions();

  // clamp coordinates to widget bounds
  if (mouseX < 0 || mouseX >= m_width || mouseY < 0 || mouseY >= m_height) {
    std::cerr << "DEBUG: Click outside widget bounds (" << mouseX << ", " << mouseY
        << ") vs (" << m_width << ", " << m_height << ")" << std::endl;
    return;
  }

  std::cerr << "DEBUG: Picking at screen coords (" << mouseX << ", " << mouseY << ")" << std::endl;

  // render scene to picking framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, m_pickingFBO);

  // verify FBO is complete
  GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "ERROR: Picking FBO not complete! Status: " << fboStatus << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  }

  glViewport(0, 0, m_width, m_height);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black with zero alpha (no geometry)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  if (m_pickingShaderID == 0) {
    std::cerr << "Picking shader not compiled!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  }

  glUseProgram(m_pickingShaderID);

  // setup view and projection matrices
  glm::mat4 view = m_camera->getViewMatrix();
  glm::mat4 projection = m_camera->getProjectionMatrix((float) m_width / (float) m_height);

  GLint viewLoc = glGetUniformLocation(m_pickingShaderID, "uView");
  GLint projLoc = glGetUniformLocation(m_pickingShaderID, "uProjection");
  GLint modelLoc = glGetUniformLocation(m_pickingShaderID, "uModel");
  GLint pickingIDLoc = glGetUniformLocation(m_pickingShaderID, "uPickingID");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // render each face with its picking ID
  for (auto &gameObject: m_scene->gameObjects) {
    glm::mat4 model = gameObject->getModelMatrixCached();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    auto mesh = gameObject->getMesh();
    for (int face = 0; face < 6; ++face) {
      const auto &material = mesh->getFaceMaterial(face);
      uint32_t pickingID = material->getPickingID();

      // Encode picking ID as normalized RGBA
      unsigned char r = (pickingID & 0xFF);
      unsigned char g = ((pickingID >> 8) & 0xFF);
      unsigned char b = ((pickingID >> 16) & 0xFF);
      unsigned char a = ((pickingID >> 24) & 0xFF);

      glUniform1ui(pickingIDLoc, pickingID);
      mesh->renderFaceForPicking(face);
    }
  }

  // read pixel at mouse position before unbinding
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  unsigned char pixel[4] = {0, 0, 0, 0};
  glReadPixels(mouseX, m_height - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

  std::cerr << "DEBUG: Read pixels at (" << mouseX << ", " << (m_height - mouseY) << "): RGBA=("
      << (int) pixel[0] << ", " << (int) pixel[1] << ", " << (int) pixel[2] << ", " << (int) pixel[3] << ")" <<
      std::endl;

  // restore framebuffer binding to default (screen)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // decode picking ID from RGBA
  uint32_t pickingID = (uint32_t) pixel[0] | ((uint32_t) pixel[1] << 8) |
                       ((uint32_t) pixel[2] << 16) | ((uint32_t) pixel[3] << 24);

  std::cerr << "DEBUG: Picked pixel RGBA: (" << (int) pixel[0] << ", " << (int) pixel[1] << ", "
      << (int) pixel[2] << ", " << (int) pixel[3] << "), Decoded ID: " << pickingID << std::endl;

  if (pickingID > 0) {
    int cubeIdx = pickingID / 6;
    int faceIdx = pickingID % 6;
    // Toggle selection
    if (m_selectedPickingIDs.count(pickingID)) {
      m_selectedPickingIDs.erase(pickingID);
      std::cerr << "DESELECTED: Cube " << cubeIdx << ", Face " << faceIdx << ", Picking ID: " << pickingID << std::endl;
    } else {
      m_selectedPickingIDs.insert(pickingID);
      std::cerr << "SELECTED: Cube " << cubeIdx << ", Face " << faceIdx << ", Picking ID: " << pickingID << std::endl;
    }
    std::cerr << "Total selected faces: " << m_selectedPickingIDs.size() << std::endl;
    if (cubeIdx < (int) m_scene->gameObjects.size()) {
      auto mesh = m_scene->gameObjects[cubeIdx]->getMesh();
      const auto &mat = mesh->getFaceMaterial(faceIdx);
      std::cerr << "Picked: Cube " << cubeIdx << ", Face " << faceIdx
          << ", Picking ID: " << pickingID
          // << ", Texture ID: " << mat->nam()
          // << ", Color: (" << mat.getColor().x << ", " << mat.getColor().y << ", " << mat.getColor().z << ")"
          << std::endl;
    }
  } else {
    std::cerr << "No geometry picked" << std::endl;
  }
}