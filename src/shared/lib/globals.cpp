#include "globals.h"

#include "materialpropertiestemplate.h"
#include "shared/models/texture.h"
#include "shared/models/texture_model.h"
#include "shared/models/texture_proxy_model.h"
#include "globals.h"

namespace gs {
  ProgramMode programMode;
  QString configRoot;
  QString homeDir;
  QString configDirectory;
  QString configDirectoryAutoMasker;
  QString configDirectoryAssets;
  QString configDirectoryAssetsTextures;
  QString configDirectoryAssetsModels;
  QString configDirectoryAssetsU2net;
  QString cacheDirectory;
  QString cacheDirectoryTextures;
  QFileInfo pathDatabase;

  TextureTagManager *textureTagManager = nullptr;
  TextureManager *textureManager = nullptr;
  TextureModel *textureModel = nullptr;
  TextureProxyModel *textureProxyModel = nullptr;

  // std::function<bool(const QFileInfo&, const QFileInfo&, const QFileInfo&, const QString&, const QString&)>
  // FUNC_GENERATE_VMT_VTF_FILES = nullptr;
  std::function<bool(void *)> FUNC_GENERATE_VMT_VTF_FILES = nullptr;
  std::function<bool(void *)> FUNC_GENERATE_STB_FILES = nullptr;

  QMap<QString, MaterialPropertiesTemplate *> resourceTemplates = {};
  QMap<QString, QString> nameToResourceTemplateLookup = {};
} // namespace gs

QString cleanTextureTag(QString tag) {
  if (tag == "wooden")
    tag = "wood";
  else if (tag == "gray")
    tag = "grey";
  else if (tag == "mountain")
    tag = "rock";
  else if (tag == "sci" || tag == "fi")
    tag = "scifi";
  else if (tag == "stone")
    tag = "stones";
  else if (tag == "brick")
    tag = "bricks";
  else if (tag == "rock")
    tag = "rocks";
  else if (tag == "leaf")
    tag = "grass";
  else if (tag == "ground" || tag == "flooring")
    tag = "floor";
  else if (tag == "crack")
    tag = "cracks";
  return tag;
}

const QMap<TextureImageType, QString> TEXIMGTYPE2STR = {
    {TextureImageType::diffuse, "Color"},
    {TextureImageType::normal, "NormalGL"},
    {TextureImageType::displacement, "Displacement"},
    {TextureImageType::roughness, "Roughness"},
    {TextureImageType::ao, "AmbientOcclusion"},
    {TextureImageType::arm, "ARM"},
    {TextureImageType::metalness, "Metalness"},
    {TextureImageType::opacity, "Opacity"},
    {TextureImageType::emission, "Emission"},
    {TextureImageType::scattering, "Scattering"},
};
const QMap<QString, TextureImageType> STR2TEXIMGTYPE = {
    {"unknown", TextureImageType::unknown},
    {"Color", TextureImageType::diffuse},
    {"Diffuse", TextureImageType::diffuse},
    {"NormalGL", TextureImageType::normal},
    {"NormalDX", TextureImageType::normal},
    {"Displacement", TextureImageType::displacement},
    {"Roughness", TextureImageType::roughness},
    {"AmbientOcclusion", TextureImageType::ao},
    {"ARM", TextureImageType::arm},
    {"Metalness", TextureImageType::metalness},
    {"Specular", TextureImageType::specular},
    {"Opacity", TextureImageType::opacity},
    {"Emission", TextureImageType::emission},
    {"Scattering", TextureImageType::scattering},
};
const QMap<TextureSize, QString> TEXSIZE2STR = {
    {TextureSize::null, "null"}, {TextureSize::x256, "256"}, {TextureSize::x512, "512"}, {TextureSize::x768, "768"},
    {TextureSize::x1024, "1K"},  {TextureSize::x2048, "2K"}, {TextureSize::x4096, "4K"}};
const QMap<QString, TextureSize> STR2TEXSIZE = {
    {"null", TextureSize::null}, {"256", TextureSize::x256}, {"512", TextureSize::x512}, {"768", TextureSize::x768},
    {"1K", TextureSize::x1024},  {"2K", TextureSize::x2048}, {"4K", TextureSize::x4096}, {"1k", TextureSize::x1024},
    {"2k", TextureSize::x2048},  {"4k", TextureSize::x4096},
};
