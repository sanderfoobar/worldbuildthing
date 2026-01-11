#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>
#include <QScreen>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QJsonDocument>
#include <QLabel>
#include <QCheckBox>

#include "shared/models/texture_model.h"
#include "shared/models/texture_proxy_model.h"

// #include "client/lib/globals.h"
#include "client/ctx.h"
#include "client/lib/utils.h"
#include "mainwindow.h"

#include <shared/lib/nameof.hpp>

#include "ui_mainwindow.h"

MainWindow *MainWindow::pMainWindow = nullptr;

void MainWindow::onSetSidebarTitle(const QString& title) {
  m_sidebarTitle = title;
  emit sidebarTitleChanged();
}

void MainWindow::onSetSidebarItem(const QString& item) {
  m_sidebarItem = item;
  emit sidebarItemChanged();
}

void MainWindow::onEditorModeChanged(g::EditorMode mode) {
  const int index = static_cast<int>(mode);
  if (ui->comboMode->currentIndex() != index) {
    QSignalBlocker b(ui->comboMode);
    ui->comboMode->setCurrentIndex(index);
  }
}

MainWindow::MainWindow(Ctx *ctx, QWidget *parent) :
    QMainWindow(parent),
    ctx_(ctx),
    keys_wsad_({Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_Q, Qt::Key_E}),
    ui(new Ui::MainWindow) {
  ui->setupUi(this);
  utils::ui_zeroMargins(ui->root);
  utils::ui_nukeLabels(ui->itemtree);

  connect(ctx_, &Ctx::editorModeChanged, this, &MainWindow::onEditorModeChanged);

  m_widgetProperties = new WidgetProperties(this);
  m_widgetItemTree = new WidgetItemTree(this);
  m_widgetProperties->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_widgetItemTree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  mainSidebarClicked("properties");

  g::devicePixelRatio = this->devicePixelRatio();

  pMainWindow = this;

  // installEventFilter(this);

  // gl context available
  connect(ctx_, &Ctx::GLContextAvailable, [this] {
    // create 2d widget
    // m_glWidget2DGrid = new gl::GLWidget2DGrid(gl::GLWidget2DGrid::ViewType::Top, this);
    // connect(m_glWidget2DGrid, &gl::GLWidget2DGrid::initDone, this, [this] {
    // });
  });

  setWindowTitle("Editor");
  resize(1900, 1200);

  const QScreen *screen = QGuiApplication::primaryScreen();
  const QRect screenGeometry = screen->availableGeometry();
  move((screenGeometry.width() - width()) / 2, (screenGeometry.height() - height()) / 2);

  ui->comboMode->addItem(QIcon(":/icons_blender/object_datamode.svg"), "Object Mode");
  ui->comboMode->addItem(QIcon(":/icons_blender/editmode_hlt.svg"), "Edit Mode");
  ui->comboMode->addItem(QIcon(":/icons_blender/material.svg"), "Material Mode");

  connect(ui->comboMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, [this](int index) {
      if (!ui->comboMode->hasFocus())
        return;
      ctx_->switchMode(static_cast<g::EditorMode>(index));
    });

  QLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->root->layout());
  ui->toolbar->setContentsMargins(0, 0, 0, 6);

  ui->toolbar_line->setFrameShape(QFrame::HLine);
  ui->toolbar_line->setFrameShadow(QFrame::Plain);
  ui->toolbar_line->setLineWidth(1);
  ui->toolbar_line->setStyleSheet("color: #22a0ff;");

  ui->root->setContentsMargins(0, 0, 0, 0);
  ui->root->layout()->setSpacing(0);

  // =====
  ui->sidebar_left->setContentsMargins(0,0,0,0);
  ui->sidebar_left->layout()->setSpacing(0);
  ui->sidebar_right->setContentsMargins(0,0,0,0);
  ui->sidebar_right->layout()->setSpacing(0);
  ui->content->setContentsMargins(0,0,0,0);
  ui->content->layout()->setSpacing(0);
  ui->itemtree->setContentsMargins(0,0,0,0);
  ui->itemtree->layout()->setSpacing(0);
  // default sizes
  auto window_width = width();
  ui->splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  ui->toolbar->setMaximumHeight(48);
  ui->toolbar->setMinimumHeight(48);
  ui->toolbar->layout()->setSpacing(0);
  ui->toolbar_layout->setSpacing(0);

  // fixed left sidebar
  ui->sidebar_left->setMinimumWidth(76);
  ui->sidebar_left->setMaximumWidth(76);

  // optional right sidebar
  ui->sidebar_right->setMaximumWidth(76);

  // tree
  ui->itemtree->setMinimumWidth(150);
  ui->itemtree->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

  // content
  ui->content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // splitter stretch
  ui->splitter->setStretchFactor(0, 0);
  ui->splitter->setStretchFactor(1, 0);
  ui->splitter->setStretchFactor(2, 1);
  ui->splitter->setStretchFactor(3, 0);

  // set initial sizes (optional, ensures sidebar_left is 6px at startup)
  ui->splitter->setSizes({76, 200, 400, 76});

  ui->toolbar->setContentsMargins(6, 0, 6, 0);

  ui->content->layout()->setSpacing(0);
  ui->content->layout()->setContentsMargins(0,0,0,0);

  ui->itemtree->setContentsMargins(0,0,0,0);
  ui->itemtree->layout()->setSpacing(0);
  ui->itemtree->layout()->setContentsMargins(0,0,0,0);
  ui->itemtree->layout()->setSpacing(8);
  
  // for (auto& [mode, name] : g::renderModesLookup) {
  //   auto cb = new QCheckBox(QString::fromUtf8(name), this);
  //   cb->setChecked(g::renderModes.has(mode));
  //   connect(cb, &QCheckBox::toggled, this, [mode](bool checked) {
  //     if (checked) g::renderModes.set(mode);
  //     else g::renderModes.clear(mode);
  //   });
  //   mainLayout->addWidget(cb);
  // }
  // if (auto vLayout = qobject_cast<QVBoxLayout*>(mainLayout)) {
  //   vLayout->addSpacing(32);
  // }

  // m_glWidget3D = new GLWidget3D(this);
  // m_glWidget3D->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  // connect(m_glWidget3D, &GLWidget3D::initDone, this, [this] {  });

  // createQml();

  // QWidget *controls = new QWidget(this);
  // controls->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  // controls->setMaximumWidth(256);
  // QVBoxLayout *controlsLayout = new QVBoxLayout(controls);
  //
  // // Light direction sliders
  // auto addSlider = [&](const QString &label, int min, int max, int value, auto slot){
  //   QSlider *slider = new QSlider(Qt::Horizontal);
  //   slider->setRange(min, max);
  //   slider->setValue(value);
  //   controlsLayout->addWidget(new QLabel(label));
  //   controlsLayout->addWidget(slider);
  //   connect(slider, &QSlider::valueChanged, slot);
  // };

  //mainLayout->addWidget(controls);

  // switchMode(EditorMode::OBJECT);
  this->show();

  QTimer::singleShot(250, [this] {
    const auto scene = gl::Scene::setupSceneFromVMF("/home/dsc/CLionProjects/godot/texture_engine/data/shadow_test.vmf");
    if (scene.isNull()) {
      qCritical() << "Failed to setup scene";
      return;
    }

    if (g::engine3D == nullptr) {
      qCritical() << "Failed to setup engine3D";
      return;
    }

    g::engine3D->set_scene(scene);
  });

  createQmlSidebarLeft();
  createQmlContent();
  createQmlItemTreeHeader();
}

