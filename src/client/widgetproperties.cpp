#include <QJsonObject>
#include <QFile>

#include "widgetproperties.h"
#include "ui_widgetproperties.h"

#include "client/lib/utils.h"

WidgetProperties::WidgetProperties(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WidgetProperties) {
  ui->setupUi(this);
}

WidgetProperties::~WidgetProperties() {
  delete ui;
}
