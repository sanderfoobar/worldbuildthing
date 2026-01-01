#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QHash>

class TextureTag {
public:
  explicit TextureTag(const QString &tag);

  [[nodiscard]] QString name() const;
  void set_name(const QString &tag);

  [[nodiscard]] unsigned int usage_counter() const;
  void usage_counter_set(unsigned int value);
  void usage_counter_increment() { m_usage += 1; }
  void usage_counter_decrease() { m_usage -= 1; }

private:
  QString m_name;
  unsigned int m_usage = 0;
};

class TextureTagManager final : public QObject {
Q_OBJECT

public:
  explicit TextureTagManager(QObject *parent = nullptr);
  void insert(const QSharedPointer<TextureTag> &tex);

  QSharedPointer<TextureTag> &get_or_insert(const QString &name);
  bool remove(const QString &name);
  [[nodiscard]] bool has_tag(const QString &name) const;

  QList<QSharedPointer<TextureTag>> tags() { return m_tags_sorted; }

signals:
  void tagAdded(const QString &name);
  void tagChanged(const QString &name);

private:
  QHash<QString, QSharedPointer<TextureTag>> m_tags;
  QList<QSharedPointer<TextureTag>> m_tags_sorted;
};