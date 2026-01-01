#pragma once
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "shared/lib/globals.h"
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
  return QString("%1_%2_AmbientOcclusion." + get_ext()).arg(name, getTextureSize(size));
}
inline QString TextureImage::file_arm() const { return QString("%1_%2_ARM." + get_ext()).arg(name, getTextureSize(size)); }
inline QString TextureImage::file_cache_thumbnail() const {
  return QString("%1-%2.%3").arg(name, m_checksum, this->is_alpha ? "png" : "jpg");
}

inline QFileInfo TextureImage::path_vtf() const {
  // const auto _basedir =
  //     gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "vmtvtf" + QDir::separator();
  // std::filesystem::create_directories(_basedir.toStdString());
  // return QFileInfo(_basedir + QDir::separator() + this->name_original + ".vtf");
  return {};
}

inline QFileInfo TextureImage::path_vmt() const {
  // const auto _basedir =
  //     gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "vmtvtf" + QDir::separator();
  // std::filesystem::create_directories(_basedir.toStdString());
  // return QFileInfo(_basedir + QDir::separator() + this->name_original + ".vmt");
  return {};
}

inline QFileInfo TextureImage::path_stb() const {
  // const auto _basedir = gs::cacheDirectory + QDir::separator() + pack->name() + QDir::separator() + "stb" + QDir::separator();
  // std::filesystem::create_directories(_basedir.toStdString());
  // return QFileInfo(_basedir + QDir::separator() + this->name_original + ".stb");
  return {};
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

inline QString TextureImage::checksum_get() const { return m_checksum; }
inline unsigned short TextureImage::channels_get() const { return m_channels; }

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
  if (!m_checksum.isEmpty())
    obj["checksum"] = m_checksum;
  if (m_channels > 0)
    obj["channels"] = m_channels;
  if (size != TextureSize::null)
    obj["size"] = static_cast<int>(size);
  return obj;
}

inline rapidjson::Value TextureImage::to_rapidjson(rapidjson::Document::AllocatorType& alloc) const {
  rapidjson::Value obj(rapidjson::kObjectType);
  obj.AddMember("name", rapidjson::Value(name.toUtf8().constData(), alloc), alloc);
  obj.AddMember("name_original", rapidjson::Value(name_original.toUtf8().constData(), alloc), alloc);
  obj.AddMember("name_lower", rapidjson::Value(name_lower.toUtf8().constData(), alloc), alloc);
  obj.AddMember("name_technical", rapidjson::Value(name_technical.toUtf8().constData(), alloc), alloc);
  obj.AddMember("variant", rapidjson::Value(variant.toUtf8().constData(), alloc), alloc);
  obj.AddMember("filename", rapidjson::Value(path.fileName().toUtf8().constData(), alloc), alloc);

  if (!m_checksum.isEmpty())
    obj.AddMember("checksum", rapidjson::Value(m_checksum.toUtf8().constData(), alloc), alloc);
  if (m_channels > 0)
    obj.AddMember("channels", m_channels, alloc);
  if (size != TextureSize::null)
    obj.AddMember("size", static_cast<int>(size), alloc);
  return obj;
}

inline void TextureImage::setTextureImageType(const TextureImageType _type) {
  this->type = _type;
}

inline void TextureImage::set_channels(unsigned short channels) { m_channels = channels; }
inline void TextureImage::set_checksum(const QString& checksum) { m_checksum = checksum; }

inline void TextureImage::set_path(const QFileInfo &path) {
  this->path = path;
  this->basedir = path.absoluteDir().path() + "/";
}

inline QSharedPointer<TextureImage> TextureImage::from_json(const QJsonObject &o) {
  auto tex_ext = o["ext"].toString().toLower() == "jpg" ? TexImgExt::JPG : TexImgExt::PNG;
  const auto tex_name = o["name"].toString();
  const auto tex_info = TexImgInfo(tex_name);

  auto tex_img = QSharedPointer<TextureImage>::create(tex_info, tex_ext);
  tex_img->name = tex_name;
  tex_img->name = tex_name.toLower();
  tex_img->name_original = o["name_original"].toString();
  tex_img->name_technical = o["name_technical"].toString();
  tex_img->variant = o["variant"].toString();
  tex_img->is_alpha = o["is_alpha"].toBool();
  tex_img->set_checksum(o["checksum"].toString());
  tex_img->set_channels(static_cast<unsigned short>(o["channels"].toInt()));

  tex_img->dimensions = QSize(
    o["width"].toInt(),
    o["height"].toInt()
  );

  tex_img->size = getTextureSize(o["size"].toString());
  tex_img->type = getTextureImageType(o["type"].toString());
  tex_img->ext = o["ext"].toString().toLower() == "jpg" ? TexImgExt::JPG : TexImgExt::PNG;

  tex_img->basedir = o["basedir"].toString();
  tex_img->path = QFileInfo(o["path"].toString());

  return tex_img;
}