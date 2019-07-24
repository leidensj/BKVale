#include "suppliermodel.h"

SupplierModel::SupplierModel(QObject *parent)
  : JModel(parent)
{

}

QString SupplierModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SUPPLIER_SQL_TABLE_NAME "." SQL_COLID ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL03
                   " FROM " SUPPLIER_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   FORM_SQL_TABLE_NAME
                   " ON " SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL01 " = "
                   FORM_SQL_TABLE_NAME "." SQL_COLID);
  return strQuery;
}

void SupplierModel::select(QHeaderView* header)
{
  JModel::select(getStrQuery());
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Nome"));
  setHeaderData(2, Qt::Horizontal, tr("Apelido"));
  if (header != nullptr)
  {
    if (header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    }
  }
}
