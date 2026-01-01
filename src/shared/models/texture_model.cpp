#include "texture_model.h"
#include "texture.h"
#include "texture_getters_setters.h"
#include "texture_manager.h"

TextureModel::TextureModel(QObject *parent) : QAbstractListModel(parent) { refresh(); }

void TextureModel::refresh() {
  beginResetModel();
  m_items = gs::textureManager->all();
  endResetModel();
}

int TextureModel::rowCount(const QModelIndex &) const { return m_items.size(); }

QVariant TextureModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return {};

  const auto &tex = m_items.at(index.row());

  switch (role) {
    case NameRole:
      return tex->name;
    case NameLowerRole:
      return tex->name_lower;
    case TagsRole:
      return tex->tags_as_variant();
    case ThumbRole: {
      const auto path = tex->path_thumbnail();
      if (path.exists())
        return path.fileName();
    }
    case TexturePtrRole:
      return QVariant::fromValue(tex.data());
    default:
      return {};
  }
}

QHash<int, QByteArray> TextureModel::roleNames() const {
  return {{NameRole, "name"},
          {NameLowerRole, "name_lower"},
          {TagsRole, "tags"},
          {ThumbRole, "thumb"},
          {TexturePtrRole, "texture"}};
}

QSharedPointer<Texture> TextureModel::entry(int row) const {
  if (row < 0 || row >= m_items.size())
    return {};

  return m_items.at(row);
}
