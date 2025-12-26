#pragma once
#include "shared/lib/materialpropertiestemplate.h"
#include "texture.h"
#include "texture_image.h"

inline QVariantList Texture::tags_as_variant() {
  return m_tags_as_variants;
}

inline void Texture::set_author(const QString &author) {
  m_author = author;
}

inline QString Texture::get_author() {
  return m_author;
}

inline void Texture::set_license(const QString &license) {
  m_license = license;
}

inline QString Texture::get_license() {
  return m_license;
}

inline void Texture::set_tags(const QSet<QSharedPointer<TextureTag>> &tags) {
  for (const auto &tag : tags) {
    const auto tag_name = tag->name();
    m_tags_as_variants << tag->name();
  }

  m_tags = tags;
}

inline void Texture::append_tag(const QSharedPointer<TextureTag> &tag) {
  const auto tag_name = tag->name();
  m_tags << tag;
  m_tags_as_variants << tag_name;
  tag->usage_counter_increment();
}

inline QSharedPointer<TextureImage> Texture::get_diffuse(const TextureSize size, const bool fuzzy) const {
  if(diffuse.contains(size))
    return diffuse[size];

  if(!fuzzy)
    return nullptr;

  for(const auto tsize: {
    TextureSize::x1024,
    TextureSize::x4096,
    TextureSize::x2048,
    TextureSize::x768,
    TextureSize::x512,
    TextureSize::x256}) {
    if(diffuse.contains(size))
      return diffuse[size];
    }

  return nullptr;
}

inline QSharedPointer<TextureImage> Texture::get_image(TextureImageType type ,TextureSize size) const {
  if(type == TextureImageType::diffuse) {
    if(diffuse.contains(size))
      return diffuse[size];
  } else if(type == TextureImageType::displacement) {
    if(displacement.contains(size))
      return displacement[size];
  } else if(type == TextureImageType::roughness) {
    if(roughness.contains(size))
      return roughness[size];
  } else if(type == TextureImageType::normal) {
    if(normal.contains(size))
      return normal[size];
  } else if(type == TextureImageType::ao) {
    if(ao.contains(size))
      return ao[size];
  } else if(type == TextureImageType::arm) {
    if(arm.contains(size))
      return arm[size];
  } else if(type == TextureImageType::metalness) {
    if(metalness.contains(size))
      return metalness[size];
  } else if(type == TextureImageType::opacity) {
    if(opacity.contains(size))
      return opacity[size];
  } else if(type == TextureImageType::emission) {
    if(emission.contains(size))
      return emission[size];
  } else if(type == TextureImageType::specular) {
    if(specular.contains(size))
      return specular[size];
  } else if(type == TextureImageType::scattering) {
    if(scattering.contains(size))
      return scattering[size];
  } else {
    qWarning() << "unknown weird type" << static_cast<int>(type);
  }

  return nullptr;
}

inline void Texture::set_texture(const QSharedPointer<TextureImage> &tex) {
  // @TODO: support variants
  if(tex->type == TextureImageType::diffuse) {
    this->setDiffuse(tex->size, tex);
  } else if(tex->type == TextureImageType::displacement) {
    this->setDisplacement(tex->size, tex);
  } else if(tex->type == TextureImageType::roughness) {
    this->setRoughness(tex->size, tex);
  } else if(tex->type == TextureImageType::normal) {
    this->setNormal(tex->size, tex);
  } else if(tex->type == TextureImageType::ao) {
    this->setAO(tex->size, tex);
  } else if(tex->type == TextureImageType::arm) {
    this->setARM(tex->size, tex);
  } else if(tex->type == TextureImageType::metalness) {
    this->setMetalness(tex->size, tex);
  } else if(tex->type == TextureImageType::opacity) {
    this->setOpacity(tex->size, tex);
  } else if(tex->type == TextureImageType::emission) {
    this->setEmission(tex->size, tex);
  } else if(tex->type == TextureImageType::specular) {
    this->setSpecular(tex->size, tex);
  } else if(tex->type == TextureImageType::scattering) {
    this->setScattering(tex->size, tex);
  } else {
    qWarning() << "uhm weird type" << static_cast<int>(tex->type);
  }
}

