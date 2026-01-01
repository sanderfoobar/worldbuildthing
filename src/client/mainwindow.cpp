#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>
#include <QScreen>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QLabel>
#include <QCheckBox>

#include "shared/models/texture_model.h"
#include "shared/models/texture_proxy_model.h"

#include "client/ctx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *MainWindow::pMainWindow = nullptr;

MainWindow::MainWindow(Ctx *ctx, QWidget *parent) :
    QMainWindow(parent),
    m_ctx(ctx),
    m_keys_wsad({Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_Q, Qt::Key_E}),
    ui(new Ui::MainWindow) {

  g::devicePixelRatio = this->devicePixelRatio();

  ui->setupUi(this);
  pMainWindow = this;

  // installEventFilter(this);

  // gl context available
  connect(m_ctx, &Ctx::GLContextAvailable, [this] {
    // create 2d widget
    // m_glWidget2DGrid = new gl::GLWidget2DGrid(gl::GLWidget2DGrid::ViewType::Top, this);
    // connect(m_glWidget2DGrid, &gl::GLWidget2DGrid::initDone, this, [this] {
    //
    // });
  });

  setWindowTitle("Editor");
  resize(1900, 1200);

  const QScreen *screen = QGuiApplication::primaryScreen();
  const QRect screenGeometry = screen->availableGeometry();
  move((screenGeometry.width() - width()) / 2, (screenGeometry.height() - height()) / 2);

  QLayout *mainLayout = ui->centralWidget->layout();

  for (auto& [mode, name] : g::renderModesLookup) {
    auto cb = new QCheckBox(QString::fromUtf8(name), this);
    cb->setChecked(g::renderModes.has(mode));
    connect(cb, &QCheckBox::toggled, this, [mode](bool checked) {
      if (checked) g::renderModes.set(mode);
      else g::renderModes.clear(mode);
    });
    mainLayout->addWidget(cb);
  }

  if (auto vLayout = qobject_cast<QVBoxLayout*>(mainLayout)) {
    vLayout->addSpacing(32);
  }


  // m_glWidget3D = new GLWidget3D(this);
  // m_glWidget3D->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  // connect(m_glWidget3D, &GLWidget3D::initDone, this, [this] {  });

  createQml();

  QWidget *controls = new QWidget(this);
  controls->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  controls->setMaximumWidth(256);
  QVBoxLayout *controlsLayout = new QVBoxLayout(controls);

  // Light direction sliders
  auto addSlider = [&](const QString &label, int min, int max, int value, auto slot){
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(min, max);
    slider->setValue(value);
    controlsLayout->addWidget(new QLabel(label));
    controlsLayout->addWidget(slider);
    connect(slider, &QSlider::valueChanged, slot);
  };

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
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::createQml() {
  if(m_quickWidget != nullptr) return;
  m_quickWidget = new QQuickWidget(this);

  connect(m_quickWidget->quickWindow(), &QQuickWindow::sceneGraphInitialized, [this] {
    g::glcontext = QOpenGLContext::currentContext();
    emit m_ctx->GLContextAvailable();
  });

  auto *qctx = m_quickWidget->rootContext();
  qctx->setContextProperty("ctx", m_ctx);
  qctx->setContextProperty("TextureModel", gs::textureModel);
  qctx->setContextProperty("TextureProxyModel", gs::textureProxyModel);
  qctx->setContextProperty("mainwindow", this);
  qctx->engine()->addImageProvider(QLatin1String("textureProvider"), g::textureThumbnailQmlProvider);

  m_quickWidget->setSource(QUrl(QStringLiteral("qrc:/Main/qml/Test.qml")));
  m_quickWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
  ui->centralWidget->layout()->addWidget(m_quickWidget);
  m_quickWidget->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::destroyQml() {
  if(m_quickWidget == nullptr) return;
  m_quickWidget->disconnect();
  m_quickWidget->deleteLater();
  m_quickWidget = nullptr;
}

MainWindow* MainWindow::getInstance() { return pMainWindow; }
Ctx* MainWindow::getContext() { return pMainWindow ? pMainWindow->m_ctx : nullptr; }

// bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
//   if (event->type() == QEvent::KeyPress) {
//     QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
//     if (keyEvent->key() == Qt::Key_Tab) {
//       qDebug() << "Tab pressed!";
//       return true; // eat it if you don’t want focus to change
//     }
//   }
//   return QObject::eventFilter(obj, event);
// }

  // addSlider("Sun Pos X", -360, 360, int(m_glWidget3D->sunPosition().x),
  //           [this](const int v){ auto pos = m_glWidget3D->sunPosition(); pos.x = float(v); m_glWidget3D->setSunPosition(pos); });
  // addSlider("Sun Pos Y", -360, 360, int(m_glWidget3D->sunPosition().y),
  //           [this](const int v){ auto pos = m_glWidget3D->sunPosition(); pos.y = float(v); m_glWidget3D->setSunPosition(pos); });
  // addSlider("Sun Pos Z", -360, 360, int(m_glWidget3D->sunPosition().z),
  //           [this](const int v){ auto pos = m_glWidget3D->sunPosition(); pos.z = float(v); m_glWidget3D->setSunPosition(pos); });
  //
  // addSlider("Bias", 0, 100, int(m_glWidget3D->shadowBias()),
  //           [this](int v) {
  //             m_glWidget3D->setShadowBias(static_cast<float>(v));
  //           });
  //
  // // distance
  // addSlider("Light Distance", 1, 1000, int(m_glWidget3D->shadowLightDistanceMultiplier()),
  //           [this](int v){ m_glWidget3D->setShadowLightDistanceMultiplier(float(v)); });
  //
  // // Near/Far planes
  // addSlider("Near Plane", 1, 100, int(m_glWidget3D->shadowNearPlane()*10.0f),
  //           [this](int v) {
  //             qDebug() << "Near Plane" << v;
  //             m_glWidget3D->setShadowNearPlane(float(v));
  //           });
  // addSlider("Far Plane", 1, 1000, int(m_glWidget3D->shadowFarPlane()),
  //           [this](int v) {
  //             qDebug() << "Far Plane" << v;
  //             m_glWidget3D->setShadowFarPlane(float(v));
  //           });
  //
  // // Ortho scale
  // addSlider("Ortho Scale", 1, 200, int(m_glWidget3D->shadowOrthoScale()*100),
  //           [this](int v){ m_glWidget3D->setShadowOrthoScale(float(v)/100.0f); });
  //
  // // Y-up checkbox
  // QCheckBox *checkYUp = new QCheckBox("Use Y-up");
  // checkYUp->setChecked(m_glWidget3D->shadowUseYUp());
  // controlsLayout->addWidget(checkYUp);
  // connect(checkYUp, &QCheckBox::toggled, [this](bool checked){ m_glWidget3D->setShadowUseYUp(checked); });


void MainWindow::keyPressEvent(QKeyEvent *event) {
  const int key = event->key();
  // qDebug() << "Key pressed:" << QKeySequence(key).toString();

  if (m_keys_wsad.contains(key)) {
    g::engine3D->handleKeyPress(event);
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
  if (m_keys_wsad.contains(key)) {
    g::engine3D->handleKeyRelease(event);
    return;
  }

  QMainWindow::keyReleaseEvent(event);
}
