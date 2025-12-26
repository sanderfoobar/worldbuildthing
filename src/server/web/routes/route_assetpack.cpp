#include <QHttpServerResponse>
#include <QtConcurrent>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "server/ctx.h"
#include "server/globals.h"
#include "server/web/routes/route_assetpack.h"
#include "server/web/routes/route_utils.h"
#include "server/web/web_sessionstore.h"

#include "shared/lib/utils.h"
#include "shared/models/texture_getters_setters.h"
#include "shared/models/texture_image_getters_setters.h"

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
      // for(auto it = g::ctx->assetPackManager->packs.cbegin(); it != g::ctx->assetPackManager->packs.cend(); ++it) {
      //   AssetPackType type = it.key();
      //   const QList<QSharedPointer<AssetPack>>& list = it.value();
      //
      //   for(const QSharedPointer<AssetPack>& pack : list) {
      //     rapidjson::Value item(rapidjson::kObjectType);
      //     item.AddMember("name", rapidjson::Value(pack->name().toUtf8().constData(), allocator), allocator);
      //     item.AddMember("description", rapidjson::Value(pack->description().toUtf8().constData(), allocator), allocator);
      //     item.AddMember("type_str", rapidjson::Value(pack->atype_str().toUtf8().constData(), allocator), allocator);
      //     item.AddMember("type", rapidjson::Value().SetInt(static_cast<int>(pack->atype)), allocator);
      //     item.AddMember("status_str", rapidjson::Value(pack->status_str().toUtf8().constData(), allocator), allocator);
      //     item.AddMember("status", rapidjson::Value().SetInt(static_cast<int>(pack->status)), allocator);
      //     arr.PushBack(item, allocator);
      //   }
      // }

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

  // server->route("/api/1/ap/<arg>", QHttpServerRequest::Method::Get, [](const QString& pname, const QHttpServerRequest &request) {
  //   QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, &request] {
  //     if (g::web_requires_auth) {
  //       const auto current_user = g::webSessions->get_user(request);
  //       if (current_user.isNull())
  //         return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
  //     }
  //
  //     const auto pack = g::ctx->assetPackManager->by_name(pname);
  //     if (pack.isNull())
  //       return QHttpServerResponse("application/json", "asset pack by name not found", QHttpServerResponder::StatusCode::NotFound);
  //
  //     rapidjson::Document root;
  //     root.SetObject();
  //     auto& allocator = root.GetAllocator();
  //
  //     QJsonObject json_obj;
  //     json_obj["name"] = pname;
  //     json_obj["dir"] = pack->get_directory().baseName();
  //     json_obj["description"] = pack->description();
  //     json_obj["tags"] = QJsonArray{};
  //
  //     QList<QSharedPointer<TextureTag>> all_tags;
  //     for(auto it = pack->textures.cbegin(); it != pack->textures.cend(); ++it) {
  //       const QSharedPointer<Texture>& tex = it.value();
  //       for (const auto& tex_tag : tex->tags()) {
  //         if (!all_tags.contains(tex_tag))
  //           all_tags << tex_tag;
  //       }
  //     }
  //
  //     QJsonObject textures;
  //
  //     for(auto it = pack->textures.cbegin(); it != pack->textures.cend(); ++it) {
  //       const QString& name = it.key();
  //       const QSharedPointer<Texture>& tex = it.value();
  //
  //       auto tex_blob = tex->to_json();
  //       auto tex_tags = tex->tags();
  //
  //       QJsonArray tex_tags_json{};
  //       for (auto _it = tex_tags.begin(); _it != tex_tags.end(); ++_it) {
  //         const QSharedPointer<TextureTag>& tex_tag = _it.value();
  //         auto tag_idx = all_tags.indexOf(tex_tag);
  //         tex_tags_json << tag_idx;
  //       }
  //
  //       tex_blob["tags"] = tex_tags_json;
  //       textures[name] = tex_blob;
  //     }
  //
  //     json_obj["meta"] = textures;
  //
  //     auto json_obj_tags = QJsonArray{};
  //     for (const auto& tag: all_tags)
  //       json_obj_tags << tag->name();
  //     json_obj["tags"] = json_obj_tags;
  //
  //     rapidjson::Value obj(rapidjson::kObjectType);
  //     QJsonDocument qdoc(json_obj);
  //     QByteArray jsonBytes = qdoc.toJson(QJsonDocument::Compact);
  //     rapidjson::Document tmp;
  //     tmp.Parse(jsonBytes.constData());
  //     rapidjson::Value blob;
  //     blob.CopyFrom(tmp, allocator);
  //
  //     root.AddMember("data", blob, allocator);
  //
  //     // serialize
  //     rapidjson::StringBuffer buffer;
  //     rapidjson::Writer writer(buffer);
  //     root.Accept(writer);
  //
  //     QByteArray jsonData(buffer.GetString(), static_cast<int>(buffer.GetSize()));
  //     return QHttpServerResponse("application/json", jsonData, QHttpServerResponder::StatusCode::Ok);
  //   });
  //   return future;
  // });

  // server->route("/api/1/ap/<arg>/metadata", QHttpServerRequest::Method::Get, [](const QString& pname, const QHttpServerRequest &request) {
  //   QFuture<QHttpServerResponse> future = QtConcurrent::run([pname, &request] {
  //     if (g::web_requires_auth) {
  //       const auto current_user = g::webSessions->get_user(request);
  //       if (current_user.isNull())
  //         return QHttpServerResponse("Unauthorized", QHttpServerResponder::StatusCode::Unauthorized);
  //     }
  //
  //     rapidjson::Document root;
  //     root.SetObject();
  //     auto& allocator = root.GetAllocator();
  //
  //     for(auto it = g::ctx->assetPackManager->packs.cbegin(); it != g::ctx->assetPackManager->packs.cend(); ++it) {
  //       AssetPackType type = it.key();
  //       const QList<QSharedPointer<AssetPack>>& list = it.value();
  //
  //       for(const QSharedPointer<AssetPack>& pack : list) {
  //         if (pack->name() == pname) {
  //           root.AddMember("data", pack->metadata_to_rapidjson(allocator), allocator);
  //
  //           // serialize
  //           rapidjson::StringBuffer buffer;
  //           rapidjson::Writer writer(buffer);
  //           root.Accept(writer);
  //
  //           QByteArray jsonData(buffer.GetString(), static_cast<int>(buffer.GetSize()));
  //           return QHttpServerResponse("application/json", jsonData, QHttpServerResponder::StatusCode::Ok);
  //         }
  //       }
  //     }
  //
  //     return QHttpServerResponse("application/json", {}, QHttpServerResponder::StatusCode::NotFound);
  //   });
  //   return future;
  // });
}

}