void MainWindow::createQmlItemTreeHeader() {
  utils::ui_nukeLabels(ui->itemtree);

  if(itemtreeHeader_ != nullptr) return;
  itemtreeHeader_ = new QQuickWidget(this);

  auto *qctx = itemtreeHeader_->rootContext();
  qctx->setContextProperty("ctx", ctx_);
  qctx->setContextProperty("window", this);
  qctx->engine()->addImageProvider(QLatin1String("icons_blender"), g::iconsBlenderQmlProvider);
  qctx->engine()->addImageProvider(QLatin1String("icons_godot"), g::iconsGodotQmlProvider);

  itemtreeHeader_->setSource(QUrl(QStringLiteral("qrc:/Main/qml/itemtree/ItemTreeHeader.qml")));
  itemtreeHeader_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  itemtreeHeader_->setResizeMode(QQuickWidget::SizeRootObjectToView);
  itemtreeHeader_->setMinimumSize(QSize(48, 48));
  itemtreeHeader_->setMaximumSize(QSize(32323, 48));
  auto *v = qobject_cast<QVBoxLayout*>(ui->itemtree->layout());
  v->insertWidget(0, itemtreeHeader_);

  itemtreeHeader_->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::createQmlSidebarLeft() {
  if(quickSidebarLeft_ != nullptr) return;
  quickSidebarLeft_ = new QQuickWidget(this);

  utils::ui_nukeLabels(ui->sidebar_left);

  auto *qctx = quickSidebarLeft_->rootContext();
  qctx->setContextProperty("ctx", ctx_);
  qctx->setContextProperty("window", this);
  qctx->engine()->addImageProvider(QLatin1String("icons_blender"), g::iconsBlenderQmlProvider);
  qctx->engine()->addImageProvider(QLatin1String("icons_godot"), g::iconsGodotQmlProvider);

  quickSidebarLeft_->setSource(QUrl(QStringLiteral("qrc:/Main/qml/sidebar/MainSidebar.qml")));
  quickSidebarLeft_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  quickSidebarLeft_->setResizeMode(QQuickWidget::SizeRootObjectToView);
  ui->sidebar_left->layout()->addWidget(quickSidebarLeft_);

  quickSidebarLeft_->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::createQmlContent() {
  if(quickContent_ != nullptr) return;
  quickContent_ = new QQuickWidget(this);

  utils::ui_nukeLabels(ui->content);

  connect(quickContent_->quickWindow(), &QQuickWindow::sceneGraphInitialized, [this] {
    g::glcontext = QOpenGLContext::currentContext();
    emit ctx_->GLContextAvailable();
  });

  auto *qctx = quickContent_->rootContext();
  qctx->setContextProperty("ctx", ctx_);
  qctx->setContextProperty("TextureModel", gs::textureModel);
  qctx->setContextProperty("TextureProxyModel", gs::textureProxyModel);
  qctx->setContextProperty("window", this);
  qctx->engine()->addImageProvider(QLatin1String("textureProvider"), g::textureThumbnailQmlProvider);
  qctx->engine()->addImageProvider(QLatin1String("icons_blender"), g::iconsBlenderQmlProvider);
  qctx->engine()->addImageProvider(QLatin1String("icons_godot"), g::iconsGodotQmlProvider);

  quickContent_->setSource(QUrl(QStringLiteral("qrc:/Main/qml/Main.qml")));
  quickContent_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  quickContent_->setResizeMode(QQuickWidget::SizeRootObjectToView);
  ui->content->layout()->addWidget(quickContent_);
  quickContent_->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::destroyQml() {
  if(quickContent_ == nullptr) return;
  quickContent_->disconnect();
  quickContent_->deleteLater();
  quickContent_ = nullptr;
}

MainWindow* MainWindow::getInstance() { return pMainWindow; }
Ctx* MainWindow::getContext() { return pMainWindow ? pMainWindow->ctx_ : nullptr; }

void MainWindow::keyPressEvent(QKeyEvent *event) {
  const int key = event->key();
  // qDebug() << "Key pressed:" << QKeySequence(key).toString();

  if (keys_wsad_.contains(key)) {
    g::engine3D->handleKeyPress(event);
    return;
  }

  if (key == Qt::Key_Tab) {
    if (ctx_->editorMode == g::EditorMode::OBJECT) {
      ctx_->switchMode(g::EditorMode::MATERIAL);
    } else if (ctx_->editorMode == g::EditorMode::MATERIAL) {
      ctx_->switchMode(g::EditorMode::OBJECT);
    }

    return;
  }

  if(key == Qt::Key_Z) {
    auto res = g::engine3D->toggleFreeLook();
    if (res) {
      setMouseTracking(true);
      grabMouse(); // capture mouse to allow movement outside window
      setCursor(Qt::BlankCursor); // Hide cursor when free look is enabled
    } else {
      setMouseTracking(false);
      releaseMouse(); // release mouse capture
      unsetCursor(); // restore cursor when free look is disabled
    }
  }

  QMainWindow::keyPressEvent(event);
}

void MainWindow::mouseEventFreeLookEnable() {
  m_rightMouseDown = true;
  m_firstMouse = true;
  setCursor(Qt::BlankCursor);
  // store position to restore after release
  m_mouseReleaseRestorePos = QCursor::pos();
  m_lastMouseX = width() / 2;
  m_lastMouseY = height() / 2;
  // warp mouse to center of widget
  QPoint center = mapToGlobal(QPoint(width() / 2, height() / 2));
  QCursor::setPos(center);
}

void MainWindow::mouseEventFreeLookDisable() {
  m_rightMouseDown = false;
  unsetCursor();
  QCursor::setPos(m_mouseReleaseRestorePos);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  const int key = event->key();
  if (keys_wsad_.contains(key)) {
    g::engine3D->handleKeyRelease(event);
    return;
  }

  QMainWindow::keyReleaseEvent(event);
}

void MainWindow::mainSidebarClicked(const QString& menu_item) {
  if (menu_item == "properties") {
    m_widgetItemTree->hide();
    ui->itemtree->layout()->removeWidget(m_widgetItemTree);
    ui->itemtree->layout()->addWidget(m_widgetProperties);
    m_widgetProperties->show();
    onSetSidebarTitle("Properties");
    onSetSidebarItem("properties");
  } else if (menu_item == "itemtree") {
    m_widgetProperties->hide();
    ui->itemtree->layout()->removeWidget(m_widgetProperties);
    ui->itemtree->layout()->addWidget(m_widgetItemTree);
    m_widgetItemTree->show();
    onSetSidebarTitle("Files");
    onSetSidebarItem("itemtree");
  }
  emit sidebarItemChanged();
}

MainWindow::~MainWindow() {
  delete ui;
}
