#include <QHttpServerResponse>
#include <QtConcurrent>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "web/routes/route_assetpack_texture.h"
#include "web/routes/route_utils.h"
#include "web/web_sessionstore.h"

#include "ctx.h"
#include "globals.h"
#include "models/texture_getters_setters.h"
#include "models/texture_image_getters_setters.h"
#include "lib/file_packer.h"
#include "lib/utils.h"

namespace AssetPackTextureRoute {

void install(QHttpServer *server) {
  // http://127.0.0.1:3000/api/1/ap/polyhaven_1k/texture/BrushedConcrete001/thumbnail
  server->route("/api/1/ap/<arg>/texture/<arg>/thumbnail", QHttpServerRequest::Method::Get, [](const QString& pname, const QString& tname, const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, tname, &request] {
      const auto tex_name = tname.toLower();
      if (g::web_requires_auth) {
        const auto current_user = g::webSessions->get_user(request);
        if (current_user.isNull())
          return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
      }

      const auto ap = g::ctx->assetPackManager->by_name(pname);
      if (ap.isNull())
        return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);

      if (!ap->textures_lower.contains(tex_name))
        return QHttpServerResponse("application/json", "texture not found", QHttpServerResponder::StatusCode::NotFound);

      const auto tex = ap->textures_lower[tex_name];
      const auto thumb = tex->path_thumbnail();
      const auto fp = thumb.absoluteFilePath();
      const auto data = Utils::fileOpen(fp);

      QHttpHeaders headers;
      headers.insert(0, "Content-Disposition", QString("attachment; filename=\"%1\"").arg(thumb.fileName()));

      qDebug() << "sending len" << data.length();
      QHttpServerResponse response("application/octet-stream", data);
      response.setHeaders(std::move(headers));
      return response;

    });
    return future;
  });

  // http://127.0.0.1:3000/api/1/ap/polyhaven_1k/texture/BrushedConcrete001/diffuse/1k
  server->route("/api/1/ap/<arg>/texture/<arg>/<arg>/<arg>", QHttpServerRequest::Method::Get, [](
      const QString& pname, const QString& tname, const QString &ttype, const QString &tsize, const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, tname, ttype, tsize, &request] {
      const auto tex_name = tname.toLower();
      if (g::web_requires_auth) {
        const auto current_user = g::webSessions->get_user(request);
        if (current_user.isNull())
          return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
      }

      const auto ap = g::ctx->assetPackManager->by_name(pname);
      if (ap.isNull())
        return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);

      if (!ap->textures_lower.contains(tex_name))
        return QHttpServerResponse("application/json", "texture not found", QHttpServerResponder::StatusCode::NotFound);

      const TextureImageType _ttype = getTextureImageType(ttype.toLower());
      if (_ttype == TextureImageType::unknown)
        return QHttpServerResponse("application/json", "unknown texture type",
                                   QHttpServerResponder::StatusCode::NotFound);

      const TextureSize _tsize = getTextureSize(tsize.toLower());
      if (_tsize == TextureSize::null)
        return QHttpServerResponse("application/json", "unknown texture size", QHttpServerResponder::StatusCode::NotFound);

      const auto tex = ap->textures_lower[tex_name];

      const auto tex_img = tex->get_image(_ttype, _tsize);
      if (tex_img.isNull())
        return QHttpServerResponse("application/json", "texture image not found", QHttpServerResponder::StatusCode::NotFound);

      const auto img = tex_img->path_get(_ttype);
      qDebug() << img.absoluteFilePath();
      const auto fp = img.absoluteFilePath();
      const auto data = Utils::fileOpen(fp);
      const auto aa = img.fileName();

      QHttpHeaders headers;
      headers.insert(0, "Content-Disposition", QString("attachment; filename=\"%1\"").arg(img.fileName()));

      qDebug() << "sending len" << data.length();
      QHttpServerResponse response("application/octet-stream", data);
      response.setHeaders(std::move(headers));
      return response;

    });
    return future;
  });

  // http://127.0.0.1:3000/api/1/ap/polyhaven_1k/texture/BrushedConcrete001/1K/pack
  // @TODO: refuse to access when asset pack is busy (pack->status)
  server->route("/api/1/ap/<arg>/texture/<arg>/<arg>/pack", [](const QString &pname, const QString &tname, const QString &image_size, const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, tname, image_size, &request] {
      const QUrlQuery query(request.url());

      if (!STR2TEXSIZE.contains(image_size))
        return QHttpServerResponse("wrong image_size", QHttpServerResponder::StatusCode::NotFound);
      auto image_size_enum = STR2TEXSIZE[image_size];

      const auto pack = g::ctx->assetPackManager->by_name(pname);
      if (pack.isNull())
        return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);

      if (pack->atype != AssetPackType::assetPackTexture)
        return QHttpServerResponse("application/json", "asset pack has the wrong pack type", QHttpServerResponder::StatusCode::NotFound);

      qDebug() << "packing" << tname;
      QString err_msg;

      if (pack.isNull()) {
        err_msg = QString("asset_pack '%1' not found").arg(pname);
        qWarning() << err_msg;
        return QHttpServerResponse(err_msg, QHttpServerResponder::StatusCode::NotFound);
      }

      if (!pack->textures_lower.contains(tname.toLower())) {
        err_msg = QString("texture by name not found: %s").arg(tname.toLower());
        qWarning() << err_msg;
        return QHttpServerResponse(err_msg, QHttpServerResponder::StatusCode::NotFound);
      }

      const auto tex = pack->textures_lower[tname.toLower()];
      // @TODO: mat properties?
      // auto properties = tex->

      QStringList paths;
      for (int i = 0; i < static_cast<int>(TextureImageType::textureImageTypeCount); ++i) {
        const auto ttype = static_cast<TextureImageType>(i);
        if (ttype == TextureImageType::unknown)
          continue;

        if (auto img = tex->get_image(ttype, image_size_enum); !img.isNull())
          paths << img->path.absoluteFilePath();
      }

      if (paths.isEmpty()) {
        err_msg = QString("no suitable paths found");
        qWarning() << err_msg;
        return QHttpServerResponse(err_msg, QHttpServerResponder::StatusCode::NotFound);
      }

      // const auto tres = tex->to_tres(image_size_enum);
      // const QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
      // const QString baseName = tex->name_lower + ".tres";
      // const QString filePath = tempDir + "/" + baseName;
      // qDebug() << "writing" << filePath;
      //
      // QFile file(filePath);
      // if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      //   QTextStream out(&file);
      //   out << tres;
      //   file.close();
      // }

      qDebug() << "packing";
      // paths << filePath;
      qDebug() << paths;
      const QByteArray data = packFiles(paths);
      // QFile::remove(filePath);

      QHttpHeaders headers;
      headers.insert(0, "Content-Disposition", QString("attachment; filename=\"%1\"").arg(tex->name + ".bin"));

      qDebug() << "sending len" << data.length();
      QHttpServerResponse response("application/octet-stream", data);
      response.setHeaders(std::move(headers));
      return response;
    });

    return future;
  });
}

}
