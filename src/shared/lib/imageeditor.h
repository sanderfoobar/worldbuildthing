#pragma once
#include <QImage>
#include <QColor>

class ImageEditorFast {
  QImage original;

  int brightness = 0;
  double contrast = 1.0;
  double saturation = 1.0;
  int rShift = 0, gShift = 0, bShift = 0;
  int levelMin = 0, levelMax = 255;
  QColor tintColor = QColor(0,0,0);
  double tintIntensity = 0.0;

public:
  ImageEditorFast(const QString &path);

  void resetAdjustments();

  void setBrightnessContrast(int b, double c);
  void setSaturation(double s);
  void setColorShift(int r, int g, int b);
  void setLevels(int minVal, int maxVal);
  void setTint(const QColor &color, double intensity);

  QImage getImage() const;
  void save(const QString &path) const;
};
