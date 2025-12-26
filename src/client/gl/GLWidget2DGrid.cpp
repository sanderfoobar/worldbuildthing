#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <algorithm>

#include "GLWidget2DGrid.h"

#include "gl_functions.h"

using namespace gl;

static const std::string linesVertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 inPos;
uniform mat3 uProj; // 3x3 2D affine projection (NDC)
void main() {
  vec3 p = uProj * vec3(inPos, 1.0);
  gl_Position = vec4(p.xy, 0.0, 1.0);
}
)";

static const std::string linesFragmentShaderSrc = R"(
#version 330 core
out vec4 oColor;
uniform vec4 uColor;
void main(){
  oColor = uColor;
}
)";

GLWidget2DGrid::GLWidget2DGrid(const ViewType type, QWidget *parent)
  : QOpenGLWidget(parent), m_viewType(type) {
  setFocusPolicy(Qt::StrongFocus);
  m_updateTimer = new QTimer(this);
  connect(m_updateTimer, &QTimer::timeout, this, &GLWidget2DGrid::onFrameTimer);
  m_updateTimer->start(1000/60);

  m_gridSizes.clear();
  float val = 1.0f;
  while (val <= 512.0f) {
    m_gridSizes.push_back(val / 10.0f);
    val *= 2.0f;
  }

  m_gridIndex = 2; // start from middle zoom
  m_gridExtent = m_gridSizes[m_gridIndex];
  m_zoom = 1.0f / m_gridExtent; // initial zoom so that one cell roughly matches extent
  m_gridScale = 3.0;

  connect(this, &GLWidget2DGrid::sceneLoaded, [this] {
    rebuildObjectsGeometry();
    update();
  });
}

void GLWidget2DGrid::rebuildGridGeometry() {
  if (!isValid()) return;
  makeCurrent();

  std::vector<glm::vec2> verts;
  float extent = 3276.8f;
  int numLines = int(extent / m_gridScale);

  for (int i = -numLines; i <= numLines; ++i) {
    float x = i * m_gridScale;
    verts.emplace_back(x, -extent);
    verts.emplace_back(x,  extent);

    float y = i * m_gridScale;
    verts.emplace_back(-extent, y);
    verts.emplace_back( extent, y);
  }

  m_gridVertices = GLsizei(verts.size());

  glBindVertexArray(m_gridVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec2), verts.data(), GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glBindVertexArray(0);

  doneCurrent();
}

void GLWidget2DGrid::onFrameTimer() {
  update();
}

GLWidget2DGrid::~GLWidget2DGrid() {
  makeCurrent();
  if (m_gridVBO) glDeleteBuffers(1, &m_gridVBO);
  if (m_gridVAO) glDeleteVertexArrays(1, &m_gridVAO);
  if (m_objsVBO) glDeleteBuffers(1, &m_objsVBO);
  if (m_objsVAO) glDeleteVertexArrays(1, &m_objsVAO);
  if (m_progLines) glDeleteProgram(m_progLines);
  doneCurrent();
}

void GLWidget2DGrid::initializeGL() {
  initializeOpenGLFunctions();
  gl::g_glFunctions = this;

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenVertexArrays(1, &m_gridVAO);
  glGenBuffers(1, &m_gridVBO);

  glGenVertexArrays(1, &m_objsVAO);
  glGenBuffers(1, &m_objsVBO);

  m_progLines = loadShaderProgram(linesVertexShaderSrc, linesFragmentShaderSrc);
  m_uProj = glGetUniformLocation(m_progLines, "uProj");
  m_uColor = glGetUniformLocation(m_progLines, "uColor");

  rebuildGridGeometry();
  rebuildObjectsGeometry();

  m_init = true;
  emit initDone();
}

void GLWidget2DGrid::resizeGL(int w, int h) {
  glViewport(0,0,w,h);
  rebuildGridGeometry();
  rebuildObjectsGeometry();
}

glm::vec2 GLWidget2DGrid::worldToView(const glm::vec3 &pos) const {
  glm::vec2 p;
  switch (m_viewType) {
    case ViewType::Top:
      p = glm::vec2(pos.x, pos.y);
      break;
    case ViewType::Left:
      p = glm::vec2(pos.z, pos.y);
      break;
    case ViewType::Right:
      p = glm::vec2(-pos.z, pos.y);
      break;
  }
  p = (p + m_pan) * m_zoom;
  return p;
}


