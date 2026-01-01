#pragma once

#include <QSortFilterProxyModel>

class TextureProxyModel final : public QSortFilterProxyModel {
Q_OBJECT

public:
  explicit TextureProxyModel(QObject *parent = nullptr);

  void setNameFilter(const QString &name);
  void clearNameFilter();
  void setLimit(int limit);

protected:
  bool filterAcceptsRow(int row, const QModelIndex &parent) const override;

private:
  QString m_nameFilter;
  int m_limit = 1000;
  mutable int m_count = 0;
};
