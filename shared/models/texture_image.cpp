#include "models/texture_image.h"

TextureImage::TextureImage(const TexImgInfo &tex_info, const TexImgExt ext, QObject *parent) : ext(ext), QObject(parent) {
  this->name = tex_info.name;
  this->name_original = tex_info.name_original;
  this->size = tex_info.size;
  this->type = tex_info.type;
  this->variant = tex_info.variant;

  // name_technical
  if(!tex_info.variant.isEmpty()) {
    name_technical = QString("%1_%2").arg(
      textureImageType2Str[type],
      this->variant);
  } else {
    name_technical = textureImageType2Str[type];
  }
}

void TextureImage::setTextureImageType(const TextureImageType _type) {
  this->type = _type;
}

void TextureImage::setPath(const QFileInfo &path){
  this->path = path;
  this->basedir = path.absoluteDir().path() + "/";
}

TextureImage* TextureImage::fromPath(const QFileInfo &path) {
  const auto name = path.baseName();
  const auto tex_info = TexImgInfo(name);
  const TexImgExt ext = path.suffix() == "png" ? TexImgExt::PNG : TexImgExt::JPG;
  auto *cls = new TextureImage(tex_info, ext);
  cls->setPath(path);
  return cls;
}

void TextureImage::inspect_channels_and_dimensions() {
  RawImageInfo info;
  const auto ext = path.suffix();

  if(ext == "png") {
    info = Utils::pngInfo(path);
  } else if(ext == "jpg") {
    info = Utils::jpgInfo(path);
  } else {
    qWarning() << "could not determine filetype for image inspection" << path;
    return;
  }

  if(info.success) {
    channels = info.channels;
    dimensions = {info.width, info.height};
  } else {
    qWarning() << "png/jpg inspection failed for" << path.absoluteFilePath();
  }

  isAlpha = channels == 4;
}

void TextureImage::inspect_checksum() {
  QFile file(path.absoluteFilePath());
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "could not open" << path.absoluteFilePath() << "for checksum generation";
    return;
  }
  auto part = file.read(120000);
  checksum = QString(QCryptographicHash::hash(part, QCryptographicHash::Md5).toHex());
  file.close();
}

void TextureImage::ensure_thumbnail(bool force, QString &err) {
  if (path.absoluteFilePath().isEmpty()) {
    err = "cannot create thumbnail without source path";
    qWarning() << err;
    return;
  }

  const QFileInfo path_thumb = this->path_thumbnail();
  const QString path_thumb_out = path_thumb.absoluteFilePath();
  if(path_thumb_out.isEmpty()) {
    err = "could not generate path_out";
    qWarning() << err;
    return;
  }

  if (Utils::fileExists(path_thumb_out) && !force) {
    return;
  }

  const auto width_new = 256;
  const auto height_new = 256;

  qDebug() << "writing:" << path_thumb_out;
  auto img = QImage(path.absoluteFilePath());
  img = img.scaled(height_new, width_new, Qt::KeepAspectRatio);

  bool res;
  if(isAlpha)
    res = img.save(path_thumb_out, "png", 80);
  else
    res = img.save(path_thumb_out, "jpg", 80);

  if(!res) {
    err = QString("could not save image %1").arg(path_thumb_out);
    qWarning() << err;
  } else if(!Utils::fileExists(path_thumb_out)) {
    err = QString("failed to write %1").arg(path_thumb_out);
    qWarning() << err;
  }
}

void TextureImage::metadata_generate() {
  inspect_channels_and_dimensions();
  inspect_checksum();
}

QFileInfo TextureImage::path_thumbnail() {
  if(channels == 0) {
    qWarning() << "cannot generate thumbnail path without inspecting channels for " << name;
    return {};
  }

  if (checksum.isEmpty()) {
    inspect_checksum();
    if (checksum.isEmpty()) {
      qWarning() << name << "cannot generate thumbnail path without checksum";
      return {};
    }
  }

  const QString ext = isAlpha ? "png" : "jpg";
  auto x = QFileInfo(gs::cacheDirectory + QDir::separator() + checksum + "." + ext);
  // qDebug() << x.absoluteFilePath();
  return x;
}
