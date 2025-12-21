#include "materialpropertiestemplate.h"

#include <QFile>
#include <QJsonArray>
#include <QTextStream>

#include "lib/globals.h"

static QString parseExtResource(const QString &v) {
  int a = v.indexOf('"');
  int b = v.lastIndexOf('"');
  return (a >= 0 && b > a) ? v.mid(a + 1, b - a - 1) : QString();
}

static QVector3D parseVector3(const QString &v) {
  int a = v.indexOf('(');
  int b = v.indexOf(')');
  if (a < 0 || b < 0)
    return {};
  const QStringList parts = v.mid(a + 1, b - a - 1).split(',');
  if (parts.size() != 3)
    return {};
  return QVector3D(parts[0].toDouble(), parts[1].toDouble(), parts[2].toDouble());
}

MaterialPropertiesTemplate::MaterialPropertiesTemplate() :
    m_metallic(0.4), m_metallicSpecular(0.4), m_metallicTextureChannel(0), m_roughness(0.8),
    m_roughnessTextureChannel(0), m_normalEnabled(false), m_normalScale(0.4), m_aoEnabled(false), m_aoLightAffect(0.4),
    m_heightmapScale(0.0), m_heightmapDeepParallax(false), m_heightmapMinLayers(0), m_heightmapMaxLayers(0) {}

bool MaterialPropertiesTemplate::read(const QString &path) {
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream s(&f);
  while (!s.atEnd()) {
    const QString line = s.readLine().trimmed();
    if (line.isEmpty() || line.startsWith('['))
      continue;

    const int eq = line.indexOf('=');
    if (eq < 0)
      continue;

    const QString key = line.left(eq).trimmed();
    const QString val = line.mid(eq + 1).trimmed();

    if (key == "albedo_texture")
      m_albedoTexture = parseExtResource(val);
    else if (key == "metallic")
      m_metallic = val.toDouble();
    else if (key == "metallic_specular")
      m_metallicSpecular = val.toDouble();
    else if (key == "metallic_texture")
      m_metallicTexture = parseExtResource(val);
    else if (key == "metallic_texture_channel")
      m_metallicTextureChannel = val.toInt();
    else if (key == "roughness")
      m_roughness = val.toDouble();
    else if (key == "roughness_texture")
      m_roughnessTexture = parseExtResource(val);
    else if (key == "roughness_texture_channel")
      m_roughnessTextureChannel = val.toInt();
    else if (key == "normal_enabled")
      m_normalEnabled = (val == "true");
    else if (key == "normal_scale")
      m_normalScale = val.toDouble();
    else if (key == "normal_texture")
      m_normalTexture = parseExtResource(val);
    else if (key == "ao_enabled")
      m_aoEnabled = (val == "true");
    else if (key == "ao_light_affect")
      m_aoLightAffect = val.toDouble();
    else if (key == "ao_texture")
      m_aoTexture = parseExtResource(val);
    else if (key == "heightmap_scale")
      m_heightmapScale = val.toDouble();
    else if (key == "heightmap_deep_parallax")
      m_heightmapDeepParallax = (val == "true");
    else if (key == "heightmap_min_layers")
      m_heightmapMinLayers = val.toInt();
    else if (key == "heightmap_max_layers")
      m_heightmapMaxLayers = val.toInt();
    else if (key == "heightmap_texture")
      m_heightmapTexture = parseExtResource(val);
    else if (key == "uv1_scale")
      m_uv1Scale = parseVector3(val);
  }

  return true;
}

QJsonObject MaterialPropertiesTemplate::toJson() const {
  QJsonObject o;
  o["albedo_texture"] = m_albedoTexture;
  o["metallic"] = m_metallic;
  o["metallic_specular"] = m_metallicSpecular;
  o["metallic_texture"] = m_metallicTexture;
  o["metallic_texture_channel"] = m_metallicTextureChannel;
  o["roughness"] = m_roughness;
  o["roughness_texture"] = m_roughnessTexture;
  o["roughness_texture_channel"] = m_roughnessTextureChannel;
  o["normal_enabled"] = m_normalEnabled;
  o["normal_scale"] = m_normalScale;
  o["normal_texture"] = m_normalTexture;
  o["ao_enabled"] = m_aoEnabled;
  o["ao_light_affect"] = m_aoLightAffect;
  o["ao_texture"] = m_aoTexture;
  o["heightmap_scale"] = m_heightmapScale;
  o["heightmap_deep_parallax"] = m_heightmapDeepParallax;
  o["heightmap_min_layers"] = m_heightmapMinLayers;
  o["heightmap_max_layers"] = m_heightmapMaxLayers;
  o["heightmap_texture"] = m_heightmapTexture;

  QJsonArray uv;
  uv.append(m_uv1Scale.x());
  uv.append(m_uv1Scale.y());
  uv.append(m_uv1Scale.z());
  o["uv1_scale"] = uv;

  return o;
}

