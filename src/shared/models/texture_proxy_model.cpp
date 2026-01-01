#include "texture_proxy_model.h"
#include "texture_model.h"

TextureProxyModel::TextureProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {}

void TextureProxyModel::setNameFilter(const QString &name) {
  m_nameFilter = name.toLower();
  invalidate();
}

void TextureProxyModel::clearNameFilter() {
  m_nameFilter.clear();
  invalidate();
}

void TextureProxyModel::setLimit(int limit) {
  m_limit = limit;
  invalidate();
}

bool TextureProxyModel::filterAcceptsRow(const int row, const QModelIndex &parent) const {
  if (!sourceModel())
    return false;

  if (m_count >= m_limit)
    return false;

  const auto idx = sourceModel()->index(row, 0, parent);
  if (!idx.isValid())
    return false;

  if (!m_nameFilter.isEmpty()) {
    auto name = idx.data(TextureModel::NameLowerRole).toString();
    if (!name.contains(m_nameFilter))
      return false;
  }

  m_count++;
  return true;
}
