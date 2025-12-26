#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLWidget>
#include <QTimer>
#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <vector>
#include "../engine/camera.h"
#include "../engine/gameobject.h"
#include "../engine/scene.h"

#include "client/gl/gl_functions.h"

namespace gl {

#define GL (*gl::g_glFunctions)

struct ShaderAmbientLight { // struct size: 16
  //                         // base align  // aligned offset
  QVector4D color;           // 16          // 0
};

struct ShaderDirectionalLight { // struct size: 32
  //                         // base align  // aligned offset
  QVector4D color;           // 16          // 0
  QVector4D direction;       // 16          // 16
};

struct ShaderPointLight { // struct size: 48
  //                           // base align  // aligned offset
  QVector4D color;             // 16          // 0
  QVector4D pos;               // 16          // 16
  QVector4D attenuation;       // 16          // 32
};

struct ShaderSpotLight { // struct size: 80
  //                           // base align  // aligned offset
  QVector4D color;             // 16          // 0
  QVector4D pos;               // 16          // 16
  QVector4D direction;         // 16          // 32
  QVector4D attenuation;       // 16          // 48
  QVector4D cutOff;            // 16          // 64
};

struct ShaderlightInfo { // struct size: 1424
  //                                          // base align  // aligned offset
  int ambientLightNum;                        // 4           // 0
  int directionalLightNum;                    // 4           // 4
  int pointLightNum;                          // 4           // 8
  int spotLightNum;                           // 4           // 12
  ShaderAmbientLight ambientLight[8];         // 16          // 16
  ShaderDirectionalLight directionalLight[8]; // 32          // 144
  ShaderPointLight pointLight[8];             // 48          // 400
  ShaderSpotLight spotLight[8];               // 80          // 784
};


class Engine3D final : public QObject, protected QOpenGLFunctions_4_5_Core {
Q_OBJECT

public:
  explicit Engine3D(QWidget *parent = nullptr);
  ~Engine3D() override;

  std::function<void(int, int)> cb_resize_debounce = nullptr;

  void set_size(const int width, const int height) {
    m_width = width;
    m_height = height;
  }

  // test
  QOpenGLShaderProgram program;
  QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
  QOpenGLVertexArrayObject vao;
  QElapsedTimer timer;

  // void setupSceneWithABunchOfBoxes();
  // void setupSceneFromVMF(std::filesystem::path);

  void set_scene(const QSharedPointer<gl::Scene> &scene) {
    m_scene = scene;
    emit sceneLoaded();
  }

  bool toggleFreeLook();
  void handleKeyPress(QKeyEvent *);
  void handleKeyRelease(QKeyEvent *);
  void printDebugValues() const;

  void paintGL();
  // void paintGL2();
  void cleanup() {}
  void initializeGL();
  void reshape(QRect geometry);

  void doResizeGL(const int _w, const int _h) { return resizeGL(_w, _h); }

signals:
  void initDone();
  void sceneLoaded();

protected:
  void wheelEvent(QWheelEvent *event);
  void generateShadows();
  void renderShadowMap(bool generate);
  void resizeGL(int _w, int _h);

private:
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void calculateSceneBoundsAndLightingAndAlsoPossiblyOtherThings();
  void updateCamera(float deltaTime);
  void performPicking(int mouseX, int mouseY);
  void commitLightInfo() const;
  std::vector<std::pair<int, int>> getSelectedFaces() const;

  QSharedPointer<gl::Scene> m_scene;

  int m_width;
  int m_height;

  // rendering system
  GLuint m_currentShader;
  //
  GLuint m_mainShaderID;
  GLuint m_pointViewShaderID;
  // GLuint m_pbrShaderID;
  // GLuint m_fullBrightID;
  // GLuint m_phongShaderID;
  // GLuint m_debugNormalsShaderID;

  GLuint m_shadowShaderID;
  GLuint m_pickingShaderID;
  GLuint m_pickingFBO;  // Framebuffer for picking
  GLuint m_pickingTexture;

  // viewport
  qreal m_devicePixelRatio;

