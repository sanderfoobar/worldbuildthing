#pragma once

#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <utility>

#include "shared/lib/config.h"
#include "shared/lib/globals.h"
#include "shared/lib/utils.h"
#include "texture.h"

#include <GL/gl.h>

class TextureImage final : public QObject {
Q_OBJECT

// enum class TexImgExt {
//   PNG,
//   JPG
// };

public:
  [[nodiscard]] uint32_t getPickingID() const { return m_pickingID; }
  void setPickingID(const uint32_t id) { m_pickingID = id; }

  [[nodiscard]] GLuint getAlbedoTexture() const { return m_albedoID; }
  void setAlbedoTexture(const GLuint id) { m_albedoID = id; }

  void bind() const;
  void unbind() const;
private:
  GLuint m_albedoID = 0;
  GLuint m_normalID = 0;
  GLuint m_metallicID = 0;
  GLuint m_roughnessID = 0;
  GLuint m_aoID = 0;
  uint32_t m_pickingID = 0;
public:
  explicit TextureImage(const TexImgInfo &tex_info, TexImgExt ext, QObject *parent = nullptr);
  static TextureImage* from_path(const QFileInfo &path);

  QString name;
  QString name_original;
  QString name_technical;
  QString name_lower;
  TexImgExt ext;
  QString variant;
  bool is_alpha = false;

  TextureSize size = TextureSize::null;
  TextureImageType type = TextureImageType::unknown;

  QSize dimensions;
  QFileInfo path;

  QString basedir;

  void metadata_generate();

  void ensure_thumbnail(bool force, QString &err);
  void set_path(const QFileInfo &path);
  void setTextureImageType(TextureImageType _type);
  QFileInfo path_thumbnail();

  void inspect_channels_and_dimensions();
  void inspect_checksum();

  void set_channels(unsigned short channels);
  void set_checksum(const QString& checksum);

  // const getters/setters
  [[nodiscard]] QString checksum_get() const;
  [[nodiscard]] unsigned short channels_get() const;
  [[nodiscard]] QFileInfo path_get(TextureImageType ttype) const;
  [[nodiscard]] QString size_str() const;
  [[nodiscard]] QString type_str() const;
  [[nodiscard]] QFileInfo path_vmt() const;
  [[nodiscard]] QFileInfo path_stb() const;
  [[nodiscard]] QFileInfo path_vtf() const;
  [[nodiscard]] QString get_ext() const;
  [[nodiscard]] QString file_color() const;
  [[nodiscard]] QString file_normal() const;
  [[nodiscard]] QString file_displacement() const;
  [[nodiscard]] QString file_metalness() const;
  [[nodiscard]] QString file_opacity() const;
  [[nodiscard]] QString file_emission() const;
  [[nodiscard]] QString file_specular() const;
  [[nodiscard]] QString file_scattering() const;
  [[nodiscard]] QString file_roughness() const;
  [[nodiscard]] QString file_ambient_occlussion() const;
  [[nodiscard]] QString file_arm() const;
  [[nodiscard]] QString file_cache_thumbnail() const;

  [[nodiscard]] QJsonObject to_json() const;
  static QSharedPointer<TextureImage> from_json(const QJsonObject &o);
  rapidjson::Value to_rapidjson(rapidjson::Document::AllocatorType& alloc) const;

private:
  QString m_checksum;
  unsigned short m_channels = 0;
};

struct QJsonTextureImage {
  QJsonTextureImage(QString name, QMap<TextureSize, QSharedPointer<TextureImage>> textures) : name(std::move(name)), textures(std::move(textures)) {};

  QString name;
  QMap<TextureSize, QSharedPointer<TextureImage>> textures;
};