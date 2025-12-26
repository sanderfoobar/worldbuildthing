#include "directional_light.h"

#include "shared/lib/utils.h"

namespace gl {
  DirectionalLight::DirectionalLight(QObject * parent) : AbstractLight() {
    m_color = QVector3D(1.0f, 1.0f, 1.0f);
    m_direction = QVector3D(-1.0f, -1.0f, -1.0f);
    setObjectName("Untitled Directional Light");
    setParent(parent);
  }

  DirectionalLight::DirectionalLight(QVector3D color, QVector3D direction, QObject * parent): AbstractLight(color) {
    m_direction = direction;
    setObjectName("Untitled Directional Light");
    setParent(parent);
  }

  DirectionalLight::DirectionalLight(const DirectionalLight & light): AbstractLight(light) {
    m_direction = light.m_direction;
    setObjectName(light.objectName());
  }

  DirectionalLight::~DirectionalLight() {

    qDebug() << "Directional light" << this->objectName() << "is destroyed";
  }

  void DirectionalLight::dumpObjectInfo(int l) {
    qDebug().nospace() << "Directional Light: " << objectName();
    qDebug().nospace() << "Enabled: " << m_enabled;
    qDebug().nospace() << "Color: " << m_color;
    qDebug().nospace() << "Direction: " << m_direction;
    qDebug().nospace() << "Intensity: " << m_intensity;
  }

  void DirectionalLight::dumpObjectTree(int l) {
    dumpObjectInfo(l);
  }

  QVector3D DirectionalLight::direction() {
    return m_direction;
  }

  void DirectionalLight::setDirection(QVector3D direction) {
    if (!Utils::isEqual(m_direction, direction)) {
      m_direction = direction;

      qDebug() << "The direction of" << this->objectName() << "is set to" << direction;
      directionChanged(m_direction);
    }
  }
}