  // frame timing
  float m_deltaTime = 0.0f;
  std::chrono::high_resolution_clock::time_point m_lastFrameTime;

  // MSAA
  int m_msaaSamples = 0;
  GLuint m_msaaFBO = 0;
  GLuint m_msaaColorBuffer = 0;
  GLuint m_msaaDepthRBO = 0;
  void setAASamples(int samples);
  void createMSAAFBO();

  // lighting
  glm::vec3 m_lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
  // shadow map
  GLuint m_shadowFBO = 0;
  GLuint m_shadowMap = 0;
  const int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
  glm::mat4 m_lightSpaceMatrix;
  std::function<void()> cb_generate_shadows = nullptr;
  bool m_generate_shadows = false;

  // shadow variables
  float m_shadowLightDistance = 100.0f;
  float m_shadowNearPlane = 0.1f;
  float m_shadowFarPlane = 600.0f;
  bool m_shadowUseYUp = false;   // Z-up world
  float m_shadowOrthoScale = 1.2f;
  glm::vec3 m_sun_position{FLT_MAX};
  glm::vec3 m_sun_direction{FLT_MAX};
  float m_shadowBias = 0.01f;

  // AABB
  glm::vec3 m_scene_minBounds{FLT_MAX};
  glm::vec3 m_scene_maxBounds{-FLT_MAX};
  glm::vec3 m_sceneCenter{FLT_MAX};

  // input tracking
  bool m_keysPressed[256];
  int m_lastMouseX = 0;
  int m_lastMouseY = 0;
  bool m_firstMouse = true;
  bool m_freeLookEnabled = false;  // toggle with 'Z' key
  bool m_mouseCaptured = false;  // tracks if mouse is currently captured
  bool m_rightMouseDown = false;  // track right mouse button for mouse look
  std::set<uint32_t> m_selectedPickingIDs;  // track selected materials by picking ID
  QPoint m_mouseReleaseRestorePos;

  // camera
  std::unique_ptr<Camera> m_camera;
  float m_cameraSpeed = 50.0f;

  GLuint m_whiteTex = 0;
  GLuint m_blackTex = 0;
  GLuint m_grayTex = 0;
  GLuint m_defaultNormalTex = 0;

  // picking
  GLuint m_pickingDepthRBO = 0; // depth renderbuffer FBO

  static OpenGLUniformBufferObject *m_cameraInfo, *m_lightInfo;

  QVector<QObject*> m_gizmoMeshes, m_gridlineMeshes, m_lightMeshes, m_normalMeshes;
public:
  glm::vec3 shadowLightDir() const { return m_sun_direction; }
  void setShadowLightDir(const glm::vec3 &dir) { m_sun_direction = glm::normalize(dir); generateShadows(); }

  float shadowLightDistanceMultiplier() const { return m_shadowLightDistance; }
  void setShadowLightDistanceMultiplier(float d) { m_shadowLightDistance = d; generateShadows(); }

  float shadowNearPlane() const { return m_shadowNearPlane; }
  void setShadowNearPlane(float n) { m_shadowNearPlane = n; generateShadows(); }

  float shadowFarPlane() const { return m_shadowFarPlane; }
  void setShadowFarPlane(float f) { m_shadowFarPlane = f; generateShadows(); }

  float shadowOrthoScale() const { return m_shadowOrthoScale; }
  void setShadowOrthoScale(float s) { m_shadowOrthoScale = s; generateShadows(); }

  bool shadowUseYUp() const { return m_shadowUseYUp; }
  void setShadowUseYUp(bool b) { m_shadowUseYUp = b; generateShadows(); }

  glm::vec3 sunPosition() const { return m_sun_position; }
  void setSunPosition(glm::vec3 pos) { m_sun_position = pos; generateShadows(); }

  float shadowBias() const { return m_shadowBias * 100.0f; }
  void setShadowBias(float v) {
    float _v = 0;
    if (v != 0)
      _v = v / 100;
    m_shadowBias = _v;
    generateShadows();
  }

private:
  bool m_init = false;
  bool isInit() const {return m_init;}
};
}