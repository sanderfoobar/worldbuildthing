#include <QApplication>
#include <QGuiApplication>
#include <QResource>
#include <QtCore>

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

#if defined(Q_OS_LINUX) && defined(STATIC)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif

#include "png.h"

#include <sys/types.h>
#include <unistd.h>

#include "shared/lib/logger.h"
#include "client/lib/qt/theme_css_watcher.h"
#include "client/ctx.h"
#include "client/mainwindow.h"
#include "client/gl2/quick3dfbo.h"

int main(int argc, char **argv) {
  qDebug() << "Qt version:" << qVersion();

  QSurfaceFormat format;
  format.setSamples(0);
  format.setMajorVersion(4);
  format.setMinorVersion(5);
  format.setSwapInterval(1);  // vsync
  format.setSwapBehavior(QSurfaceFormat::SingleBuffer); // vsync
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication::setApplicationName("matedit");
  QApplication::setOrganizationDomain("kroket.io");
  QApplication::setOrganizationName("Kroket Ltd.");
  QApplication app(argc, argv);

  qmlRegisterType<gl::QuickFBO>("QuickFBO", 1, 0, "QuickFBO");

#ifdef DEBUG
  // list qrc:// files
  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    auto line = it.next();
    // qDebug() << line;
    if (!line.contains("qt-project.org"))
      qDebug() << it.next();
  }
#endif

  const QString path_css = "/home/dsc/CLionProjects/godot/texture_engine/src/lib/qt/theme/DarkBlue.qss";
  auto *css_theme = new CSSTheme(&app, path_css);

#ifdef DEBUG
  css_theme->enable_watcher();
#endif

  app.setStyleSheet(css_theme->readCSS());

  auto *ctx = new Ctx();

  ctx->is_debug = false;
  auto *mainWindow = new MainWindow(ctx);

  return QApplication::exec();
}