inline QList<TextureSize> Texture::available_sizes() const {
  return this->diffuse.keys();
}

inline void Texture::addVariant(TextureSize tsize, const QString &variant, const QSharedPointer<TextureImage> &tex) {
  variants[tsize][variant] = tex;
  textures << tex;
}

inline void Texture::setDiffuse(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  diffuse[tsize] = tex;
  textures << tex;
}

inline void Texture::setRoughness(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  roughness[tsize] = tex;
  textures << tex;
}

inline void Texture::setAO(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  ao[tsize] = tex;
  textures << tex;
}

inline void Texture::setARM(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  arm[tsize] = tex;
  textures << tex;
}

inline void Texture::setDisplacement(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  displacement[tsize] = tex;
  textures << tex;
}

inline void Texture::setNormal(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  normal[tsize] = tex;
  textures << tex;
}

inline void Texture::setMetalness(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  metalness[tsize] = tex;
  textures << tex;
}

inline void Texture::setOpacity(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  opacity[tsize] = tex;
  textures << tex;
}

inline void Texture::setEmission(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  emission[tsize] = tex;
  textures << tex;
}

inline void Texture::setSpecular(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  specular[tsize] = tex;
  textures << tex;
}

inline void Texture::setScattering(TextureSize tsize, const QSharedPointer<TextureImage> &tex) {
  scattering[tsize] = tex;
  textures << tex;
}

inline QJsonObject Texture::to_json() {
  QJsonObject o;

  // o["id"] = m_id;
  o["name"] = name;
  if(m_author.size()) o["author"] = m_author;
  if(m_license.size()) o["license"] = m_license;

  o["name_lower"] = name_lower;
  o["num_pixels"] = static_cast<int>(num_pixels);

  auto packImages = [](const QMap<TextureSize, QSharedPointer<TextureImage>> &src) {
    QJsonObject out;
    for(auto it = src.begin(); it != src.end(); ++it)
      if(it.value())
        out[TEXSIZE2STR[it.key()]] = it.value()->to_json();
    return out;
  };

  QJsonObject images;
  if(!diffuse.isEmpty()) images["diffuse"] = packImages(diffuse);
  if(!normal.isEmpty()) images["normal"] = packImages(normal);
  if(!roughness.isEmpty()) images["roughness"] = packImages(roughness);
  if(!metalness.isEmpty()) images["metalness"] = packImages(metalness);
  if(!ao.isEmpty()) images["ao"] = packImages(ao);
  if(!arm.isEmpty()) images["arm"] = packImages(arm);
  if(!displacement.isEmpty()) images["displacement"] = packImages(displacement);
  if(!opacity.isEmpty()) images["opacity"] = packImages(opacity);
  if(!emission.isEmpty()) images["emission"] = packImages(emission);
  if(!specular.isEmpty()) images["specular"] = packImages(specular);
  if(!scattering.isEmpty()) images["scattering"] = packImages(scattering);

  if(!variants.isEmpty()) {
    QJsonObject v;
    for(auto it = variants.begin(); it != variants.end(); ++it) {
      QJsonObject vv;
      for(auto vit = it.value().begin(); vit != it.value().end(); ++vit)
        if(vit.value())
          vv[vit.key()] = vit.value()->to_json();
      v[TEXSIZE2STR[it.key()]] = vv;
    }
    images["variants"] = v;
  }

  o["images"] = images;
  return o;
}

