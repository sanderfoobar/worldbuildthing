#pragma once
#include <QHttpServer>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

class WebSessionStore;

namespace AssetPackRoute {
  void install(QHttpServer *server);
}
