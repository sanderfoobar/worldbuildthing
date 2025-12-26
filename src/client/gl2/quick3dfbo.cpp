#include <QQuickFramebufferObject>
#include <QTimer>

#include "client//lib/globals.h"
#include "gl/gl_functions.h"
#include "shared/lib/globals.h"

#include "quick3dfbo.h"
#include "shared/lib/bitflags.h"

namespace gl {
  class QuickFBORenderer : public QQuickFramebufferObject::Renderer {
  public:
    QuickFBORenderer() : m_size(0, 0) {
      setup_tasks.set(
        EngineSetupTasks::CONSTRUCTOR,
        EngineSetupTasks::INIT,
        EngineSetupTasks::SET_SIZE);
    }

    ~QuickFBORenderer() {
      if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
      }
    }

    void render() {
      if (setup_tasks.empty()) {
        m_renderer->paintGL();
        update();
        return;
      }

      if (setup_tasks.has(EngineSetupTasks::INIT) && !setup_tasks.has(EngineSetupTasks::SET_SIZE)) {
        m_renderer->initializeGL();
        setup_tasks.clear(EngineSetupTasks::INIT);
      }

      update();
    }

    void synchronize(QQuickFramebufferObject *item) {
      const auto pyFbo = static_cast<QuickFBO *>(item);

      if (setup_tasks.has(EngineSetupTasks::CONSTRUCTOR)) {
        m_renderer = new Engine3D();
        g::engine3D = m_renderer;
        setup_tasks.clear(EngineSetupTasks::CONSTRUCTOR);
        return;
      }

      if (setup_tasks.empty() && m_sizeChanged) {
        m_renderer->reshape(QRect(QPoint(0, 0), QSize(item->width(), item->height())));
        m_sizeChanged = false;
      }
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {
      m_size = size;
      m_sizeChanged = true;
      QOpenGLFramebufferObjectFormat format;
      format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

      if (setup_tasks.has(EngineSetupTasks::SET_SIZE)) {
        m_renderer->set_size(size.width(), size.height());
        setup_tasks.clear(EngineSetupTasks::SET_SIZE);
      }

      m_init = true;
      m_sizeChanged = true;

      return new QOpenGLFramebufferObject(size, format);
    }

  public:
    enum class EngineSetupTasks : int {
      CONSTRUCTOR  = 1 << 0,
      INIT         = 1 << 1,
      SET_SIZE     = 1 << 2
    };
  private:
    QVariant m_rendererRef;
    Engine3D *m_renderer = nullptr;
    QSize m_size;
    bool m_sizeChanged = false;
    bool m_init = false;

    Flags<EngineSetupTasks> setup_tasks;
  };

  void QuickFBO::setRenderer(QVariant rendererRef) {
    if (rendererRef == m_rendererRef)
      return;
    m_rendererRef = rendererRef;
    update();
  }

  QQuickFramebufferObject::Renderer *QuickFBO::createRenderer() const {
    return new QuickFBORenderer();
  }

} // namespace gl
