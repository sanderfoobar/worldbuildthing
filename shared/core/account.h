#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QTimer>
#include <QSet>
#include <QPointer>
#include <QElapsedTimer>
#include <QReadWriteLock>
#include <QDateTime>
#include <QHash>
#include <QUuid>

#include "lib/globals.h"

class Channel;

class Account final : public QObject {

public:
  explicit Account(const QByteArray& account_name = "", QObject* parent = nullptr);

  QByteArray account_name() { return m_name; }
  void setAccountName(const QByteArray &name);

  QByteArray password() { return m_password; }
  void setPassword(const QByteArray &password);

  bool verifyPassword(const QByteArray &password);

  ~Account() override;
private:
  QByteArray m_name;
  QByteArray m_password;
  QByteArray m_host;
};
