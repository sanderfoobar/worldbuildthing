#pragma once

#include <QList>
#include <QSplitter>
#include <QMainWindow>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQuickView>
#include <QQmlContext>

#include "gl/GLWidget2DGrid.h"
#include "gl2/engine3d.h"

namespace Ui {
  class MainWindow;
}

class Ctx;
class MainWindow final : public QMainWindow {
Q_OBJECT

  enum class EditorMode {
    OBJECT = 0,
    EDIT,
    MATERIAL
  };

public:
  explicit MainWindow(Ctx *ctx, QWidget *parent = nullptr);
  static MainWindow *getInstance();
  static Ctx *getContext();
  ~MainWindow() override;

protected:
  bool focusNextPrevChild(bool) override { return false; }  // else TAB gets eaten

private:
  static MainWindow * pMainWindow;

  QQuickWidget *m_quickWidget = nullptr;
  void createQml();
  void destroyQml();

  gl::GLWidget2DGrid *m_glWidget2DGrid;
  Ui::MainWindow *ui;
  Ctx *m_ctx = nullptr;

  EditorMode m_editorMode = EditorMode::OBJECT;

  // QWidget *m_centralContainer = nullptr;
  QSplitter *m_mainSplitter = nullptr;
  QSplitter *m_leftSplitter = nullptr;
  QSplitter *m_rightSplitter = nullptr;

  QFrame* coloredWidget(const QString &colorName);

  void displayGrid();
  void displayFullSingle();

  const QList<Qt::Key> m_keys_wsad;

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
};
