#pragma once

#include <QList>
#include <QSplitter>
#include <QMainWindow>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQuickView>
#include <QQmlContext>
#include <QItemSelection>

#include "gl/GLWidget2DGrid.h"
#include "gl2/engine3d.h"

#include "widgetitemtree.h"
#include "widgetproperties.h"

namespace Ui {
  class MainWindow;
}

class Ctx;

class MainWindow final : public QMainWindow {
Q_OBJECT
Q_PROPERTY(QString sidebarTitle MEMBER m_sidebarTitle NOTIFY sidebarTitleChanged)
Q_PROPERTY(QString sidebarItem MEMBER m_sidebarItem NOTIFY sidebarItemChanged)

public:
  explicit MainWindow(Ctx *ctx, QWidget *parent = nullptr);
  static MainWindow *getInstance();
  static Ctx *getContext();
  ~MainWindow() override;
  void createQmlItemTreeHeader();
  Q_INVOKABLE void mainSidebarClicked(const QString& menu_item);

protected:
  bool focusNextPrevChild(bool) override { return false; }  // else TAB gets eaten

private:
  static MainWindow * pMainWindow;

  QQuickWidget *quickContent_ = nullptr;
  QQuickWidget *quickSidebarLeft_ = nullptr;
  QQuickWidget *m_quickSidebarRight = nullptr;
  QQuickWidget *itemtreeHeader_ = nullptr;
  void createQmlContent();
  void createQmlSidebarLeft();
  void destroyQml();

  gl::GLWidget2DGrid *m_glWidget2DGrid;
  Ui::MainWindow *ui;
  Ctx *ctx_ = nullptr;

  const QList<Qt::Key> keys_wsad_;

  Q_INVOKABLE void mouseEventFreeLookEnable();
  Q_INVOKABLE void mouseEventFreeLookDisable();

  bool m_rightMouseDown = false;
  int m_lastMouseX, m_lastMouseY;
  bool m_firstMouse;
  bool m_mouseCaptured = false;
  QPoint m_mouseReleaseRestorePos;

  QString m_sidebarTitle = "Properties";
  QString m_sidebarItem = "properties";

signals:
  void sidebarTitleChanged();
  void sidebarItemChanged();
private slots:
  void onSetSidebarTitle(const QString& title);
  void onSetSidebarItem(const QString &item);
  void onEditorModeChanged(g::EditorMode mode);
protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
private:
  WidgetProperties *m_widgetProperties = nullptr;
  WidgetItemTree *m_widgetItemTree = nullptr;
};

