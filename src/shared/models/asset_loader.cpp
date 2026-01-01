#include <QTemporaryFile>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>

#include "asset_loader.h"

#include "shared/lib/globals.h"
#include "shared/lib/utils.h"
#include "texture.h"
#include "texture_getters_setters.h"
#include "texture_image.h"
#include "texture_image_getters_setters.h"
#include "texture_manager.h"

namespace asset_loader {
  void unpack_files(const QString &binFilePath) {
    QString baseDir = gs::cacheDirectoryTextures + QDir::separator();
    QDir().mkpath(baseDir);

    QFile file(binFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
      qCritical() << "Failed to open bin file:" << binFilePath;
      return;
    }

    // Read the JSON header (first line)
    QByteArray headerLine = file.readLine();
    if (headerLine.isEmpty() || headerLine[0] != '[') {
      qCritical() << "Bad bin file, missing JSON header";
      return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(headerLine, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
      qCritical() << "Failed to parse JSON header:" << parseError.errorString();
      return;
    }

    QJsonArray metadataArray = doc.array();

    // fileDataSection starts after the first line
    qint64 dataSectionOffset = headerLine.size();

    for (const QJsonValue &val : metadataArray) {
      if (!val.isObject())
        continue;

      QJsonObject obj = val.toObject();
      QString fileName = obj.value("fileName").toString();
      qint64 offset = obj.value("offset").toVariant().toLongLong();
      qint64 size = obj.value("fileSize").toVariant().toLongLong();

      if (!file.seek(dataSectionOffset + offset)) {
        qCritical() << "Failed to seek to file offset:" << fileName;
        continue;
      }

      QByteArray fileData = file.read(size);
      if (fileData.size() != size) {
        qCritical() << "Unexpected read size for file:" << fileName;
        continue;
      }

      QString outPath = QDir(baseDir).filePath(fileName);
      QDir().mkpath(QFileInfo(outPath).path());

      QFile outFile(outPath);
      if (!outFile.open(QIODevice::WriteOnly)) {
        qCritical() << "failed to write file:" << outPath;
        continue;
      }

      outFile.write(fileData);
      outFile.close();
      qDebug() << "wrote file:" << outPath;
    }

    file.close();
  }

  QCoro::Task<> load_thumbnail_from_network(QStringList thumbnail_filenames) {
    QNetworkAccessManager networkAccessManager;

    const QUrl url("http://127.0.0.1:3000/api/1/texture/thumbnails_pack");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonArray filenamesArray;
    for (const QString &f: thumbnail_filenames)
      filenamesArray.append(f);

    const QJsonObject payload{{"filenames", filenamesArray}};
    const QByteArray json_data = QJsonDocument(payload).toJson();

    const std::unique_ptr<QNetworkReply> reply(co_await networkAccessManager.post(request, json_data));
    if (reply->error()) {
      qCritical() << "Network request failed:" << reply->errorString();
      co_return;
    }

    qDebug() << "Network request successful, data = reply->readAll()";
    const QByteArray data = reply->readAll();
    qDebug() << "tmpFile";

    QTemporaryFile tmpFile;
    if (!tmpFile.open()) {
      qCritical() << "Failed to open temporary file";
      co_return;
    }

    tmpFile.write(data);
    tmpFile.flush();
    tmpFile.seek(0);
    qDebug() << "Data written to temporary file:" << tmpFile.fileName();

    unpack_files(tmpFile.fileName());
  }

  QCoro::Task<> load_from_network() {
    QNetworkAccessManager networkAccessManager;
    const QString url = "http://127.0.0.1:3000/api/1/textures";
    std::unique_ptr<QNetworkReply> reply(co_await networkAccessManager.get(QNetworkRequest{url}));
    if (reply->error()) {
      auto msg = QString("Error occurred during network request. Error code: %1").arg(reply->error());
      qCritical() << msg;
      co_return;
    }

    const QByteArray data = reply->readAll();

    // parse
    QVector<AssetPackListHTTPResult> packs;
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
      qCritical() << "Error occurred during network request. data_ap #1";
      co_return;
    }

    gs::textureManager->clear();

    const QJsonObject obj = doc.object();
    const QJsonObject meta = obj.value("meta").toObject();

    QList<QSharedPointer<TextureTag>> texture_tags;
    for (const auto& tag: obj.value("tags").toArray()) {
      auto ptr = gs::textureTagManager->get_or_insert(tag.toString());
      texture_tags << ptr;
    }

    for (auto it = meta.begin(); it != meta.end(); ++it) {
      QString key = it.key();
      QJsonObject tex_blob = it.value().toObject();

      auto tex_name = tex_blob.value("name").toString();
      auto tex_author = tex_blob.value("author").toString();
      auto tex_license = tex_blob.value("license").toString();
      auto tex_thumbnail = tex_blob.value("thumbnail").toString();

      auto tex = QSharedPointer<Texture>(new Texture(tex_name));
      tex->set_author(tex_author);
      tex->set_license(tex_license);

      QJsonArray tags_array = tex_blob.value("tags").toArray();
      QSet<QSharedPointer<TextureTag>> tex_tags;
      for (const auto& tag: tags_array) {
        auto tag_idx = tag.toInt();
        if (tag_idx != -1 && tag_idx < texture_tags.size()) {
          QSharedPointer<TextureTag> ptr = texture_tags.at(tag_idx);
          tex_tags << ptr;
        }
      }

      if (!tex_thumbnail.isEmpty())
        tex->set_thumbnail_name(tex_thumbnail);
      tex->set_tags(tex_tags);

      auto tex_images = tex_blob.value("images").toObject();
      for (auto it_ = tex_images.begin(); it_ != tex_images.end(); ++it_) {
        const QString key = it_.key();
        const QJsonObject size_map = it_.value().toObject();

        for (auto size_it = size_map.begin(); size_it != size_map.end(); ++size_it) {
          auto tex_obj = size_it.value().toObject();
          const QString name = tex_obj.value("name").toString();
          const QString name_original = tex_obj.value("name_original").toString();
          const QString filename = tex_obj.value("filename").toString();
          const int channels = tex_obj.value("channels").toInt();
          const QString checksum = tex_obj.value("checksum").toString();

          TexImgInfo texinfo(name_original);
          const TexImgExt ext = filename.endsWith(".png", Qt::CaseInsensitive) ? TexImgExt::PNG : TexImgExt::JPG;

          auto tex_img = QSharedPointer<TextureImage>::create(texinfo, ext);
          tex_img->set_channels(channels);
          tex_img->set_checksum(checksum);
          tex->set_texture(tex_img);
        }
      }

      gs::textureManager->add(tex);
    }

    // ensure we have all the thumbnails
    QStringList missing_thumbnails;
    for (const auto& tex: gs::textureManager->all()) {
      const auto thumb_path = tex->path_thumbnail();
      if (!thumb_path.exists())
        missing_thumbnails << thumb_path.fileName();
    }
    if (!missing_thumbnails.isEmpty())
      asset_loader::load_thumbnail_from_network(missing_thumbnails);
  }

