#include <QDir>
#include <QObject>
#include <filesystem>
#include <QStandardPaths>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions>

#include "shared/lib/seaquel.h"
#include "shared/lib/utils.h"
#include "shared/models/asset_loader.h"

#include "ctx.h"
#include "gl/gl_functions.h"

using namespace std::chrono;

Ctx::Ctx() {
  g_instance = this;
  g::glTextureManager = new gl::GLTextureManager;
  gs::textureTagManager = new TextureTagManager(this);
  g::renderModes.set(g::RenderMode::PHONG, g::RenderMode::FAKE_SHADING);

  gs::configRoot = QDir::homePath();
  gs::homeDir = QDir::homePath();
  gs::configDirectory = QString("%1/.config/%2/").arg(gs::configRoot, QCoreApplication::applicationName());
  gs::configDirectoryAutoMasker = QString("%1%2").arg(gs::configDirectory, "autoMasker");
  gs::configDirectoryAssets = QString("%1%2").arg(gs::configDirectory, "data");
  gs::configDirectoryAssetsTextures = QString("%1/%2").arg(gs::configDirectoryAssets, "textures");
  gs::configDirectoryAssetsModels = QString("%1/%2").arg(gs::configDirectoryAssets, "models");
  gs::configDirectoryAssetsU2net = QString("%1/%2").arg(gs::configDirectoryAssets, "u2net");
  gs::cacheDirectory = QString("%1/cache").arg(gs::configDirectory);
  gs::pathDatabase = QFileInfo(gs::configDirectory + QDir::separator() + "db.sqlite3");
  qDebug() << "path database:" << gs::pathDatabase;
  qDebug() << "cache database:" << gs::cacheDirectory;

  createConfigDirectory(QStringList({
    gs::configDirectory,
    gs::cacheDirectory,
    gs::configDirectoryAutoMasker,
    gs::configDirectoryAssets,
    gs::configDirectoryAssetsModels,
    gs::configDirectoryAssetsU2net
  }));

  unpackAppArtifacts();

  asset_loader::load_from_network();

  if(!SQL::initialize(m_path_db.absoluteFilePath()))
    throw QString("Cannot open db at %1").arg(m_path_db.absoluteFilePath()).toStdString();

  //  imageAutoMasker = new ImageAutoMasker(this);
  //  imageAutoMasker->detect(
  //    "/home/dsc/CLionProjects/godot/texture_browser/src/tools/test.jpg");
  //    "/home/dsc/CLionProjects/godot/texture_browser/src/tools/out.png");

  pathTextureDirectory = config()->get(ConfigKeys::PathDirectoryTexture).toString();

//  textureModel = new TextureModel();
//  textureModel->setAssetManager(assetPackManager);
//  textureModel->onRefresh();
//  connect(assetPackManager, &AssetPackManager::scanFinished, textureModel, &TextureModel::onRefresh);
//
//  textureQmlProvider = new TextureQMLProvider(textureModel);
//  textureProxyModel = new TextureProxyModel();
//  textureProxyModel->setSourceModel(textureModel);
//
//  treez = new TreeModel();
//  auto x = vmfpp::VMF::openFile("/media/dsc/0376C0A40D1AE4C9/source_maps/cutscene_test.vmf");
//  auto y = 1337;

  // auto a = QOpenGLContext::currentContext();
  // auto *glFuncs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(a);
  // glFuncs->initializeOpenGLFunctions();
  // initializeOpenGLFunctions();
  // gl::g_glFunctions->initializeOpenGLFunctions();

  int wegw = 1;
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