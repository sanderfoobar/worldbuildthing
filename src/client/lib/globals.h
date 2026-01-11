#pragma once
#include <ranges>
#include <string>
#include <sstream>
#include <map>

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QPixmapCache>
#include <QReadWriteLock>
#include <QOpenGLContext>
#include <QQuickImageProvider>

#include "client/gl2/texture_manager.h"
#include "client/gl2/engine3d.h"
#include "shared/lib/bitflags.h"

class TextureQMLProvider;
class BlenderIconsQRCImageProvider;
class GodotIconsQRCImageProvider;

namespace g {
  Q_NAMESPACE

  enum class EditorMode {
    OBJECT = 0,
    EDIT,
    MATERIAL
  };
  Q_ENUM_NS(EditorMode)

  enum class RenderMode : int {
    PBR              = 1 << 0,
    POINTS           = 1 << 1,
    FULLBRIGHT       = 1 << 2,
    PHONG            = 1 << 3,
    DEBUG_NORMALS    = 1 << 4,
    FAKE_SHADING     = 1 << 5,
    DEBUG_AABB       = 1 << 6,
    DEBUG_CAMERA_FRUSTUM_PROJECTION = 1 << 7
  };
  Q_ENUM_NS(RenderMode)

  extern QOpenGLContext *glcontext;
  extern gl::GLTextureManager* glTextureManager;
  extern gl::Engine3D *engine3D;
  extern Flags<RenderMode> renderModes;

  extern qreal devicePixelRatio;
  extern QReadWriteLock *mtxRender;

  extern GLuint texDefaultWhite;
  extern GLuint texDefaultBlack;
  extern GLuint texDefaultGray;
  extern GLuint texDefaultNormal;

  extern TextureQMLProvider *textureThumbnailQmlProvider;
  extern BlenderIconsQRCImageProvider *iconsBlenderQmlProvider;
  extern GodotIconsQRCImageProvider *iconsGodotQmlProvider;

  struct RenderModeName {
    RenderMode mode;
    const char* name;
  };
  inline const std::array<std::pair<RenderMode, const char*>, 8> renderModesLookup = {{
    { RenderMode::PBR, "PBR" },
    { RenderMode::POINTS, "POINTS" },
    { RenderMode::FULLBRIGHT, "FULLBRIGHT" },
    { RenderMode::PHONG, "PHONG" },
    { RenderMode::DEBUG_NORMALS, "DEBUG_NORMALS" },
    { RenderMode::FAKE_SHADING, "FAKE_SHADING" },
    { RenderMode::DEBUG_AABB, "DEBUG_AABB" },
    { RenderMode::DEBUG_CAMERA_FRUSTUM_PROJECTION, "DEBUG_CAMERA_FRUSTUM_PROJECTION"}
  }};
}
