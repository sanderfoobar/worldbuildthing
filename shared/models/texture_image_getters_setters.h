#pragma once
#include "texture.h"
#include "texture_image.h"

inline QString TextureImage::file_color() const { return QString("%1_1K-Color." + get_ext()).arg(name); }
inline QString TextureImage::file_normal() const { return QString("%1_1K-NormalGL." + get_ext()).arg(name); }
inline QString TextureImage::file_displacement() const { return QString("%1_1K-Displacement." + get_ext()).arg(name); }
inline QString TextureImage::file_metalness() const { return QString("%1_1K-Metalness." + get_ext()).arg(name); }
inline QString TextureImage::file_opacity() const { return QString("%1_1K-Opacity." + get_ext()).arg(name); }
inline QString TextureImage::file_emission() const { return QString("%1_1K-Emission." + get_ext()).arg(name); }
inline QString TextureImage::file_specular() const { return QString("%1_1K-Specular." + get_ext()).arg(name); }
inline QString TextureImage::file_scattering() const { return QString("%1_1K-Scattering." + get_ext()).arg(name); }
inline QString TextureImage::file_roughness() const { return QString("%1_1K-Roughness." + get_ext()).arg(name); }
inline QString TextureImage::file_ambient_occlussion() const {
  return QString("%1_1K-AmbientOcclusion." + get_ext()).arg(name);
}
inline QString TextureImage::file_arm() const { return QString("%1_1K-ARM." + get_ext()).arg(name); }
inline QString TextureImage::file_cache_thumbnail() const {
  return QString("%1-%2.%3").arg(name, checksum, this->isAlpha ? "png" : "jpg");
}

inline QFileInfo TextureImage::path_vtf() const {
  auto _basedir = gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "vmtvtf" + QDir::separator();
  std::filesystem::create_directories(_basedir.toStdString());
  return QFileInfo(_basedir + QDir::separator() + this->name_original + ".vtf");
}

inline QFileInfo TextureImage::path_vmt() const {
  auto _basedir = gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "vmtvtf" + QDir::separator();
  std::filesystem::create_directories(_basedir.toStdString());
  return QFileInfo(_basedir + QDir::separator() + this->name_original + ".vmt");
}

inline QFileInfo TextureImage::path_stb() const {
  auto _basedir = gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "stb" + QDir::separator();
  std::filesystem::create_directories(_basedir.toStdString());
  return QFileInfo(_basedir + QDir::separator() + this->name_original + ".stb");
}

inline QString TextureImage::size_str() const {
  if(textureSize2Str.contains(size))
    return textureSize2Str[size];
  return "not sure";
}

inline QString TextureImage::type_str() const {
  if(textureImageType2Str.contains(type))
    return textureImageType2Str[type];
  return "not sure";
}

inline QString TextureImage::get_ext() const {
  if(ext == TexImgExt::PNG) return "png";
  else if(ext == TexImgExt::JPG) return "jpg";
  else return "unknown";
}

inline QString TextureImage::path_TextureImage(TextureImageType ttype) const {
  QString fn;
  if(ttype == TextureImageType::diffuse)
    fn = file_color();
  else if(ttype == TextureImageType::normal)
    fn = file_normal();
  else if(ttype == TextureImageType::displacement)
    fn = file_displacement();
  else if(ttype == TextureImageType::roughness)
    fn = file_roughness();
  else if(ttype == TextureImageType::ao)
    fn = file_ambient_occlussion();
  else if(ttype == TextureImageType::arm)
    fn = file_arm();
  else if(ttype == TextureImageType::metalness)
    fn = file_metalness();
  else if(ttype == TextureImageType::opacity)
    fn = file_opacity();
  else if(ttype == TextureImageType::emission)
    fn = file_emission();
  else if(ttype == TextureImageType::specular)
    fn = file_specular();
  else if(ttype == TextureImageType::scattering)
    fn = file_scattering();

  auto path = QString("%1/1k/%2").arg(basedir, fn);
  if(Utils::fileExists(path))
    return path;
  qWarning() << path << "not found";
  return {};
}

inline QJsonObject TextureImage::to_json() const {
  QJsonObject obj;
  if(!checksum.isEmpty())
    obj["checksum"] = checksum;
  if(channels > 0)
    obj["channels"] = channels;
  if(size != TextureSize::null)
    obj["size"] = size;
  return obj;
}

inline void TextureImage::setPack(AssetPack* p_pack) { pack = p_pack; }