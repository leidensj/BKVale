#include "purchasemodel.h"
#include "common.h"
#include <QDate>

PurchaseModel::PurchaseModel(QObject *parent)
  : JModel(parent)
{

}

QString PurchaseModel::getStrQuery()
{
  QString strQuery("SELECT "
                   PURCHASE_SQL_TABLE_NAME "." SQL_COLID ","
                   PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_NUM ","
                   PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL_ALI ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL_NAM ","
                   "(COALESCE(_TTOTAL._TSUBTOTAL,0) + " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DIS ")"
                   " FROM " PURCHASE_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   "(SELECT " PURCHASE_ELEMENTS_SQL_COL_NID ","
                   "COALESCE(SUM(" PURCHASE_ELEMENTS_SQL_COL_AMM "*" PURCHASE_ELEMENTS_SQL_COL_PRI "), 0) AS _TSUBTOTAL"
                   " FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                   " GROUP BY " PURCHASE_ELEMENTS_SQL_COL_NID ") AS _TTOTAL"
                   " ON " PURCHASE_SQL_TABLE_NAME "." SQL_COLID "= _TTOTAL." PURCHASE_ELEMENTS_SQL_COL_NID
                   " LEFT OUTER JOIN "
                   SUPPLIER_SQL_TABLE_NAME
                   " ON " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID "=" SUPPLIER_SQL_TABLE_NAME "." SQL_COLID
                   " LEFT OUTER JOIN "
                   FORM_SQL_TABLE_NAME
                   " ON " SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL_FID " = " FORM_SQL_TABLE_NAME "." SQL_COLID);
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
      value = Data::strMoney(value.toDouble());
  }
  return value;
}
