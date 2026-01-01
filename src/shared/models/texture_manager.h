#pragma once

#include <QObject>
#include <QHash>
#include <QList>
#include <QSharedPointer>
#include <QString>

class Texture;
class TextureManager final : public QObject {
Q_OBJECT

public:
  explicit TextureManager(QObject *parent = nullptr);

  QSharedPointer<Texture> get(const QString& name) const;
  QSharedPointer<Texture> get_lower(const QString& name_lower) const;

  QList<QSharedPointer<Texture>> all() const;

  void add(const QSharedPointer<Texture>& texture);
  void remove(const QString& name);
  void clear();

private:
  QHash<QString, QSharedPointer<Texture>> m_textures;
  QHash<QString, QSharedPointer<Texture>> m_textures_lower;
  QList<QSharedPointer<Texture>> m_flat;
};
