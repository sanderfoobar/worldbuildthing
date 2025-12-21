#pragma once
#include "texture.h"
#include "texture_image.h"

inline QString TextureImage::file_color() const { return QString("%1_%2_Color." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_normal() const { return QString("%1_%2_NormalGL." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_displacement() const { return QString("%1_%2_Displacement." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_metalness() const { return QString("%1_%2_Metalness." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_opacity() const { return QString("%1_%2_Opacity." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_emission() const { return QString("%1_%2_Emission." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_specular() const { return QString("%1_%2_Specular." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_scattering() const { return QString("%1_%2_Scattering." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_roughness() const { return QString("%1_%2_Roughness." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_ambient_occlussion() const {
  return QString("%1_1K_AmbientOcclusion." + get_ext()).arg(name, getTextureSize(size));
}
inline QString TextureImage::file_arm() const { return QString("%1_%2_ARM." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_cache_thumbnail() const {
  return QString("%1-%2.%3").arg(name, checksum, this->is_alpha ? "png" : "jpg");
}

inline QFileInfo TextureImage::path_vtf() const {
  const auto _basedir =
      gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "vmtvtf" + QDir::separator();
  std::filesystem::create_directories(_basedir.toStdString());
  return QFileInfo(_basedir + QDir::separator() + this->name_original + ".vtf");
}

inline QFileInfo TextureImage::path_vmt() const {
  const auto _basedir =
      gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "vmtvtf" + QDir::separator();
  std::filesystem::create_directories(_basedir.toStdString());
  return QFileInfo(_basedir + QDir::separator() + this->name_original + ".vmt");
}

inline QFileInfo TextureImage::path_stb() const {
  const auto _basedir = gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "stb" + QDir::separator();
  std::filesystem::create_directories(_basedir.toStdString());
  return QFileInfo(_basedir + QDir::separator() + this->name_original + ".stb");
}

inline QString TextureImage::size_str() const {
  if (TEXSIZE2STR.contains(size))
    return TEXSIZE2STR[size];
  return "not sure";
}

inline QString TextureImage::type_str() const {
  if (TEXIMGTYPE2STR.contains(type))
    return TEXIMGTYPE2STR[type];
  return "not sure";
}

inline QString TextureImage::get_ext() const {
  if (ext == TexImgExt::PNG)
    return "png";
  else if (ext == TexImgExt::JPG)
    return "jpg";
  else
    return "unknown";
}

inline QFileInfo TextureImage::path_get(TextureImageType ttype) const {
  QString fn;
  if (ttype == TextureImageType::diffuse)
    fn = file_color();
  else if (ttype == TextureImageType::normal)
    fn = file_normal();
  else if (ttype == TextureImageType::displacement)
    fn = file_displacement();
  else if (ttype == TextureImageType::roughness)
    fn = file_roughness();
  else if (ttype == TextureImageType::ao)
    fn = file_ambient_occlussion();
  else if (ttype == TextureImageType::arm)
    fn = file_arm();
  else if (ttype == TextureImageType::metalness)
    fn = file_metalness();
  else if (ttype == TextureImageType::opacity)
    fn = file_opacity();
  else if (ttype == TextureImageType::emission)
    fn = file_emission();
  else if (ttype == TextureImageType::specular)
    fn = file_specular();
  else if (ttype == TextureImageType::scattering)
    fn = file_scattering();

  const auto path = QString("%1/%2").arg(basedir, fn);
  if (Utils::fileExists(path))
    return QFileInfo(path);
  qWarning() << path << "not found";
  return {};
}

inline QJsonObject TextureImage::to_json() const {
  QJsonObject obj;
  if (!checksum.isEmpty())
    obj["checksum"] = checksum;
  if (channels > 0)
    obj["channels"] = channels;
  if (size != TextureSize::null)
    obj["size"] = static_cast<int>(size);
  return obj;
}

inline void TextureImage::setPack(AssetPack *p_pack) { pack = p_pack; }

inline void TextureImage::setTextureImageType(const TextureImageType _type) {
  this->type = _type;
}

inline void TextureImage::setPath(const QFileInfo &path) {
  this->path = path;
  this->basedir = path.absoluteDir().path() + "/";
}