void GLWidget2DGrid::rebuildObjectsGeometry() {
  if (!isValid()) return;
  makeCurrent();

  std::vector<glm::vec2> verts;
  int i = 0;
  if (!m_scene.isNull()) {
    for (auto &obj : m_scene->gameObjects) {
      const auto mesh = obj->getMesh();
      if (!mesh) continue;

      glm::vec3 pos = obj->getPosition();
      glm::vec3 scale = obj->getScale();

      for (const auto &v : mesh->vertices()) {
        glm::vec3 p = v.position;
        p *= scale;

        glm::vec2 p2d;
        switch (m_viewType) {
          case ViewType::Top:   p2d = glm::vec2(p.x, p.y); break;
          case ViewType::Left:  p2d = glm::vec2(p.z, p.y); break;
          case ViewType::Right: p2d = glm::vec2(-p.z, p.y); break;
        }

        p2d += glm::vec2(pos.x, pos.y);
        verts.push_back(p2d);

        qDebug() << "=======" << QString::number(i);
        qDebug() << "x, y" << p2d.x << p2d.y;
        i += 1;
      }
    }
  }

  m_objsVertices = GLsizei(verts.size());

  glBindVertexArray(m_objsVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_objsVBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec2), verts.data(), GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glBindVertexArray(0);

  doneCurrent();
}

void GLWidget2DGrid::paintGL() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  if (!m_progLines) return;

  int w = width();
  int h = height();

  glm::mat3 proj(1.0f);
  proj[0][0] = m_zoom / (w * 0.5f);
  proj[1][1] = m_zoom / (h * 0.5f);
  proj[2][0] = -m_pan.x * proj[0][0];
  proj[2][1] = -m_pan.y * proj[1][1];

  glUseProgram(m_progLines);
  glUniformMatrix3fv(m_uProj, 1, GL_FALSE, glm::value_ptr(proj));

  glBindVertexArray(m_gridVAO);
  glLineWidth(1.0f);
  glUniform4fv(m_uColor, 1, glm::value_ptr(m_gridColor));
  glDrawArrays(GL_LINES, 0, m_gridVertices);
  glBindVertexArray(0);

  // objects line width
  glBindVertexArray(m_objsVAO);
  glLineWidth(20.0f);
  glUniform4fv(m_uColor, 1, glm::value_ptr(m_objColor));
  glDrawArrays(GL_LINES, 0, m_objsVertices);
  glBindVertexArray(0);

  glUseProgram(0);
}

void GLWidget2DGrid::wheelEvent(QWheelEvent *event) {
  float delta = float(event->angleDelta().y()) / 120.0f;
  if (delta == 0) {
    event->accept();
    return;
  }

  float factor = (delta > 0) ? 1.125f : 0.8888889f;
  adjustZoomAtScreenPos(factor, event->position().toPoint());

  rebuildGridGeometry(); // only grid needs to update
  update();
  event->accept();
}

void GLWidget2DGrid::adjustZoomAtScreenPos(float factor, const QPoint &screenPos) {
  glm::vec2 sp(float(screenPos.x()) - float(width())*0.5f, float(screenPos.y()) - float(height())*0.5f);
  sp.y = -sp.y;
  glm::vec2 before = (sp / m_zoom) - m_pan;
  m_zoom *= factor;
  glm::vec2 after = (sp / m_zoom) - m_pan;
  m_pan += (after - before);
}

void GLWidget2DGrid::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::MiddleButton) {
    m_panning = true;
    m_lastMouse = event->pos();
  }
  event->accept();
}

void GLWidget2DGrid::mouseMoveEvent(QMouseEvent *event) {
  qDebug() << m_zoom;
  if (m_panning) {
    QPoint p = event->position().toPoint();
    QPoint d = p - m_lastMouse;
    m_lastMouse = p;
    glm::vec2 delta(float(d.x()), -float(d.y()));
    delta /= m_zoom;
    m_pan += delta;
    rebuildGridGeometry();
    rebuildObjectsGeometry();
    update();
  }
  event->accept();
}

void GLWidget2DGrid::setGridIndex(int idx) {
  if (idx < 0) idx = 0;
  if (idx >= int(m_gridSizes.size())) idx = int(m_gridSizes.size()) - 1;
  m_gridIndex = idx;

  m_gridScale = m_gridSizes[m_gridIndex]; // only affects grid
  rebuildGridGeometry();
  update();
}

void GLWidget2DGrid::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_BracketLeft) {
    qDebug() << "[" << m_gridIndex;
    qDebug() << "zoom" << m_zoom;
    setGridIndex(m_gridIndex - 1);
    event->accept();
    return;
  }
  if (event->key() == Qt::Key_BracketRight) {
    qDebug() << "]" << m_gridIndex;
    qDebug() << "zoom" << m_zoom;
    setGridIndex(m_gridIndex + 1);
    event->accept();
    return;
  }
  QWidget::keyPressEvent(event);
}