inline QSharedPointer<Texture> from_json(const QJsonObject &o) {
  auto tex = QSharedPointer<Texture>::create(o["name"].toString());
  tex->name_lower = o["name_lower"].toString();
  tex->num_pixels = o["num_pixels"].toInt();

  tex->set_author(o["author"].toString());
  tex->set_license(o["license"].toString());

  for(const auto &t: o["tags"].toArray()) {
    auto tag_name = t.toString();
    if (gs::textureTagManager->has_tag(tag_name))
      tex->append_tag(gs::textureTagManager->get_or_insert(tag_name));

    auto tag = QSharedPointer<TextureTag>::create(tag_name);
    tex->append_tag(tag);
  }

  const auto images = o["images"].toObject();

  auto unpackImages =
    [&](const QString &key, auto setter) {
      const auto group = images[key].toObject();
      for(auto it = group.begin(); it != group.end(); ++it) {
        auto size = getTextureSize(it.key());
        auto img = TextureImage::from_json(it.value().toObject());
        if(img)
          setter(size, img);
      }
    };

  unpackImages("diffuse",      [&](auto s, const auto &i){ tex->setDiffuse(s,i); });
  unpackImages("normal",       [&](auto s, const auto &i){ tex->setNormal(s,i); });
  unpackImages("roughness",    [&](auto s, const auto &i){ tex->setRoughness(s,i); });
  unpackImages("metalness",    [&](auto s, const auto &i){ tex->setMetalness(s,i); });
  unpackImages("ao",           [&](auto s, const auto &i){ tex->setAO(s,i); });
  unpackImages("arm",          [&](auto s, const auto &i){ tex->setARM(s,i); });
  unpackImages("displacement", [&](auto s, const auto &i){ tex->setDisplacement(s,i); });
  unpackImages("opacity",      [&](auto s, const auto &i){ tex->setOpacity(s,i); });
  unpackImages("emission",     [&](auto s, const auto &i){ tex->setEmission(s,i); });
  unpackImages("specular",     [&](auto s, const auto &i){ tex->setSpecular(s,i); });
  unpackImages("scattering",   [&](auto s, const auto &i){ tex->setScattering(s,i); });

  if(images.contains("variants")) {
    const auto vars = images["variants"].toObject();
    for(auto sit = vars.begin(); sit != vars.end(); ++sit) {
      const auto size = getTextureSize(sit.key());
      const auto vv = sit.value().toObject();
      for(auto vit = vv.begin(); vit != vv.end(); ++vit) {
        auto img = TextureImage::from_json(vit.value().toObject());
        if(img)
          tex->addVariant(size, vit.key(), img);
      }
    }
  }

  return tex;
}

inline QSharedPointer<Texture> from_json(const QByteArray &bytes) {
  QJsonParseError err;
  const auto doc = QJsonDocument::fromJson(bytes, &err);
  if(err.error != QJsonParseError::NoError || !doc.isObject())
    return nullptr;

  return from_json(doc.object());
}

