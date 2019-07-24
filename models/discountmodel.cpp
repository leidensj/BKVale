#include "discountmodel.h"
#include "items/discount.h"

DiscountModel::DiscountModel(QObject *parent)
  : JModel(parent)
{

}

QString DiscountModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   DISCOUNT_SQL_COL01 ","
                   DISCOUNT_SQL_COL04 ","
                   DISCOUNT_SQL_COL08 ","
                   DISCOUNT_SQL_COL07
                   " FROM "
                   DISCOUNT_SQL_TABLE_NAME);
  return strQuery;
}

void DiscountModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Código"));
  setHeaderData(2, Qt::Horizontal, tr("Tipo"));
  setHeaderData(3, Qt::Horizontal, tr("Descrição"));
  setHeaderData(4, Qt::Horizontal, tr("Regatado"));
  if (header != nullptr && header->count() == 5)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
  }
}

QVariant DiscountModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QModelIndex();

  QVariant value = QSqlQueryModel::data(index, role);
  if (role == Qt::DisplayRole)
  {
    if (index.column() == 2)
      value = Discount::strType((Discount::Type)value.toInt());
    else if (index.column() == 4)
      value = value.toBool() ? tr("Sim") : "";
  }
  return value;
}
