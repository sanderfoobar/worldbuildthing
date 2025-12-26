#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLWidget>
#include <QSharedPointer>
#include <QTimer>
#include <glm/glm.hpp>
#include <vector>
#include "engine/scene.h"

namespace gl {

class GLWidget2DGrid final : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
Q_OBJECT
signals:
  void initDone();
  void sceneLoaded();

public:
  enum class ViewType { Top, Left, Right };

  explicit GLWidget2DGrid(ViewType type = ViewType::Top, QWidget *parent = nullptr);
  ~GLWidget2DGrid() override;

  void setScene(const QSharedPointer<gl::Scene> &scene) {
    m_scene = scene;
  }

  // expose controls
  void zoomIn();
  void zoomOut();
  void setGridIndex(int idx); // choose index in the gridSizes list
  int gridIndex() const { return m_gridIndex; }
  ViewType viewType() const { return m_viewType; }

  void setupSceneFromVMF(std::filesystem::path path) {
    m_scene = Scene::VMF(path);
    rebuildObjectsGeometry();
  }

signals:
  void requestedUpdate();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void onFrameTimer();

private:
  void rebuildGridGeometry();
  void rebuildObjectsGeometry();
  glm::vec2 worldToView(const glm::vec3 &pos) const;
  void adjustZoomAtScreenPos(float factor, const QPoint &screenPos);

  QSharedPointer<Scene> m_scene;
  ViewType m_viewType;

  // GL objects
  GLuint m_progLines = 0;
  GLint m_uProj = -1;
  GLint m_uColor = -1;

  GLuint m_gridVAO = 0;
  GLuint m_gridVBO = 0;
  GLsizei m_gridVertices = 0;

  GLuint m_objsVAO = 0;
  GLuint m_objsVBO = 0;
  GLsizei m_objsVertices = 0;

  // view control
  float m_zoom = 1.0f;
  glm::vec2 m_pan{0.0f, 0.0f}; // world offset in view units (applied after zoom)
  QPoint m_lastMouse;
  bool m_panning = false;

  // grid sizes sequence
  std::vector<float> m_gridSizes{};
  int m_gridIndex = 0;

  // appearance
  glm::vec4 m_gridColor{0.7f,0.7f,0.7f,1.0f};
  glm::vec4 m_axisColor{0.8f,0.3f,0.3f,1.0f};
  glm::vec4 m_objColor{0.0f,1.0f,0.0f,1.0f};

  QTimer *m_updateTimer = nullptr;

  float m_gridExtent = 500.0f;
  float m_gridScale = 1.0f;

private:
  bool m_init = false;
  bool isInit() const {return m_init;}
};

}