#include "texture_manager.h"
#include "texture.h"

TextureManager::TextureManager(QObject *parent) : QObject(parent) {}

QSharedPointer<Texture> TextureManager::get(const QString &name) const { return m_textures.value(name); }

QSharedPointer<Texture> TextureManager::get_lower(const QString &name_lower) const {
  return m_textures_lower.value(name_lower);
}

QList<QSharedPointer<Texture>> TextureManager::all() const { return m_flat; }

void TextureManager::add(const QSharedPointer<Texture> &texture) {
  if (!texture)
    return;

  m_textures.insert(texture->name, texture);
  m_textures_lower.insert(texture->name_lower, texture);
  m_flat.append(texture);
}

void TextureManager::remove(const QString &name) {
  const auto tex = m_textures.take(name);
  if (!tex)
    return;

  m_textures_lower.remove(tex->name_lower);
  m_flat.removeAll(tex);
}

void TextureManager::clear() {
  m_textures.clear();
  m_textures_lower.clear();
  m_flat.clear();
}
