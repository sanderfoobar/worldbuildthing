#pragma once

#include <QString>
#include <QVector3D>
#include <QJsonObject>

class MaterialPropertiesTemplate
{
public:
  MaterialPropertiesTemplate();

  bool read(const QString& path);
  [[nodiscard]] QJsonObject toJson() const;

  [[nodiscard]] QString albedoTexture() const;
  void setAlbedoTexture(const QString& v);

  [[nodiscard]] double metallic() const;
  void setMetallic(double v);

  [[nodiscard]] double metallicSpecular() const;
  void setMetallicSpecular(double v);

  [[nodiscard]] QString metallicTexture() const;
  void setMetallicTexture(const QString& v);

  [[nodiscard]] int metallicTextureChannel() const;
  void setMetallicTextureChannel(int v);

  [[nodiscard]] double roughness() const;
  void setRoughness(double v);

  [[nodiscard]] QString roughnessTexture() const;
  void setRoughnessTexture(const QString& v);

  [[nodiscard]] int roughnessTextureChannel() const;
  void setRoughnessTextureChannel(int v);

  [[nodiscard]] bool normalEnabled() const;
  void setNormalEnabled(bool v);

  [[nodiscard]] double normalScale() const;
  void setNormalScale(double v);

  [[nodiscard]] QString normalTexture() const;
  void setNormalTexture(const QString& v);

  [[nodiscard]] bool aoEnabled() const;
  void setAoEnabled(bool v);

  [[nodiscard]] double aoLightAffect() const;
  void setAoLightAffect(double v);

  [[nodiscard]] QString aoTexture() const;
  void setAoTexture(const QString& v);

  [[nodiscard]] double heightmapScale() const;
  void setHeightmapScale(double v);

  [[nodiscard]] bool heightmapDeepParallax() const;
  void setHeightmapDeepParallax(bool v);

  [[nodiscard]] int heightmapMinLayers() const;
  void setHeightmapMinLayers(int v);

  [[nodiscard]] int heightmapMaxLayers() const;
  void setHeightmapMaxLayers(int v);

  [[nodiscard]] QString heightmapTexture() const;
  void setHeightmapTexture(const QString& v);

  [[nodiscard]] QVector3D uv1Scale() const;
  void setUv1Scale(const QVector3D& v);

private:
  QString m_albedoTexture;
  double m_metallic;
  double m_metallicSpecular;
  QString m_metallicTexture;
  int m_metallicTextureChannel;
  double m_roughness;
  QString m_roughnessTexture;
  int m_roughnessTextureChannel;
  bool m_normalEnabled;
  double m_normalScale;
  QString m_normalTexture;
  bool m_aoEnabled;
  double m_aoLightAffect;
  QString m_aoTexture;
  double m_heightmapScale;
  bool m_heightmapDeepParallax;
  int m_heightmapMinLayers;
  int m_heightmapMaxLayers;
  QString m_heightmapTexture;
  QVector3D m_uv1Scale;
};

void init_material_templates();
