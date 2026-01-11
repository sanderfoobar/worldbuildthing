#include <QObject>
#include <QLayout>
#include <QLabel>

#include "utils.h"

namespace utils {

QIcon godotIconByName(const QString& name) {
  const QString path = ":/icons_godot/" + name + ".svg";
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly))
    return {};
  const QByteArray data = file.readAll();
  QPixmap pixmap;
  pixmap.loadFromData(data, "SVG");
  return {pixmap};
}

void ui_nukeLabels(QWidget *root) {
  const auto labels = root->findChildren<QLabel*>();
  for (auto *label : labels) {
    if (auto *l = label->parentWidget()->layout())
      l->removeWidget(label);
    label->deleteLater();
  }
}

void ui_zeroMargins(QWidget *root) {
  if (!root) return;
  std::vector<QWidget*> stack { root };

  while (!stack.empty()) {
    QWidget *w = stack.back();
    stack.pop_back();

    if (auto l = w->layout()) {
      l->setContentsMargins(0, 0, 0, 0);
      for (int i = 0; i < l->count(); ++i) {
        if (auto childWidget = l->itemAt(i)->widget()) stack.push_back(childWidget);
      }
    }
  }
}

}