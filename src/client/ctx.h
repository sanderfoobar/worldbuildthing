#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJSValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMainWindow>
#include <QMessageBox>
#include <QMutex>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QQueue>
#include <QSqlDatabase>
#include <QThread>
#include <QTimer>

#include "client/engine/scene.h"
#include "client/lib/globals.h"
#include "shared/lib/config.h"

class TextureQMLProvider final : public QQuickImageProvider {
public:
  explicit TextureQMLProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}
  QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

class Ctx;
extern Ctx* g_instance;

class Ctx final : public QObject, protected QOpenGLExtraFunctions {
Q_OBJECT

public:
  explicit Ctx();
  ~Ctx() override;

  QList<QSharedPointer<gl::Scene>> scene;
  QSharedPointer<gl::Scene> getCurrentScene() {
    return scene[m_sceneIdx];
  }

  bool is_debug;
  QString preloadModel;
  QString configDirectory;
  QString configDirectoryAutoMasker;
  QString configDirectoryAssets;
  QString configDirectoryAssetsTextures;
  QString configDirectoryAssetsModels;
  QString configDirectoryAssetsU2net;
  QString cacheDirectory;
  QString pathTextureDirectory;

  QString configRoot;
  QString homeDir;
  void readInfoJSON(QString path);

  static Ctx* instance() {
    return g_instance;
  }

  Q_INVOKABLE void singleShot(int msec, QJSValue callback) const;

signals:
  void applicationLog(const QString &msg);
  void windowTitle(QString title);
  void GLContextAvailable();
  void texturesUpdated();

private slots:
  void onApplicationLog(const QString &msg);

private:
  QFileInfo m_path_db;
  QSqlDatabase m_db;
  unsigned int m_sceneIdx = 0;

  void unpackAppArtifacts();
  static void createConfigDirectory(const QStringList &lst);
};
