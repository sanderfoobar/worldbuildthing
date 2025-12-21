#pragma once

#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <utility>

#include "lib/config.h"
#include "lib/globals.h"
#include "lib/utils.h"
#include "texture.h"

#include <GL/gl.h>

class TextureImage final : public QObject {
Q_OBJECT

public:
  uint32_t getPickingID() const { return m_pickingID; }
  void setPickingID(const uint32_t id) { m_pickingID = id; }

  GLuint getAlbedoTexture() const { return m_albedoID; }
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
  static TextureImage* fromPath(const QFileInfo &path);

  QString name;
  QString name_original;
  QString name_technical;
  TexImgExt ext;
  QString variant;

  TextureSize size = TextureSize::null;
  TextureImageType type = TextureImageType::unknown;

  bool isAlpha = false;

  QSize dimensions;
  QFileInfo path;
  AssetPack* pack = nullptr;

  QString basedir;

  QString checksum;
  unsigned short channels = 0;
  void metadata_generate();

  void ensure_thumbnail(bool force, QString &err);
  void setPack(AssetPack* p_pack);
  void setPath(const QFileInfo &path);
  void setTextureImageType(TextureImageType _type);
  QFileInfo path_thumbnail();

  void inspect_channels_and_dimensions();
  void inspect_checksum();

  // const getters/setters
  QString path_TextureImage(TextureImageType ttype) const;
  QString size_str() const;
  QString type_str() const;
  QFileInfo path_vmt() const;
  QFileInfo path_stb() const;
  QFileInfo path_vtf() const;
  QString get_ext() const;
  QString file_color() const;
  QString file_normal() const;
  QString file_displacement() const;
  QString file_metalness() const;
  QString file_opacity() const;
  QString file_emission() const;
  QString file_specular() const;
  QString file_scattering() const;
  QString file_roughness() const;
  QString file_ambient_occlussion() const;
  QString file_arm() const;
  QString file_cache_thumbnail() const;

  QJsonObject to_json() const;
};

struct QJsonTextureImage {
  QJsonTextureImage(QString name, QMap<TextureSize, QSharedPointer<TextureImage>> textures) : name(std::move(name)), textures(std::move(textures)) {};

  QString name;
  QMap<TextureSize, QSharedPointer<TextureImage>> textures;
};