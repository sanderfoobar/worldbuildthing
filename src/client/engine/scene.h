#pragma once

#include <QGraphicsScene>
#include <QObject>
#include <QVector3D>
#include <filesystem>
#include <string>

#include "../gl/gl_uniform_buffer_object.h"
#include "directional_light.h"
#include "gameobject.h"

namespace gl {
class Scene final : public QObject {
Q_OBJECT
public:

  explicit Scene(QObject* parent = nullptr);

  static QSharedPointer<Scene> setupSceneFromVMF(std::filesystem::path path);
  static QSharedPointer<Scene> setupSceneWithABunchOfBoxes();

  static QSharedPointer<Scene> VMF(const std::filesystem::path &path);

  [[nodiscard]] const QVector<DirectionalLight*>& directionalLights() const {
    return m_directionalLights;
  }

  // const QVector<AmbientLight*>& Scene::ambientLights() const {
  //   return m_ambientLights;
  // }

  // const QVector<PointLight*>& Scene::pointLights() const {
  //   return m_pointLights;
  // }
  //
  // const QVector<SpotLight*>& Scene::spotLights() const {
  //   return m_spotLights;
  // }

  std::vector<std::shared_ptr<GameObject>> gameObjects;

private:
  // int m_gridlineNameCounter;
  int m_ambientLightNameCounter;
  int m_directionalLightNameCounter;
  int m_pointLightNameCounter;
  int m_spotLightNameCounter;

  // QVector<AmbientLight*> m_ambientLights;
  QVector<DirectionalLight*> m_directionalLights;
  // QVector<PointLight*> m_pointLights;
  // QVector<SpotLight*> m_spotLights;
};
}