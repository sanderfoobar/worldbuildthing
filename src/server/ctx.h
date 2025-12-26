#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QTimer>
#include <QDebug>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QDir>

#if defined(Q_OS_LINUX) and defined(PLUGIN_AUTOMASK)
#include "lib/image_auto_masker.h"
#endif

#include "server/web/web_webserver.h"
#include "shared/lib/seaquel.h"

#include "shared/models/tags.h"
#include "shared/models/texture.h"
#include "shared/models/texture_image.h"
#include "shared/models/treemodel.h"

class Ctx;
extern Ctx* g_instance;

class Ctx final : public QObject {
Q_OBJECT

public:
  explicit Ctx();
  void start_webserver();
  ~Ctx() override;

  bool is_debug;
  QString preloadModel;

  WebServer *web_server = nullptr;
  // ImageAutoMasker *imageAutoMasker;
  // TextureModel *textureModel;
  // TextureProxyModel *textureProxyModel;
  // TextureQMLProvider *textureQmlProvider;
  // TreeModel *treez;

  QString configRoot;
  QString homeDir;
  void readInfoJSON(QString path);

  static Ctx* instance() {
    return g_instance;
  }

signals:
  void applicationLog(const QString &msg);
  void windowTitle(QString title);

private slots:
  void onApplicationLog(const QString &msg);

private:
  QSqlDatabase m_db;
  QThread* m_web_thread = nullptr;

  void unpackAppArtifacts();
  static void createConfigDirectory(const QStringList &lst);
};
