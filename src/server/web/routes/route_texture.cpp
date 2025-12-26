#include <QHttpServerResponse>
#include <QtConcurrent>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "server/ctx.h"
#include "server/globals.h"
#include "server/web/routes/route_texture.h"
#include "server/web/routes/route_utils.h"
#include "server/web/web_sessionstore.h"

#include "shared/models/texture_getters_setters.h"
#include "shared/models/texture_image_getters_setters.h"
#include "shared/lib/file_packer.h"
#include "shared/lib/utils.h"

namespace AssetPackTextureRoute {

void install(QHttpServer *server) {
  server->route("/api/1/textures", QHttpServerRequest::Method::Get, [](const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([&request] {
      if (g::web_requires_auth) {
        const auto current_user = g::webSessions->get_user(request);
        if (current_user.isNull())
          return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
      }

      rapidjson::Document root;
      root.SetObject();
      auto& allocator = root.GetAllocator();

      rapidjson::Value meta(rapidjson::kObjectType);

      rapidjson::Value tags_arr(rapidjson::kArrayType);
      auto tags_global = gs::textureTagManager->tags();
      for (const auto &tag : tags_global) {
        auto tag_name = tag->name().toStdString();
        rapidjson::Value k;
        k.SetString(tag_name.c_str(),
                    static_cast<rapidjson::SizeType>(tag_name.size()),
                    allocator);
        tags_arr.PushBack(k, allocator);
      }

      root.AddMember("tags", tags_arr, allocator);

      for (const auto &tex: gs::TEXTURES_FLAT) {
        rapidjson::Value item(rapidjson::kObjectType);
        item.AddMember("name", rapidjson::Value(tex->name.toUtf8().constData(), allocator), allocator);
        item.AddMember("license", rapidjson::Value(tex->get_license().toUtf8().constData(), allocator), allocator);
        item.AddMember("author", rapidjson::Value(tex->get_author().toUtf8().constData(), allocator), allocator);

        rapidjson::Value tex_tags_rapidjson(rapidjson::kArrayType);
        auto tex_tags = tex->get_tags();
        for (const auto &tag : tex_tags) {
          auto tag_idx = tags_global.indexOf(tag);
          tex_tags_rapidjson.PushBack(
            rapidjson::Value().SetInt(static_cast<int>(tag_idx)),
            allocator
          );
        }

        item.AddMember("tags", tex_tags_rapidjson, allocator);
        item.AddMember("name_lower", rapidjson::Value(tex->name_lower.toUtf8().constData(), allocator), allocator);
        item.AddMember("num_pixels", rapidjson::Value().SetInt(static_cast<int>(tex->num_pixels)), allocator);

        auto packImages = [](const QMap<TextureSize, QSharedPointer<TextureImage>> &src,
            rapidjson::Document::AllocatorType& alloc) {
          rapidjson::Value out(rapidjson::kObjectType);
          for (auto it = src.begin(); it != src.end(); ++it)
            if (it.value())
              out.AddMember(
                rapidjson::Value(TEXSIZE2STR[it.key()].toUtf8().constData(), alloc),
                it.value()->to_rapidjson(alloc),
                alloc
              );
          return out;
        };

        rapidjson::Value images(rapidjson::kObjectType);

        if (!tex->diffuse.isEmpty())
          images.AddMember("diffuse", packImages(tex->diffuse, allocator), allocator);
        if (!tex->normal.isEmpty())
          images.AddMember("normal", packImages(tex->normal, allocator), allocator);
        if (!tex->roughness.isEmpty())
          images.AddMember("roughness", packImages(tex->roughness, allocator), allocator);
        if (!tex->metalness.isEmpty())
          images.AddMember("metalness", packImages(tex->metalness, allocator), allocator);
        if (!tex->ao.isEmpty())
          images.AddMember("ao", packImages(tex->ao, allocator), allocator);
        if (!tex->arm.isEmpty())
          images.AddMember("arm", packImages(tex->arm, allocator), allocator);
        if (!tex->displacement.isEmpty())
          images.AddMember("displacement", packImages(tex->displacement, allocator), allocator);
        if (!tex->opacity.isEmpty())
          images.AddMember("opacity", packImages(tex->opacity, allocator), allocator);
        if (!tex->emission.isEmpty())
          images.AddMember("emission", packImages(tex->emission, allocator), allocator);
        if (!tex->specular.isEmpty())
          images.AddMember("specular", packImages(tex->specular, allocator), allocator);
        if (!tex->scattering.isEmpty())
          images.AddMember("scattering", packImages(tex->scattering, allocator), allocator);

        auto key = tex->name.toStdString();
        rapidjson::Value k;
        k.SetString(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), allocator);
        item.AddMember("images", images, allocator);
        meta.AddMember(k, item, allocator);
      }

      root.AddMember("meta", meta, allocator);

      // serialize
      rapidjson::StringBuffer buffer;
      rapidjson::Writer writer(buffer);
      root.Accept(writer);

      QByteArray jsonData(buffer.GetString(), static_cast<int>(buffer.GetSize()));
      return QHttpServerResponse("application/json", jsonData, QHttpServerResponder::StatusCode::Ok);
    });
    return future;
  });

