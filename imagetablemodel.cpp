#include "imagetablemodel.h"
#include "image.h"
#include <QPixmap>
#include <QIcon>

ImageTableModel::ImageTableModel(QObject *parent, QSqlDatabase db)
  : QSqlTableModel(parent, db)
{

}

QVariant ImageTableModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlTableModel::data(idx, role);
  if (role == Qt::DecorationRole)
  {
    if (idx.column() == (int)Image::Column::Name)
    {
      QPixmap px(QSize(16, 16));
      px.loadFromData(QSqlTableModel::data(createIndex(idx.row(), (int)Image::Column::Image),
                                           Qt::EditRole).toByteArray());
      value = QVariant::fromValue(QIcon(px));
    }
  }
  return value;
}
