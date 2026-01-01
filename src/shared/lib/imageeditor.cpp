#include "imageeditor.h"
#include <algorithm>
#include <cmath>

//int main() {
//  // Load the image
//  ImageEditorFast img("input.png");
//
//  // Set non-destructive adjustments
//  img.setBrightnessContrast(20, 1.2);     // +20 brightness, 1.2 contrast
//  img.setSaturation(1.3);                  // 30% more saturation
//  img.setColorShift(10, -5, 0);           // shift RGB channels
//  img.setLevels(30, 220);                  // adjust levels
//  img.setTint(QColor(255, 100, 50), 0.3); // 30% orange tint
//
//  // Get the adjusted image
//  QImage result = img.getImage();
//  result.save("preview.png");             // optional preview
//
//  // Save to final file
//  img.save("output.png");
//
//  return 0;
//}

ImageEditorFast::ImageEditorFast(const QString &path) {
  original = QImage(path).convertToFormat(QImage::Format_ARGB32);
}

void ImageEditorFast::resetAdjustments() {
  brightness = 0;
  contrast = 1.0;
  saturation = 1.0;
  rShift = gShift = bShift = 0;
  levelMin = 0;
  levelMax = 255;
  tintColor = QColor(0,0,0);
  tintIntensity = 0.0;
}

void ImageEditorFast::setBrightnessContrast(int b, double c) { brightness = b; contrast = c; }
void ImageEditorFast::setSaturation(double s) { saturation = s; }
void ImageEditorFast::setColorShift(int r, int g, int b) { rShift = r; gShift = g; bShift = b; }
void ImageEditorFast::setLevels(int minVal, int maxVal) { levelMin = minVal; levelMax = maxVal; }
void ImageEditorFast::setTint(const QColor &color, double intensity) { tintColor = color; tintIntensity = std::clamp(intensity,0.0,1.0); }

namespace {
inline int clamp(int v) { return std::clamp(v, 0, 255); }

// simple RGB to HSV and back for saturation adjustment (fast approximation)
inline void adjustSaturationFast(int &r, int &g, int &b, double factor) {
  double maxc = std::max({r,g,b});
  double minc = std::min({r,g,b});
  double delta = maxc - minc;
  if(delta < 1e-5) return;
  double v = maxc / 255.0;
  double s = delta / maxc * factor;
  double scale = (v > 0) ? s * 255.0 / delta : 0;
  r = clamp(int(r + (r - minc) * scale));
  g = clamp(int(g + (g - minc) * scale));
  b = clamp(int(b + (b - minc) * scale));
}
}

QImage ImageEditorFast::getImage() const {
  QImage edited = original;
  int tr = tintColor.red(), tg = tintColor.green(), tb = tintColor.blue();
  double tintInv = 1.0 - tintIntensity;
  double scale = 255.0 / std::max(levelMax - levelMin, 1);

  for(int y = 0; y < edited.height(); ++y) {
    QRgb *line = reinterpret_cast<QRgb*>(edited.scanLine(y));
    for(int x = 0; x < edited.width(); ++x) {
      int a = qAlpha(line[x]);
      int r = qRed(line[x]), g = qGreen(line[x]), b = qBlue(line[x]);

      // brightness/contrast
      r = clamp(int((r - 128) * contrast + 128 + brightness));
      g = clamp(int((g - 128) * contrast + 128 + brightness));
      b = clamp(int((b - 128) * contrast + 128 + brightness));

      // saturation
      adjustSaturationFast(r,g,b,saturation);

      // color shift
      r = clamp(r + rShift);
      g = clamp(g + gShift);
      b = clamp(b + bShift);

      // levels
      r = clamp(int((r - levelMin) * scale));
      g = clamp(int((g - levelMin) * scale));
      b = clamp(int((b - levelMin) * scale));

      // tint
      r = clamp(int(r * tintInv + tr * tintIntensity));
      g = clamp(int(g * tintInv + tg * tintIntensity));
      b = clamp(int(b * tintInv + tb * tintIntensity));

      line[x] = qRgba(r,g,b,a);
    }
  }

  return edited;
}

void ImageEditorFast::save(const QString &path) const { getImage().save(path); }