  // server->route("/api/1/textures", QHttpServerRequest::Method::Get, [](const QHttpServerRequest &request) {
  //   QFuture<QHttpServerResponse> future = QtConcurrent::run([&request] {
  //     const auto tex_name = tname.toLower();
  //     if (g::web_requires_auth) {
  //       const auto current_user = g::webSessions->get_user(request);
  //       if (current_user.isNull())
  //         return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
  //     }
  //
  //     const auto ap = g::ctx->assetPackManager->by_name(pname);
  //     if (ap.isNull())
  //       return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     if (!ap->textures_lower.contains(tex_name))
  //       return QHttpServerResponse("application/json", "texture not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     const auto tex = ap->textures_lower[tex_name];
  //     const auto thumb = tex->path_thumbnail();
  //     const auto fp = thumb.absoluteFilePath();
  //     const auto data = Utils::fileOpen(fp);
  //
  //     QHttpHeaders headers;
  //     headers.insert(0, "Content-Disposition", QString("attachment; filename=\"%1\"").arg(thumb.fileName()));
  //
  //     qDebug() << "sending len" << data.length();
  //     QHttpServerResponse response("application/octet-stream", data);
  //     response.setHeaders(std::move(headers));
  //     return response;
  //
  //   });
  //   return future;
  // });