inline QString Texture::to_tres(TextureSize tsize) {
  auto tex = this;
  const auto diffuse = tex->get_image(TextureImageType::diffuse, tsize);
  if (diffuse.isNull()) {
    qWarning() << "diffuse is null for tex" << tex->name;
    return "";
  }

  //const QString asset_pack = tex->asset_pack()->name();
  QString asset_pack = "a";
  QString texname = tex->name;

  bool isAlpha = diffuse->is_alpha;
  auto arm = tex->get_image(TextureImageType::arm, tsize);
  auto spec = tex->get_image(TextureImageType::specular, tsize);
  auto metal = tex->get_image(TextureImageType::metalness, tsize);
  auto roughness = tex->get_image(TextureImageType::roughness, tsize);
  auto ao = tex->get_image(TextureImageType::ao, tsize);
  auto displacement = tex->get_image(TextureImageType::displacement, tsize);
  auto normal = tex->get_image(TextureImageType::normal, tsize);
  auto opacity = tex->get_image(TextureImageType::opacity, tsize);
  auto emission = tex->get_image(TextureImageType::emission, tsize);
  auto scattering = tex->get_image(TextureImageType::scattering, tsize);

  struct Resource {
    QSharedPointer<TextureImage> img;
    QString uid;
  };

  QMap<TextureImageType, Resource> resource_ids;

  QString output;
  QTextStream out(&output);

  out << "[gd_resource type=\"StandardMaterial3D\" load_steps=${STEPS} format=3 template=\"${TEMPLATE}\"]\n\n";

  auto genID = [](const int idx, const QString &filename) {
    const QByteArray hash = QCryptographicHash::hash(filename.toUtf8(), QCryptographicHash::Md5)
      .toHex().left(5);
    return QString::number(idx) + "_" + hash;
  };

  int idx = 1;
  for (const auto &img: tex->textures) {
    auto filename = img->path.fileName();
    auto uid = genID(idx, filename);
    QString header = QString("[ext_resource type=\"Texture2D\" path=\"res://textures/%1/%2\" id=\"%3\"]\n").arg(
      asset_pack).arg(filename).arg(uid);
    out << header;

    resource_ids[img->type] = Resource{.img = img, .uid = uid};
    idx += 1;
  }

  out << "\n[resource]\n";

  auto tmpl = this->material_properties_template;
  QMap<QString, QString> options;

  // produce .tres body
  options["albedo_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::diffuse].uid);

  if (!arm.isNull()) {
    // ao
    options["ao_enabled"] = "true";
    options["ao_light_affect"] = QString::number(tmpl->aoLightAffect(), 'f', 1);
    options["ao_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::arm].uid);
    // roughness
    options["roughness_texture_channel"] = "1";
    options["roughness"] = QString::number(tmpl->roughness(), 'f', 1);
    options["roughness_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::arm].uid);
    // metalness
    options["metallic_texture_channel"] = "2";
    options["metallic"] = QString::number(tmpl->metallic(), 'f', 1);
    options["metallic_specular"] = QString::number(tmpl->metallicSpecular(), 'f', 1);
    options["metallic_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::arm].uid);
  }

  if (arm.isNull() && !ao.isNull()) {
    options["ao_enabled"] = "true";
    options["ao_light_affect"] = QString::number(tmpl->aoLightAffect(), 'f', 1);
    options["ao_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::ao].uid);
  }

  if (arm.isNull() && !roughness.isNull()) {
    options["roughness"] = QString::number(tmpl->roughness(), 'f', 1);
    options["roughness_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::roughness].uid);
  }

  if (arm.isNull() && !metal.isNull()) {
    options["metallic"] = QString::number(tmpl->metallic(), 'f', 1);
    options["metallic_specular"] = QString::number(tmpl->metallicSpecular(), 'f', 1);
    options["metallic_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::metalness].uid);
  }

  if (!normal.isNull()) {
    options["normal_enabled"] = "true";
    options["normal_scale"] = QString::number(tmpl->normalScale(), 'f', 1);
    options["normal_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::normal].uid);
  }

  if (arm.isNull() && roughness.isNull()) {
    options["roughness"] = "0.8";
  }

  if (arm.isNull() && metal.isNull() && !spec.isNull()) {
    options["metallic"] = QString::number(tmpl->metallic(), 'f', 1);
    options["metallic_specular"] = QString::number(tmpl->metallicSpecular(), 'f', 1);
    options["metallic_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::specular].uid);
  }

  if (!emission.isNull()) {
    options["emission_enabled"] = "true";
    options["emission_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::emission].uid);
  }

  if (!scattering.isNull()) {
    options["subsurf_scatter_enabled"] = "true";
    if (texname.contains("skin"))
      options["subsurf_scatter_skin_mode"] = "true";
    options["subsurf_scatter_texture"] = QString("ExtResource(\"%1\")").arg(resource_ids[TextureImageType::scattering].uid);
  }

  if (isAlpha) {
    // alpha from diffuse, A channel
    options["transparency"] = "2";
    options["alpha_scissor_threshold"] = "0.5";
    options["alpha_antialiasing_mode"] = "1";
    options["alpha_antialiasing_edge"] = "0.01";
    options["cull_mode"] = "2";
  }

  for (const auto &[key, value] : options.toStdMap()) {
    auto line = QString("%1 = %2\n").arg(key, value);
    out << line;
  }

  output = output.replace("${STEPS}", QString::number(resource_ids.keys().size() + 1));
  output = output.replace("${TEMPLATE}", m_resourceTemplateID);

  return output;
}