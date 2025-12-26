#include <QDir>
#include <QFile>
#include <QHostAddress>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QUrlQuery>
#include <QtConcurrent>
#include "web_webserver.h"

#include "server/web/routes/route_assetpack.h"

#include "routes/route_texture.h"
#include "server/globals.h"
#include "shared/lib/utils.h"

WebServer::WebServer(QObject *parent) : QObject(parent),
    m_server(new QHttpServer(this)),
    m_tcp_server(new QTcpServer(this)) {
  if (g::webSessions == nullptr)
    g::webSessions = new WebSessionStore();
  registerRoutes();
}

void WebServer::setHost(const QString &host) { m_host = host; }
void WebServer::setPort(quint16 port) { m_port = port; }

bool WebServer::start() {
  QHostAddress addr(m_host);
  if (!m_tcp_server->listen(addr, m_port) || !m_server->bind(m_tcp_server)) {
    emit failed(tr("Server failed to listen on %1:%2").arg(m_host, m_port));
    return false;
  }
  emit started();
  return true;
}

void WebServer::registerRoutes() {
  // provide helpers to routes via lambdas capturing 'this'
  using namespace std::placeholders;

  // root
  m_server->route("/", [] {
    return QStringLiteral("Hello from QHttpServer (Qt6)");
  });

  AssetPackRoute::install(m_server);
  AssetPackTextureRoute::install(m_server);
}

void WebServer::stop() {
  if (m_tcp_server->isListening())
    m_tcp_server->close();
  // QHttpServer doesn't need explicit stop; destructor will cleanup
}

WebServer::~WebServer() {
  stop();
}