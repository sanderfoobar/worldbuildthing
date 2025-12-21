#include <filesystem>

#include <QObject>
#include <QDir>
#include <QStandardPaths>

#include "ctx.h"
#include "lib/utils.h"
//#include "lib/vmfpp/vmfpp.h"

#include <chrono>

#include "globals.h"
#include "lib/file_packer.h"
#include "lib/stb/stb.h"
#include "lib/stb/stb_image.h"
#include "lib/vtf/vtf.h"
#include "lib/materialpropertiestemplate.h"
#include "lib/zlib_compressor.h"

using namespace std::chrono;

Ctx::Ctx() {
  g_instance = this;
  Utils::init();
  init_material_templates();

  stbi_set_flip_vertically_on_load(true);

  assetPackManager = new AssetPackManager(this);
  config()->set(ConfigKeys::CacheDir, gs::cacheDirectory);

#ifdef GENERATE_VTF
  gs::FUNC_GENERATE_VMT_VTF_FILES = vtf::generate_vmt_vtf_files;
#endif
  // globals::FUNC_GENERATE_STB_FILES = stb::generate_stb_files;

  createConfigDirectory(QStringList({
    gs::configDirectory,
    gs::cacheDirectory,
    gs::configDirectoryAutoMasker,
    gs::configDirectoryAssets,
    gs::configDirectoryAssetsTextures,
    gs::configDirectoryAssetsU2net,
    gs::configDirectoryAssetsModels}));
  unpackAppArtifacts();

  if(!SQL::initialize(gs::pathDatabase.absoluteFilePath()))
    throw QString("Cannot open db at %1").arg(gs::pathDatabase.absoluteFilePath()).toStdString();

  // web server
  start_webserver();

  QString err;
  assetPackManager->load(gs::configDirectoryAssetsTextures, err);
  assetPackManager->scan();

  connect(assetPackManager, &AssetPackManager::scanProgress, [=](const int pct) {
    // qDebug() << "Scan progress" << pct;
      if (pct == 100) {
        // initGodotResourceTemplates();
      }
  });

  g::ctx = this;
}

void Ctx::unpackAppArtifacts() {
  // Python auto-masker
  auto dest_auto_masker = gs::configDirectory;
  auto files_auto_masker = {
    ":/tools/masker/masker.sh",
    ":/tools/masker/masker.py"
  };

  for(const auto &fp: files_auto_masker) {
    QFile f(fp);
    QFileInfo fileInfo(f);

    auto to_path = QString("%1/%2").arg(gs::configDirectoryAutoMasker, fileInfo.fileName());
    qDebug() << "writing" << to_path;
    f.copy(to_path);
    f.setPermissions(QFile::ExeUser);
    f.close();
  }
}

void Ctx::createConfigDirectory(const QStringList &lst) {
  for(const auto &d: lst) {
    if(!std::filesystem::exists(d.toStdString())) {
      qDebug() << QString("Creating directory: %1").arg(d);
      if(!QDir().mkpath(d))
        throw std::runtime_error("Could not create directory " + d.toStdString());
    }
  }
}

//void Ctx::automask(){
//  imageAutoMasker = new ImageAutoMasker(this);
//  //  imageAutoMasker->detect(
//  //    "/path/to/src/tools/test.jpg");
//  //    "/path/to/src/tools/out.png");
//}

void Ctx::onApplicationLog(const QString &msg) {
}

void Ctx::start_webserver() {
  m_web_thread = new QThread();
  m_web_thread->setObjectName(QString("webserver"));
  web_server = new WebServer();
  web_server->setHost("0.0.0.0");
  web_server->setPort(3000);
  web_server->moveToThread(m_web_thread);

  connect(m_web_thread, &QThread::started, web_server, [this]() {
    if (!web_server->start()) {
      qWarning() << "Failed to start server";
      QCoreApplication::quit();
    } else {
      qInfo() << "Webserver started";
    }
  });

  connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, [this] {
    web_server->stop();
    m_web_thread->quit();
    m_web_thread->wait();
    web_server->deleteLater();
    m_web_thread->deleteLater();
  });

  m_web_thread->start();
}

Ctx::~Ctx() {}

Ctx* g_instance = nullptr;