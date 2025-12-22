#include "shared/models/texture.h"
#include "shared/models/asset_pack.h"
#include "shared/models/texture_image_getters_setters.h"

#include "shared/lib/materialpropertiestemplate.h"

QMap<QString, QPixmap> pixmapCache = {};

Texture::Texture(const QString &name, QObject *parent) :
    name(name),
    name_lower(name.toLower()),
    QObject(parent) {
  //
}

QFileInfo Texture::path_thumbnail() {
  for(const auto tsize: {
      TextureSize::x1024,
      TextureSize::x2048,
      TextureSize::x4096,
      TextureSize::x768,
      TextureSize::x512,
      TextureSize::x256}) {
    if(diffuse.contains(tsize)) {
      const QSharedPointer<TextureImage> tex = diffuse[tsize];
      if (const QFileInfo path = tex->path_thumbnail(); path.exists())
        return path;
    }
  }

  qWarning() << "no thumbnail found for" << this->name;
  return {};
}

void Texture::setMaterialPropertiesTemplate() {
  auto camelToSnake = [](const QString &camel) {
    QString snake;
    snake.reserve(camel.size());
    for (QChar c : camel) {
      if (c.isUpper()) {
        if (!snake.isEmpty()) snake += '_';
        snake += c.toLower();
      } else {
        snake += c;
      }
    }
    return snake;
  };

  auto removeNumbers = [](const QString& input) {
    QString result;
    result.reserve(input.size());
    for (QChar c : input) {
      if (!c.isDigit()) result += c;
    }
    return result;
  };

  auto name_resource_keys = gs::nameToResourceTemplateLookup.keys();

  // from name
  const auto snake = camelToSnake(this->name);
  for (const auto no_numbered_snake = removeNumbers(snake);
       const auto& dismembered_snake: no_numbered_snake.split("_")) {
    if (gs::nameToResourceTemplateLookup.contains(dismembered_snake)) {
      m_resourceTemplateID = gs::nameToResourceTemplateLookup[dismembered_snake];
      this->material_properties_template = gs::resourceTemplates[m_resourceTemplateID];
      return;
    }
  }

  // from name
  for (const auto& tmpl_name: name_resource_keys) {
    if (this->name_lower.startsWith(tmpl_name)) {
      m_resourceTemplateID = gs::nameToResourceTemplateLookup[tmpl_name];
      this->material_properties_template = gs::resourceTemplates[m_resourceTemplateID];
      return;
    }
  }

  // from name... :(
  for (const auto& tmpl_name: name_resource_keys) {
    if (this->name_lower.contains(tmpl_name)) {
      m_resourceTemplateID = gs::nameToResourceTemplateLookup[tmpl_name];
      this->material_properties_template = gs::resourceTemplates[m_resourceTemplateID];
      break;
    }
  }

  // from tags
  if (m_resourceTemplateID.isEmpty()) {
    for (const auto& tag: this->tags().keys()) {
      if (gs::nameToResourceTemplateLookup.contains(tag)) {
        m_resourceTemplateID = gs::nameToResourceTemplateLookup[tag];
        this->material_properties_template = gs::resourceTemplates[m_resourceTemplateID];
        break;
      }
    }
  }

  material_properties_template = gs::resourceTemplates["default"];
}

// QPixmap Texture::thumbnail() {
//   auto cache_dir = config()->get(ConfigKeys::CacheDir).toString();

//   if(pixmapCache.contains(this->name))
//     return pixmapCache[this->name];

//   for(const auto tsize: {
//       TextureSize::x1024,
//       TextureSize::x2048,
//       TextureSize::x4096,
//       TextureSize::x768,
//       TextureSize::x512,
//       TextureSize::x256}) {
//     if(diffuse.contains(tsize)) {
//       auto tex = diffuse[tsize];
//       auto cached_file = tex->path_thumbnail();
//       if(!cache_dir.isEmpty() && Utils::fileExists(cached_file)) {
//         auto pixmap = QPixmap(cached_file);
//         pixmapCache[this->name] = pixmap;
//         return pixmap;
//       }
//     }
//   }
//   qWarning() << "no thumbnail found for" << this->name;
//   return {};
// }