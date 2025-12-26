#include "client/lib/globals.h"

namespace g {
  qreal devicePixelRatio;
  QReadWriteLock *mtxRender = nullptr;
  QOpenGLContext *glcontext = nullptr;
  gl::Engine3D *engine3D = nullptr;
  // RenderMode renderMode = RenderMode::PHONG;
  Flags<RenderMode> renderModes;
  gl::GLTextureManager *glTextureManager = nullptr;
  GLuint texDefaultWhite = 0;
  GLuint texDefaultBlack = 0;
  GLuint texDefaultGray = 0;
  GLuint texDefaultNormal = 0;

  // std::function<bool(const QFileInfo&, const QFileInfo&, const QFileInfo&, const QString&, const QString&)> FUNC_GENERATE_VMT_VTF_FILES = nullptr;
  // std::function<bool(void*)> FUNC_GENERATE_VMT_VTF_FILES = nullptr;
}