  void from_disk() {
    const QString scan_dir = gs::configDirectoryAssetsTextures;

    if (!Utils::dirExists(scan_dir)) {
      qCritical() << QString("%1 does not exist").arg(scan_dir);
      return;
    }

    QSet<QSharedPointer<Texture>> textures;

    const QDir base_dir(scan_dir);
    QList<QDir> dirs;
    for (const QFileInfo &info : base_dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
      dirs.append(QDir(info.absoluteFilePath()));

    const QRegularExpression rx(".*.png|.*.jpg");

    for (const QDir &dir_ : dirs) {
      auto aa = dir_.absolutePath();
      QList<QFileInfo> paths = Utils::fileFind(rx, aa, 0, 0, 6000);
      QList<QFileInfo> paths_colors;

      for (const QFileInfo &info: paths) {
        const auto _ = info.baseName();
        if (_.endsWith("Color"))
          paths_colors << info;
      }

      for (const auto &path_file: paths_colors) {
        auto texinfo = TexImgInfo(path_file.baseName());
        auto tex = QSharedPointer<Texture>(new Texture(texinfo.name));

        const TexImgExt ext = path_file.suffix() == "png" ? TexImgExt::PNG : TexImgExt::JPG;
        auto tex_img = QSharedPointer<TextureImage>(new TextureImage(texinfo, ext));
        tex->set_texture(tex_img);
        tex_img->set_path(path_file);
        tex_img->inspect_checksum();
        tex_img->inspect_channels_and_dimensions();

        for (const auto &file : paths) {
          auto _ = file.baseName();
          if (_.startsWith(texinfo.name) && !_.endsWith("Color")) {
            auto _texinfo = TexImgInfo(file.baseName());
            const TexImgExt _ext = file.suffix() == "png" ? TexImgExt::PNG : TexImgExt::JPG;
            auto tex_p = QSharedPointer<TextureImage>(new TextureImage(_texinfo, _ext));
            tex->set_texture(tex_p);
            tex_p->set_path(path_file);
            tex_p->inspect_checksum();
            tex_p->inspect_channels_and_dimensions();
          }
        }

        auto path_meta = dir_.absolutePath() + QDir::separator() + "meta.json";
        if (Utils::fileExists(path_meta)) {
          QFile f(path_meta);
          if(!f.open(QIODevice::ReadOnly))
            return;

          QByteArray data = f.readAll();
          f.close();

          QJsonParseError err;
          QJsonDocument doc = QJsonDocument::fromJson(data, &err);
          if(err.error != QJsonParseError::NoError)
            return;

          auto meta_obj = doc.object();
          auto meta = meta_obj.value("meta").toObject();
          QList<QSharedPointer<TextureTag>> tags;

          for (const auto &m: meta_obj.value("tags").toArray())
            tags << gs::textureTagManager->get_or_insert(m.toString());

          for (const auto& tex_ : textures) {
            if (meta.contains(tex_->name)) {
              auto tex_meta = meta[tex_->name].toObject();
              auto tex_tags = tex_meta.value("tags").toArray();
              auto tex_license = tex_meta.value("license").toString();
              auto tex_author = tex_meta.value("author").toString();

              QSet<QSharedPointer<TextureTag>> tex_tags_ptrs;
              for (const auto &tag : tex_tags) {
                if (!tag.isDouble())
                  continue;

                int idx = tag.toInt(-1);
                if (idx < 0 || idx >= tags.size())
                  continue;

                tex_tags_ptrs << tags.at(idx);
              }

              tex_->set_license(tex_license);
              tex_->set_author(tex_author);
              tex_->set_tags(tex_tags_ptrs);
            }
          }
        }

        textures << tex;
      }
    }

    for (const auto& tex : textures) {
      gs::textureManager->add(tex);
    }
  }
}