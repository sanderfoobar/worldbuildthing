#pragma once
#include <chrono>
#include <utility>
#include <functional>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <QCryptographicHash>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>

// #include "server/external/VTFLib/thirdparty/include/Compressonator.h"
#include "shared/lib/globals.h"
#include "shared/lib/config.h"
#include "shared/lib/utils.h"

extern QMap<QString, QPixmap> pixmapCache;

class TextureImage;
class Texture final : public QObject {
Q_OBJECT

public:
  explicit Texture(const QString &name, QObject *parent = nullptr);

  QString name;
  QString name_lower;
  unsigned int num_pixels = 0;

  QMap<TextureSize, QSharedPointer<TextureImage>> diffuse;
  QMap<TextureSize, QSharedPointer<TextureImage>> roughness;
  QMap<TextureSize, QSharedPointer<TextureImage>> ao;
  QMap<TextureSize, QSharedPointer<TextureImage>> arm;
  QMap<TextureSize, QSharedPointer<TextureImage>> displacement;
  QMap<TextureSize, QSharedPointer<TextureImage>> normal;
  QMap<TextureSize, QSharedPointer<TextureImage>> metalness;
  QMap<TextureSize, QSharedPointer<TextureImage>> opacity;
  QMap<TextureSize, QSharedPointer<TextureImage>> emission;
  QMap<TextureSize, QSharedPointer<TextureImage>> specular;
  QMap<TextureSize, QSharedPointer<TextureImage>> scattering;

  MaterialPropertiesTemplate* material_properties_template = nullptr;

  QMap<TextureSize, QMap<QString, QSharedPointer<TextureImage>>> variants;
  QList<QSharedPointer<TextureImage>> textures;

  QSet<QSharedPointer<TextureTag>> get_tags() {
    return m_tags;
  }

  QVariantList tags_as_variant();
  void set_author(const QString &author);
  QString get_author();
  void set_license(const QString &license);
  QString get_license();
  void set_tags(const QSet<QSharedPointer<TextureTag>> &tags);
  void append_tag(const QSharedPointer<TextureTag> &tag);
  QFileInfo path_thumbnail();
  void setMaterialPropertiesTemplate();

  [[nodiscard]] QString thumbnail_name() const { return m_thumbnail_name; };
  void set_thumbnail_name(const QString& thumbnail_name) { m_thumbnail_name = thumbnail_name; }

  [[nodiscard]] QSharedPointer<TextureImage> get_image(TextureImageType type ,TextureSize size) const;

  // @fuzzy: fallback to other resolutions when `size` is not available
  [[nodiscard]] QSharedPointer<TextureImage> get_diffuse(TextureSize size, bool fuzzy = false) const;

  void set_texture(const QSharedPointer<TextureImage> &tex);
  void addVariant(TextureSize tsize, const QString &variant, const QSharedPointer<TextureImage> &tex);
  void setDiffuse(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setRoughness(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setAO(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setARM(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setDisplacement(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setNormal(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setMetalness(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setOpacity(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setEmission(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setSpecular(TextureSize tsize, const QSharedPointer<TextureImage> &tex);
  void setScattering(TextureSize tsize, const QSharedPointer<TextureImage> &tex);

  [[nodiscard]] QList<TextureSize> available_sizes() const;

  QString to_tres(TextureSize tsize);
  QJsonObject to_json();

private:
  QString m_license;
  QString m_author;
  QString m_resourceTemplateID;
  QString m_thumbnail_name;

  QSet<QSharedPointer<TextureTag>> m_tags;
  QVariantList m_tags_as_variants;
};