QString MaterialPropertiesTemplate::albedoTexture() const { return m_albedoTexture; }

void MaterialPropertiesTemplate::setAlbedoTexture(const QString &v) { m_albedoTexture = v; }

double MaterialPropertiesTemplate::metallic() const { return m_metallic; }

void MaterialPropertiesTemplate::setMetallic(double v) { m_metallic = v; }

double MaterialPropertiesTemplate::metallicSpecular() const { return m_metallicSpecular; }

void MaterialPropertiesTemplate::setMetallicSpecular(double v) { m_metallicSpecular = v; }

QString MaterialPropertiesTemplate::metallicTexture() const { return m_metallicTexture; }

void MaterialPropertiesTemplate::setMetallicTexture(const QString &v) { m_metallicTexture = v; }

int MaterialPropertiesTemplate::metallicTextureChannel() const { return m_metallicTextureChannel; }

void MaterialPropertiesTemplate::setMetallicTextureChannel(int v) { m_metallicTextureChannel = v; }

double MaterialPropertiesTemplate::roughness() const { return m_roughness; }

void MaterialPropertiesTemplate::setRoughness(double v) { m_roughness = v; }

QString MaterialPropertiesTemplate::roughnessTexture() const { return m_roughnessTexture; }

void MaterialPropertiesTemplate::setRoughnessTexture(const QString &v) { m_roughnessTexture = v; }

int MaterialPropertiesTemplate::roughnessTextureChannel() const { return m_roughnessTextureChannel; }

void MaterialPropertiesTemplate::setRoughnessTextureChannel(int v) { m_roughnessTextureChannel = v; }

bool MaterialPropertiesTemplate::normalEnabled() const { return m_normalEnabled; }

void MaterialPropertiesTemplate::setNormalEnabled(bool v) { m_normalEnabled = v; }

double MaterialPropertiesTemplate::normalScale() const { return m_normalScale; }

void MaterialPropertiesTemplate::setNormalScale(double v) { m_normalScale = v; }

QString MaterialPropertiesTemplate::normalTexture() const { return m_normalTexture; }

void MaterialPropertiesTemplate::setNormalTexture(const QString &v) { m_normalTexture = v; }

bool MaterialPropertiesTemplate::aoEnabled() const { return m_aoEnabled; }

void MaterialPropertiesTemplate::setAoEnabled(bool v) { m_aoEnabled = v; }

double MaterialPropertiesTemplate::aoLightAffect() const { return m_aoLightAffect; }

void MaterialPropertiesTemplate::setAoLightAffect(double v) { m_aoLightAffect = v; }

QString MaterialPropertiesTemplate::aoTexture() const { return m_aoTexture; }

void MaterialPropertiesTemplate::setAoTexture(const QString &v) { m_aoTexture = v; }

double MaterialPropertiesTemplate::heightmapScale() const { return m_heightmapScale; }

void MaterialPropertiesTemplate::setHeightmapScale(double v) { m_heightmapScale = v; }

bool MaterialPropertiesTemplate::heightmapDeepParallax() const { return m_heightmapDeepParallax; }

void MaterialPropertiesTemplate::setHeightmapDeepParallax(bool v) { m_heightmapDeepParallax = v; }

int MaterialPropertiesTemplate::heightmapMinLayers() const { return m_heightmapMinLayers; }

void MaterialPropertiesTemplate::setHeightmapMinLayers(int v) { m_heightmapMinLayers = v; }

int MaterialPropertiesTemplate::heightmapMaxLayers() const { return m_heightmapMaxLayers; }

void MaterialPropertiesTemplate::setHeightmapMaxLayers(int v) { m_heightmapMaxLayers = v; }

QString MaterialPropertiesTemplate::heightmapTexture() const { return m_heightmapTexture; }