  // server->route("/api/1/ap/<arg>/texture/<arg>/<arg>/<arg>", QHttpServerRequest::Method::Get, [](
  //     const QString& pname, const QString& tname, const QString &ttype, const QString &tsize, const QHttpServerRequest &request) {
  //   QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, tname, ttype, tsize, &request] {
  //     const auto tex_name = tname.toLower();
  //     if (g::web_requires_auth) {
  //       const auto current_user = g::webSessions->get_user(request);
  //       if (current_user.isNull())
  //         return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
  //     }
  //
  //     const auto ap = g::ctx->assetPackManager->by_name(pname);
  //     if (ap.isNull())
  //       return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     if (!ap->textures_lower.contains(tex_name))
  //       return QHttpServerResponse("application/json", "texture not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     const TextureImageType _ttype = getTextureImageType(ttype.toLower());
  //     if (_ttype == TextureImageType::unknown)
  //       return QHttpServerResponse("application/json", "unknown texture type",
  //                                  QHttpServerResponder::StatusCode::NotFound);
  //
  //     const TextureSize _tsize = getTextureSize(tsize.toLower());
  //     if (_tsize == TextureSize::null)
  //       return QHttpServerResponse("application/json", "unknown texture size", QHttpServerResponder::StatusCode::NotFound);
  //
  //     const auto tex = ap->textures_lower[tex_name];
  //
  //     const auto tex_img = tex->get_image(_ttype, _tsize);
  //     if (tex_img.isNull())
  //       return QHttpServerResponse("application/json", "texture image not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     const auto img = tex_img->path_get(_ttype);
  //     qDebug() << img.absoluteFilePath();
  //     const auto fp = img.absoluteFilePath();
  //     const auto data = Utils::fileOpen(fp);
  //
  //     QHttpHeaders headers;
  //     headers.insert(0, "Content-Disposition", QString("attachment; filename=\"%1\"").arg(img.fileName()));
  //
  //     qDebug() << "sending len" << data.length();
  //     QHttpServerResponse response("application/octet-stream", data);
  //     response.setHeaders(std::move(headers));
  //     return response;
  //
  //   });
  //   return future;
  // });
  //
  // // @TODO: refuse to access when asset pack is busy (pack->status)
  // server->route("/api/1/ap/<arg>/texture/<arg>/<arg>/pack", [](const QString &pname, const QString &tname, const QString &image_size, const QHttpServerRequest &request) {
  //   QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, tname, image_size, &request] {
  //     const QUrlQuery query(request.url());
  //
  //     if (!STR2TEXSIZE.contains(image_size))
  //       return QHttpServerResponse("wrong image_size", QHttpServerResponder::StatusCode::NotFound);
  //     auto image_size_enum = STR2TEXSIZE[image_size];
  //
  //     const auto pack = g::ctx->assetPackManager->by_name(pname);
  //     if (pack.isNull())
  //       return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     if (pack->atype != AssetPackType::assetPackTexture)
  //       return QHttpServerResponse("application/json", "asset pack has the wrong pack type", QHttpServerResponder::StatusCode::NotFound);
  //
  //     qDebug() << "packing" << tname;
  //     QString err_msg;
  //
  //     if (pack.isNull()) {
  //       err_msg = QString("asset_pack '%1' not found").arg(pname);
  //       qWarning() << err_msg;
  //       return QHttpServerResponse(err_msg, QHttpServerResponder::StatusCode::NotFound);
  //     }
  //
  //     if (!pack->textures_lower.contains(tname.toLower())) {
  //       err_msg = QString("texture by name not found: %s").arg(tname.toLower());
  //       qWarning() << err_msg;
  //       return QHttpServerResponse(err_msg, QHttpServerResponder::StatusCode::NotFound);
  //     }
  //
  //     const auto tex = pack->textures_lower[tname.toLower()];
  //     // @TODO: mat properties?
  //     // auto properties = tex->
  //
  //     QStringList paths;
  //     for (int i = 0; i < static_cast<int>(TextureImageType::textureImageTypeCount); ++i) {
  //       const auto ttype = static_cast<TextureImageType>(i);
  //       if (ttype == TextureImageType::unknown)
  //         continue;
  //
  //       if (auto img = tex->get_image(ttype, image_size_enum); !img.isNull())
  //         paths << img->path.absoluteFilePath();
  //     }
  //
  //     if (paths.isEmpty()) {
  //       err_msg = QString("no suitable paths found");
  //       qWarning() << err_msg;
  //       return QHttpServerResponse(err_msg, QHttpServerResponder::StatusCode::NotFound);
  //     }
  //
  //     // const auto tres = tex->to_tres(image_size_enum);
  //     // const QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
  //     // const QString baseName = tex->name_lower + ".tres";
  //     // const QString filePath = tempDir + "/" + baseName;
  //     // qDebug() << "writing" << filePath;
  //     //
  //     // QFile file(filePath);
  //     // if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
  //     //   QTextStream out(&file);
  //     //   out << tres;
  //     //   file.close();
  //     // }
  //
  //     qDebug() << "packing";
  //     // paths << filePath;
  //     qDebug() << paths;
  //     const QByteArray data = packFiles(paths);
  //     // QFile::remove(filePath);
  //
  //     QHttpHeaders headers;
  //     headers.insert(0, "Content-Disposition", QString("attachment; filename=\"%1\"").arg(tex->name + ".bin"));
  //
  //     qDebug() << "sending len" << data.length();
  //     QHttpServerResponse response("application/octet-stream", data);
  //     response.setHeaders(std::move(headers));
  //     return response;
  //   });
  //
  //   return future;
  // });
}

}
