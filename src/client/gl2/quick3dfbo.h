#pragma once
#include <QQuickFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>
// #include <QOpenGLFunctions>

namespace gl {

  class QuickFBO : public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(QVariant renderer READ renderer WRITE setRenderer)

    public:
      Renderer *createRenderer() const;
      QVariant renderer() const { return m_rendererRef; };
      void setRenderer(QVariant rendererRef);
    private:
      QVariant m_rendererRef;
  };

}