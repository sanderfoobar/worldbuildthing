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
  class WidgetItemTree;
}

class WidgetItemTree : public QWidget
{
Q_OBJECT

public:
  explicit WidgetItemTree(QWidget *parent = nullptr);
  ~WidgetItemTree() override;

  void setupItemTree(const QJsonDocument &doc);

private slots:
  void onTreeNodeSelected(const QItemSelection &selected, const QItemSelection &deselected);
  void onNodeViewContextMenuRequested(const QPoint &pos);
  void onTreeNodeDoubleClicked(const QModelIndex &index);

private:
  void setupUITable();
  void populateMethodsTree(const QJsonObject &methodsObj, QStandardItem *parentItem);
  void populateNodeTree(const QJsonObject &nodeObj, QStandardItem *parentItem);
  Ui::WidgetItemTree *ui;

  QStandardItemModel *modelTree;
};
