#include <QObject>
#include <QHostAddress>
#include <QDateTime>

#include "lib/bcrypt/bcrypt.h"
#include "lib/globals.h"
#include "account.h"

Account::Account(const QByteArray& account_name, QObject* parent) : m_name(account_name) ,QObject(parent) {
  qDebug() << "new account" << account_name;
}

void Account::setAccountName(const QByteArray &account_name) {
  m_name = account_name;
}

void Account::setPassword(const QByteArray &password) {
  m_password = password;
}

bool Account::verifyPassword(const QByteArray &password) {
  // @TODO: implement
  return true;
}

Account::~Account() {
  qDebug() << "RIP account";
}
