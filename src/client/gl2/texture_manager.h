#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QTimer>
#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <vector>

#include "client/gl2/material.h"

namespace gl {
  class GLTextureManager final : public QObject {
  public:
    GLTextureManager();
    static GLuint load_texture_from_memory(const QByteArray& data);
    void uploadTexture(QSharedPointer<TextureImage> img);
    QSharedPointer<Material> get_by_name(const QString &name);
    void init();
  private:
    bool m_initialized = false;
    QMap<QString, QSharedPointer<Material>> m_entries_by_name = {};
    QMap<QString, QSharedPointer<Material>> m_entries_by_idx = {};
  };
}