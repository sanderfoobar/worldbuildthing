#pragma once
#include <QIcon>
#include <QFile>
#include <QString>

namespace utils {

QIcon godotIconByName(const QString& name);
void ui_nukeLabels(QWidget *root);
void ui_zeroMargins(QWidget *root);

}