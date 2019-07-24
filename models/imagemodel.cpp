#include "imagemodel.h"

ImageModel::ImageModel(QObject *parent)
  : JModel(parent)
{

}

QString ImageModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   IMAGE_SQL_COL01 ","
                   IMAGE_SQL_COL02
                   " FROM "
                   IMAGE_SQL_TABLE_NAME);
  return strQuery;
}

void ImageModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Imagem"));
  setHeaderData(2, Qt::Horizontal, tr("Imagem"));
  if (header != nullptr && header->count() == 3)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    header->hideSection(2);
  }
}

QVariant ImageModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DecorationRole)
  {
    if (idx.column() == 1)
    {
      QPixmap px(QSize(16, 16));
      px.loadFromData(QSqlQueryModel::data(createIndex(idx.row(), 2),
                                           Qt::EditRole).toByteArray());
      value = QVariant::fromValue(QIcon(px));
    }
  }
  return value;
}
