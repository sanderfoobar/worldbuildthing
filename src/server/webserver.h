#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QTcpServer>
#include <QHttpServer>

// class WebServer2 : public QObject {
//   Q_OBJECT
//
// public:
//   explicit WebServer2(QObject *parent = nullptr);
//   ~WebServer2() override = default;
//
// private:
//   QHttpServer* m_server;
//   QTcpServer* m_tcp_server;
//   int m_tcp_port = 19200;
// };