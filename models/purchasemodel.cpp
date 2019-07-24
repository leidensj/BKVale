#include "purchasemodel.h"
#include <QDate>

PurchaseModel::PurchaseModel(QObject *parent)
  : JModel(parent)
{

}

QString PurchaseModel::getStrQuery()
{
  QString strQuery("SELECT "
                   NOTE_SQL_TABLE_NAME "." SQL_COLID ","
                   NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL01 ","
                   NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL02 ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL03 ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                                                          "(COALESCE(_TTOTAL._TSUBTOTAL,0) + " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL05 ")"
                                                                                                                                      " FROM " NOTE_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   "(SELECT " NOTE_ITEMS_SQL_COL01 ","
                                                   "COALESCE(SUM(" NOTE_ITEMS_SQL_COL03 "*" NOTE_ITEMS_SQL_COL04 "), 0) AS _TSUBTOTAL"
                                                                                                                 " FROM " NOTE_ITEMS_SQL_TABLE_NAME
                   " GROUP BY " NOTE_ITEMS_SQL_COL01 ") AS _TTOTAL"
                                                     " ON " NOTE_SQL_TABLE_NAME "." SQL_COLID "= _TTOTAL." NOTE_ITEMS_SQL_COL01
                   " LEFT OUTER JOIN "
                   SUPPLIER_SQL_TABLE_NAME
                   " ON " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03 "=" SUPPLIER_SQL_TABLE_NAME "." SQL_COLID
                   " LEFT OUTER JOIN "
                   FORM_SQL_TABLE_NAME
                   " ON " SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL01 " = " FORM_SQL_TABLE_NAME "." SQL_COLID);
  return strQuery;
}

void PurchaseModel::select(QHeaderView* header)
{
  JModel::select(getStrQuery());
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Número"));
  setHeaderData(2, Qt::Horizontal, tr("Data"));
  setHeaderData(3, Qt::Horizontal, tr("Fornecedor"));
  setHeaderData(4, Qt::Horizontal, tr("Fornecedor"));
  setHeaderData(5, Qt::Horizontal, tr("Total"));
  if (header != nullptr)
  {
    if (header->count() == 6)
    {
      header->hideSection(0);
      header->hideSection(4);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
    }
  }
}

QVariant PurchaseModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DisplayRole)
  {
    if (idx.column() == 2)
      value = QDate::fromString(value.toString(), Qt::ISODate).toString("yyyy/MM/dd");
    else if (idx.column() == 3)
    {
      if (value.toString().isEmpty())
        value = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 1), role);
    }
    else if (idx.column() == 5)
      value = "R$ " + QString::number(value.toDouble(), 'f', 2);
  }
  return value;
}
