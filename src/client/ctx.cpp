#include <QDir>
#include <QObject>
#include <filesystem>
#include <QStandardPaths>
#include <QPixmapCache>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions>

#include "shared/lib/seaquel.h"
#include "shared/lib/utils.h"
#include "shared/models/asset_loader.h"
#include "shared/models/texture_manager.h"

#include "ctx.h"
#include "gl/gl_functions.h"
#include "shared/models/texture_model.h"
#include "shared/models/texture_proxy_model.h"

using namespace std::chrono;

QPixmap TextureQMLProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
  if (id.isEmpty()) {
    qCritical() << "TextureQMLProvider requested id is empty";
    return {};
  }

  const auto baseDir = gs::programMode == ProgramMode::server ? gs::cacheDirectory + QDir::separator()
                                                              : gs::cacheDirectoryTextures + QDir::separator();
  const auto path = baseDir + id;

  QPixmap pixmap;
  if (!QPixmapCache::find(id, &pixmap)) {
    if (QFile::exists(path)) {
      pixmap.load(path);
      if (!requestedSize.isEmpty())
        pixmap = pixmap.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      QPixmapCache::insert(id, pixmap);
    }
  } else if (!requestedSize.isEmpty()) {
    pixmap = pixmap.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  if (size)
    *size = pixmap.size();

  return pixmap;
}

Ctx::Ctx() {
  gs::programMode = ProgramMode::client;
  g::renderModes.set(g::RenderMode::PHONG, g::RenderMode::FAKE_SHADING);
  g_instance = this;
  g::glTextureManager = new gl::GLTextureManager;
  gs::textureTagManager = new TextureTagManager(this);
  gs::textureManager = new TextureManager(this);
  gs::textureModel = new TextureModel(this);
  gs::textureProxyModel = new TextureProxyModel(this);
  g::textureThumbnailQmlProvider = new TextureQMLProvider();

  gs::configRoot = QDir::homePath();
  gs::homeDir = QDir::homePath();
  gs::configDirectory = QString("%1/.config/%2/").arg(gs::configRoot, QCoreApplication::applicationName());
  gs::configDirectoryAutoMasker = QString("%1%2").arg(gs::configDirectory, "autoMasker");
  gs::configDirectoryAssets = QString("%1%2").arg(gs::configDirectory, "data");
  gs::configDirectoryAssetsTextures = QString("%1/%2").arg(gs::configDirectoryAssets, "textures");
  gs::configDirectoryAssetsModels = QString("%1/%2").arg(gs::configDirectoryAssets, "models");
  gs::configDirectoryAssetsU2net = QString("%1/%2").arg(gs::configDirectoryAssets, "u2net");
  gs::cacheDirectory = QString("%1/cache").arg(gs::configDirectory);
  gs::cacheDirectoryTextures = QString("/tmp/test2/").arg(gs::configDirectory);

  gs::pathDatabase = QFileInfo(gs::configDirectory + QDir::separator() + "db.sqlite3");
  qDebug() << "path database:" << gs::pathDatabase;
  qDebug() << "cache database:" << gs::cacheDirectory;

  createConfigDirectory(QStringList({
    gs::configDirectory,
    gs::cacheDirectory,
    gs::cacheDirectoryTextures,
    gs::configDirectoryAutoMasker,
    gs::configDirectoryAssets,
    gs::configDirectoryAssetsModels,
    gs::configDirectoryAssetsU2net
  }));

  unpackAppArtifacts();

  asset_loader::load_from_network().then([] {
    gs::textureModel->refresh();
  });

  if(!SQL::initialize(m_path_db.absoluteFilePath()))
    throw QString("Cannot open db at %1").arg(m_path_db.absoluteFilePath()).toStdString();

  //  imageAutoMasker = new ImageAutoMasker(this);
  //  imageAutoMasker->detect(
  //    "/home/dsc/CLionProjects/godot/texture_browser/src/tools/test.jpg");
  //    "/home/dsc/CLionProjects/godot/texture_browser/src/tools/out.png");

  pathTextureDirectory = config()->get(ConfigKeys::PathDirectoryTexture).toString();
}

void Ctx::unpackAppArtifacts() {}

void Ctx::createConfigDirectory(const QStringList &lst) {
  for(const auto &d: lst) {
    if(!std::filesystem::exists(d.toStdString())) {
      qDebug() << QString("Creating directory: %1").arg(d);
      if(!QDir().mkpath(d))
        throw std::runtime_error("Could not create directory " + d.toStdString());
    }
  }
}

void Ctx::onApplicationLog(const QString &msg) {
}

Ctx::~Ctx() {}

void Ctx::singleShot(int msec, QJSValue callback) const {
  QTimer::singleShot(msec, this, [callback] () mutable {
    if (callback.isCallable())
      callback.call();
  });
}

Ctx* g_instance = nullptr;