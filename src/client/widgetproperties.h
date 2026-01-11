#pragma once
#include <QCompleter>
#include <QCheckBox>
#include <QPushButton>
#include <QStringList>
#include <QClipboard>
#include <QScroller>
#include <QStringListModel>
#include <QTimer>
#include <QEasingCurve>
#include <QMessageBox>
#include <QItemSelection>
#include <QStandardItemModel>
#include <QWidget>
#include <QMenu>
#include <QTreeView>

#include "client/widgetitemtree.h"

namespace Ui {
  class WidgetProperties;
}

class WidgetProperties : public QWidget
{
Q_OBJECT

public:
  explicit WidgetProperties(QWidget *parent = nullptr);
  ~WidgetProperties() override;

private:
  Ui::WidgetProperties *ui;

  QStandardItemModel *modelTree;
};