void MaterialPropertiesTemplate::setHeightmapTexture(const QString &v) { m_heightmapTexture = v; }

QVector3D MaterialPropertiesTemplate::uv1Scale() const { return m_uv1Scale; }

void MaterialPropertiesTemplate::setUv1Scale(const QVector3D &v) { m_uv1Scale = v; }

void init_material_templates() {
  gs::nameToResourceTemplateLookup["paper"] = "concrete";
  gs::nameToResourceTemplateLookup["concrete"] = "concrete";
  gs::nameToResourceTemplateLookup["cardboard"] = "concrete";
  gs::nameToResourceTemplateLookup["cement"] = "concrete";
  gs::nameToResourceTemplateLookup["gravel"] = "concrete";
  gs::nameToResourceTemplateLookup["pavement"] = "concrete";
  gs::nameToResourceTemplateLookup["road"] = "concrete";
  gs::nameToResourceTemplateLookup["asphalt"] = "concrete";
  gs::nameToResourceTemplateLookup["brick"] = "brickwall";
  gs::nameToResourceTemplateLookup["wall"] = "brickwall";
  gs::nameToResourceTemplateLookup["cobblestone"] = "cobblestone";
  gs::nameToResourceTemplateLookup["grass"] = "grass";
  gs::nameToResourceTemplateLookup["sand"] = "grass";
  gs::nameToResourceTemplateLookup["beach"] = "grass";
  gs::nameToResourceTemplateLookup["terrain"] = "grass";
  gs::nameToResourceTemplateLookup["field"] = "grass";
  gs::nameToResourceTemplateLookup["leaves"] = "grass_leafs";
  gs::nameToResourceTemplateLookup["leafs"] = "grass_leafs";
  gs::nameToResourceTemplateLookup["forrest"] = "grass_leafs";
  gs::nameToResourceTemplateLookup["metal_sheet"] = "metal_sheet";
  gs::nameToResourceTemplateLookup["metal"] = "metal_sheet";
  gs::nameToResourceTemplateLookup["porcelain"] = "metal_sheet";
  gs::nameToResourceTemplateLookup["planks"] = "planks";
  gs::nameToResourceTemplateLookup["wood"] = "planks";
  gs::nameToResourceTemplateLookup["walnut"] = "planks";
  gs::nameToResourceTemplateLookup["oak"] = "planks";
  gs::nameToResourceTemplateLookup["bark"] = "planks";
  gs::nameToResourceTemplateLookup["shelf"] = "planks";
  gs::nameToResourceTemplateLookup["plaster"] = "plaster_damaged";
  gs::nameToResourceTemplateLookup["stone"] = "stone";
  gs::nameToResourceTemplateLookup["rock"] = "stone";
  gs::nameToResourceTemplateLookup["tiles"] = "tiles";
  gs::nameToResourceTemplateLookup["tile"] = "tiles";
  gs::nameToResourceTemplateLookup["sofa"] = "fabric";
  gs::nameToResourceTemplateLookup["clothes"] = "fabric";
  gs::nameToResourceTemplateLookup["cloth"] = "fabric";
  gs::nameToResourceTemplateLookup["carpet"] = "fabric";
  gs::nameToResourceTemplateLookup["fabric"] = "fabric";
  gs::nameToResourceTemplateLookup["fur"] = "fabric";
  gs::nameToResourceTemplateLookup["painting"] = "painting";
  gs::nameToResourceTemplateLookup["ground"] = "concrete";
  gs::nameToResourceTemplateLookup["surface"] = "concrete";
  gs::nameToResourceTemplateLookup["marble"] = "marble";
  gs::nameToResourceTemplateLookup["mosaic"] = "marble";
  gs::nameToResourceTemplateLookup["roman"] = "marble";
  gs::nameToResourceTemplateLookup["gold"] = "marble";
  gs::nameToResourceTemplateLookup["glitter"] = "marble";
  gs::nameToResourceTemplateLookup["carbon"] = "marble";

  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString path_resource = it.next();
    if (!path_resource.endsWith(".tres"))
      continue;

    QFileInfo info(path_resource);
    QFile file(path_resource);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    QString content = in.readAll();
    auto res = new MaterialPropertiesTemplate();
    res->read(path_resource);
    gs::resourceTemplates[info.baseName()] = res;
  }
}
