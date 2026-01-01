#pragma once

#include <QObject>
#include <QCoro/QCoroNetwork>
#include <QCoro/QCoroTask>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace asset_loader {
  QCoro::Task<> load_thumbnail_from_network(QStringList thumbnail_filenames);
  QCoro::Task<> load_from_network();
  void from_disk();

  void unpack_files(const QString &binFilePath);
}