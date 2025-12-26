#include "tags.h"

TextureTagManager::TextureTagManager(QObject *parent) : QObject(parent) {}

QSharedPointer<TextureTag> &TextureTagManager::get_or_insert(const QString &name) {
  const QString key = name.toLower();
  if (!m_tags.contains(key)) {
    const auto ptr = QSharedPointer<TextureTag>::create(key);
    m_tags.insert(key, ptr);
    m_tags_sorted << ptr;
    emit tagAdded(key);
  }
  return m_tags[key];
}

bool TextureTagManager::remove(const QString &name) { return m_tags.remove(name.toLower()) > 0; }

bool TextureTagManager::has_tag(const QString &name) const { return m_tags.contains(name.toLower()); }

// ===

TextureTag::TextureTag(const QString &tag) : m_name(tag.toLower()) {}

QString TextureTag::name() const { return m_name; }

void TextureTag::set_name(const QString &tag) { m_name = tag.toLower(); }

unsigned int TextureTag::usage_counter() const { return m_usage; }

void TextureTag::usage_counter_set(const unsigned int value) { m_usage = value; }
