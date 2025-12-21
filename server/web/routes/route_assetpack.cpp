#include <QHttpServerResponse>
#include <QtConcurrent>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "web/routes/route_assetpack.h"
#include "web/routes/route_utils.h"
#include "web/web_sessionstore.h"

#include "ctx.h"
#include "globals.h"
#include "lib/utils.h"

#include "models/texture_getters_setters.h"
#include "models/texture_image_getters_setters.h"

namespace AssetPackRoute {

void install(QHttpServer *server) {
  server->route("/api/1/ap", QHttpServerRequest::Method::Get, [](const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([&request] {
      if (g::web_requires_auth) {
        const auto current_user = g::webSessions->get_user(request);
        if (current_user.isNull())
          return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
      }

      rapidjson::Document root;
      root.SetObject();
      auto& allocator = root.GetAllocator();

      rapidjson::Value arr(rapidjson::kArrayType);
      for(auto it = g::ctx->assetPackManager->packs.cbegin(); it != g::ctx->assetPackManager->packs.cend(); ++it) {
        AssetPackType type = it.key();
        const QList<QSharedPointer<AssetPack>>& list = it.value();

        for(const QSharedPointer<AssetPack>& pack : list) {
          arr.PushBack(pack->to_rapidjson(allocator), allocator);
        }
      }

      root.AddMember("data", arr, allocator);

      // serialize
      rapidjson::StringBuffer buffer;
      rapidjson::Writer writer(buffer);
      root.Accept(writer);

      QByteArray jsonData(buffer.GetString(), static_cast<int>(buffer.GetSize()));
      return QHttpServerResponse("application/json", jsonData, QHttpServerResponder::StatusCode::Ok);
    });
    return future;
  });

  // http://127.0.0.1:3000/api/1/ap/polyhaven_1k/list
  server->route("/api/1/ap/<arg>/list", QHttpServerRequest::Method::Get, [](const QString& pname, const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, &request] {
      if (g::web_requires_auth) {
        const auto current_user = g::webSessions->get_user(request);
        if (current_user.isNull())
          return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
      }

      const auto pack = g::ctx->assetPackManager->by_name(pname);
      if (pack.isNull())
        return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);

      rapidjson::Document root;
      root.SetObject();
      auto& allocator = root.GetAllocator();

      QJsonObject json_obj;
      json_obj["name"] = pname;
      json_obj["dir"] = pack->dir();
      json_obj["description"] = pack->description();

      QJsonObject textures;

      for(auto it = pack->textures.cbegin(); it != pack->textures.cend(); ++it) {
        QJsonObject json_tex;
        QString name = it.key();
        QSharedPointer<Texture> tex = it.value();

        textures[name] = tex->to_json();
      }

      json_obj["textures"] = textures;

      rapidjson::Value obj(rapidjson::kObjectType);
      QJsonDocument qdoc(json_obj);
      QByteArray jsonBytes = qdoc.toJson(QJsonDocument::Compact);
      rapidjson::Document tmp;
      tmp.Parse(jsonBytes.constData());
      rapidjson::Value blob;
      blob.CopyFrom(tmp, allocator);

      root.AddMember("data", blob, allocator);

      // serialize
      rapidjson::StringBuffer buffer;
      rapidjson::Writer writer(buffer);
      root.Accept(writer);

      QByteArray jsonData(buffer.GetString(), static_cast<int>(buffer.GetSize()));
      return QHttpServerResponse("application/json", jsonData, QHttpServerResponder::StatusCode::Ok);
    });
    return future;
  });

  // http://127.0.0.1:3000/api/1/ap/metadata
  server->route("/api/1/ap/<arg>/metadata", QHttpServerRequest::Method::Get, [](const QString& pname, const QHttpServerRequest &request) {
    QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, &request] {
      if (g::web_requires_auth) {
        const auto current_user = g::webSessions->get_user(request);
        if (current_user.isNull())
          return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
      }

      rapidjson::Document root;
      root.SetObject();
      auto& allocator = root.GetAllocator();

      for(auto it = g::ctx->assetPackManager->packs.cbegin(); it != g::ctx->assetPackManager->packs.cend(); ++it) {
        AssetPackType type = it.key();
        const QList<QSharedPointer<AssetPack>>& list = it.value();

        for(const QSharedPointer<AssetPack>& pack : list) {
          if (pack->name() == pname) {
            root.AddMember("data", pack->metadata_to_rapidjson(allocator), allocator);

            // serialize
            rapidjson::StringBuffer buffer;
            rapidjson::Writer writer(buffer);
            root.Accept(writer);

            QByteArray jsonData(buffer.GetString(), static_cast<int>(buffer.GetSize()));
            return QHttpServerResponse("application/json", jsonData, QHttpServerResponder::StatusCode::Ok);
          }
        }
      }

      return QHttpServerResponse("application/json", {}, QHttpServerResponder::StatusCode::NotFound);
    });
    return future;
  });
}

}
