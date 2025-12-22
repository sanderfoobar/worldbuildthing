#pragma once
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "shared/lib/globals.h"

QByteArray packFiles(const QStringList& filepaths);
QList<PackedFile> unpackFiles(const QByteArray& buffer);
