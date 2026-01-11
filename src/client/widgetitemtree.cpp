#include <QJsonObject>
#include <QFile>

#include "widgetitemtree.h"
#include "ui_widgetitemtree.h"

#include "client/lib/utils.h"

WidgetItemTree::WidgetItemTree(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WidgetItemTree) {
  ui->setupUi(this);
  this->setupUITable();

  modelTree = new QStandardItemModel(this);
  modelTree->setHorizontalHeaderLabels({ "Scene" });

  ui->tree->setModel(modelTree);
  ui->tree->setHeaderHidden(true);
  ui->tree->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(ui->tree, &QTreeView::doubleClicked, this, &WidgetItemTree::onTreeNodeDoubleClicked);
  connect(ui->tree, &QTreeView::customContextMenuRequested, this, &WidgetItemTree::onNodeViewContextMenuRequested);
  connect(ui->tree->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WidgetItemTree::onTreeNodeSelected);
}

void WidgetItemTree::setupUITable() {

}

void WidgetItemTree::populateNodeTree(const QJsonObject &nodeObj, QStandardItem *parentItem) {
  if (!nodeObj.contains("children")) return;

  QJsonObject children = nodeObj["children"].toObject();
  for (const QString &childName : children.keys()) {
    QJsonObject childObj = children[childName].toObject();
    QString node_type = childObj["type"].toString();

    QString label = QString("%1").arg(childName);
    auto *item = new QStandardItem(utils::godotIconByName(node_type), label);
    item->setEditable(false);
    item->setData(node_type, Qt::UserRole + 1);
    parentItem->appendRow(item);

    populateNodeTree(childObj, item);
  }
}

void WidgetItemTree::onTreeNodeSelected(const QItemSelection &selected, const QItemSelection &deselected) {
  if (selected.indexes().isEmpty())
    return;

  QModelIndex index = selected.indexes().first();

  // Example: assume name is in column 0, type in column 1
  QString name = index.sibling(index.row(), 0).data().toString();
  QString type = index.data(Qt::UserRole + 1).toString();

  // if (m_nodeWidget)
  //   m_nodeWidget->setNode(name, type);
}

void WidgetItemTree::populateMethodsTree(const QJsonObject &methodsObj, QStandardItem *parentItem) {
  for (const QString &methodName : methodsObj.keys()) {
    if (!methodName.startsWith("gcam"))
      continue;

    QStandardItem *methodItem = new QStandardItem(utils::godotIconByName("Code"), methodName);
    parentItem->appendRow(methodItem);

    // optionally, list the arguments as children
    // QJsonObject methodDetails = methodsObj[methodName].toObject();
    // QJsonArray args = methodDetails["args"].toArray();
    // for (const QJsonValue &argVal : args) {
    //   QJsonObject argObj = argVal.toObject();
    //   QString argName = argObj["name"].toString();
    //   QStandardItem *argItem = new QStandardItem(QIcon(":/icons/arg_icon.png"), argName);
    //   methodItem->appendRow(argItem);
    // }
  }
}

void WidgetItemTree::setupItemTree(const QJsonDocument &doc) {
  // fill doc here with some test data
  QJsonObject methodsObj;
  methodsObj["methodA"] = QJsonObject{{"description", "First test method"}};
  methodsObj["methodB"] = QJsonObject{{"description", "Second test method"}};

  QJsonObject nodesObj;
  nodesObj["type"] = "Node3D";

  QJsonObject worldNode;
  worldNode["type"] = "MeshInstance3D";

  QJsonObject cameraNode;
  cameraNode["type"] = "Camera3D";

  QJsonObject childNodes;
  childNodes["worldspawn"] = worldNode;
  childNodes["Camera1"] = cameraNode;

  nodesObj["children"] = childNodes;

  QJsonObject dataObj;
  dataObj["methods"] = methodsObj;
  dataObj["nodes"] = nodesObj;

  QJsonObject rootObj;
  rootObj["data"] = dataObj;

  modelTree->clear();
  modelTree->setHorizontalHeaderLabels({ "Scene" });

  //QJsonObject rootObj = doc.object();
  //QJsonObject dataObj = rootObj["data"].toObject();

  // Setup methods branch
  if (dataObj.contains("methods")) {
    QJsonObject _methodsObj = dataObj["methods"].toObject();
    auto *methodsRootItem = new QStandardItem(utils::godotIconByName("MemberMethod"), QString("Methods"));
    modelTree->appendRow(methodsRootItem);
    populateMethodsTree(_methodsObj, methodsRootItem);
  }

  // Setup nodes branch
  //QJsonObject nodesObj = dataObj["nodes"].toObject();
  QString rootType = nodesObj["type"].toString();
  QStandardItem *rootItem = new QStandardItem(utils::godotIconByName(rootType), QString("Root"));
  modelTree->appendRow(rootItem);
  populateNodeTree(nodesObj, rootItem);

  ui->tree->expandAll();
}

void WidgetItemTree::onTreeNodeDoubleClicked(const QModelIndex &index) {
  if (!index.isValid())
    return;

  QString name = index.data(Qt::DisplayRole).toString();
  QString nodeType = index.data(Qt::UserRole + 1).toString();
  if (nodeType.isEmpty())
    return;

  // for (const auto &action : m_nodeWidget->typeActions) {
  //   if (action->name == name) {
  //     action->func();
  //     break;
  //   }
  // }
}

void WidgetItemTree::onNodeViewContextMenuRequested(const QPoint &pos) {
  QModelIndex index = ui->tree->indexAt(pos);
  if (!index.isValid())
    return;

  QString name = index.data(Qt::DisplayRole).toString();
  QString nodeType = index.data(Qt::UserRole + 1).toString();
  if (nodeType.isEmpty())
    return;

  // // Find actions for the nodeType
  // QList<QSharedPointer<NodeTypeActions>> matchingActions;
  // for (const auto &action : m_nodeWidget->typeActions) {
  //   if (action->name == name) {
  //     matchingActions.append(action);
  //   }
  // }
  //
  // if (matchingActions.isEmpty())
  //   return;
  //
  // // Create context menu
  // QMenu contextMenu(this);
  // for (const auto &action : matchingActions) {
  //   QAction *menuAction = new QAction(action->title, &contextMenu);
  //   connect(menuAction, &QAction::triggered, action->func);
  //   contextMenu.addAction(menuAction);
  // }
  //
  // contextMenu.exec(ui->tree->viewport()->mapToGlobal(pos));
}

WidgetItemTree::~WidgetItemTree() {
  delete ui;
}
