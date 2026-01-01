#pragma once

#include <QAbstractListModel>
#include <QSharedPointer>

class Texture;

class TextureModel final : public QAbstractListModel {
Q_OBJECT

public:
  enum Role { NameRole = Qt::UserRole + 1, NameLowerRole, TagsRole, ThumbRole, TexturePtrRole };

  explicit TextureModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  QSharedPointer<Texture> entry(int row) const;

public slots:
  void refresh();

private:
  QList<QSharedPointer<Texture>> m_items;
};
