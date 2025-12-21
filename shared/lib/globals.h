#pragma once
#include <QObject>
#include <QDirIterator>
#include <QDebug>
#include <QFileInfo>

class MaterialPropertiesTemplate;

namespace gs {
  extern QString configRoot;
  extern QString homeDir;
  extern QString configDirectory;
  extern QString configDirectoryAutoMasker;
  extern QString configDirectoryAssets;
  extern QString configDirectoryAssetsTextures;
  extern QString configDirectoryAssetsModels;
  extern QString configDirectoryAssetsU2net;
  extern QString cacheDirectory;
  extern QFileInfo pathDatabase;

  extern std::function<bool(void*)> FUNC_GENERATE_VMT_VTF_FILES;
  extern std::function<bool(void*)> FUNC_GENERATE_STB_FILES;

  extern QMap<QString, MaterialPropertiesTemplate*> resourceTemplates;
  extern QMap<QString, QString> nameToResourceTemplateLookup;
}

enum TextureLicense {
  cc0 = 1,
  royalty_free
};

enum class TextureSize {
  null = 0,
  x256,
  x512,
  x768,
  x1024,
  x2048,
  x4096
};

enum class TextureImageType {
  unknown = 0,
  diffuse,
  roughness,
  ao,
  arm,
  displacement,
  normal,
  metalness,
  opacity,
  emission,
  scattering,  // subsurface scattering
  specular,
  textureImageTypeCount
};

enum class TexImgExt {
  PNG,
  JPG
};

struct TextureTag {
  explicit TextureTag(QString tag) {
    name = tag.toLower();
  }

  QString name = "";
  unsigned int score = 0;
};

struct PackedFile {
  QString fileName;
  QByteArray data;
};

struct RawImageInfo
{
  unsigned short channels;
  unsigned short height;
  unsigned short width;
  bool success;
};

QString cleanTextureTag(QString tag);
extern QMap<QString, QSharedPointer<TextureTag>> CACHE_TEXTURE_TAGS;

extern const QMap<QString, TextureSize> STR2TEXSIZE;
extern const QMap<TextureSize, QString> TEXSIZE2STR;

extern const QMap<QString, TextureImageType> STR2TEXIMGTYPE;
extern const QMap<TextureImageType, QString> TEXIMGTYPE2STR;

inline QString getTextureSize(const TextureSize &tsize) {
  if (TEXSIZE2STR.contains(tsize))
    return TEXSIZE2STR.value(tsize);
  return "UNKNOWN";
}
inline TextureSize getTextureSize(const QString &name) {
  if (STR2TEXSIZE.contains(name))
    return STR2TEXSIZE.value(name);
  return TextureSize::null;
}

extern const QMap<QString, TextureImageType> STR2TEXIMGTYPE;
extern const QMap<TextureImageType, QString> TEXIMGTYPE2STR;
inline QString getTextureImageType(const TextureImageType &ttype) {
  if (TEXIMGTYPE2STR.contains(ttype))
    return TEXIMGTYPE2STR.value(ttype);
  return "UNKNOWN";
}
inline TextureImageType getTextureImageType(const QString &name) {
  QString key = name;
  if (!key.isEmpty())
    key[0] = key[0].toUpper();
  if (STR2TEXIMGTYPE.contains(key))
    return STR2TEXIMGTYPE.value(key);
  return TextureImageType::unknown;
}

struct TexImgInfo {
  explicit TexImgInfo(const QString &name) {
    this->name_original = name;
    auto spl = name.split("_");
    if (spl.size() < 3) {
      this->errorString = "split('_') < 3";
      qWarning() << "skipping" << name << this->errorString;
      success = false;
      return;
    }

    std::ranges::reverse(spl);
    const QString &ttype_str = spl[0];
    const QString &tsize_str = spl[1];
    QStringList name_spl = spl.mid(2);
    std::ranges::reverse(name_spl);
    this->name = name_spl.join('_');

    if(STR2TEXSIZE.contains(tsize_str))
      this->size = STR2TEXSIZE[tsize_str];
    else
      this->size = TextureSize::null;

    if(STR2TEXIMGTYPE.contains(ttype_str))
      this->type = STR2TEXIMGTYPE[ttype_str];
    else {
      this->errorString = "unknown ttype " + ttype_str;
      qDebug() << "skipping" << name << this->errorString;
      success = false;
      return;
    }

    if (spl.length() == 4)
      variant = spl.at(3);
    if (spl.length() == 5) {
      variant = QString("%1_%2").arg(spl.at(3), spl.at(4));
    }
  }

  QString name = "";
  QString name_original = "";
  TextureSize size;
  TextureImageType type;
  QString variant = "";
  bool success = true;
  QString errorString = "";
};

Q_DECLARE_METATYPE(TextureImageType)

namespace gs {
  // QMap<QString, QString> parseResource(const QString& inp) {
  //   QMap<QString, QString> options;
  //   QStringList lines = inp.split('\n', Qt::SkipEmptyParts);
  //
  //   for (const QString &line : lines) {
  //     QString trimmed = line.trimmed();
  //     if (trimmed.startsWith('[') || trimmed.isEmpty()) continue;
  //
  //     int eqIndex = trimmed.indexOf('=');
  //     if (eqIndex > 0) {
  //       QString key = trimmed.left(eqIndex).trimmed();
  //       QString value = trimmed.mid(eqIndex + 1).trimmed();
  //       options[key] = value;
  //     }
  //   }
  //
  //   return options;
  // }
}