#include "productcodemodel.h"

ProductCodeModel::ProductCodeModel(QObject *parent)
  : JModel(parent)
{

}

QString ProductCodeModel::getStrQuery()
{
  QString strQuery("SELECT "
                   PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "." SQL_COLID ","
                   PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "." PRODUCT_CODE_ITEMS_SQL_COL02 ","
                   PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL01
                   " FROM "
                   PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   PRODUCT_SQL_TABLE_NAME
                   " ON " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "." PRODUCT_CODE_ITEMS_SQL_COL01
                   " = " PRODUCT_SQL_TABLE_NAME "." SQL_COLID);
  return strQuery;
}

void ProductCodeModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Código"));
  setHeaderData(2, Qt::Horizontal, tr("Produto"));
  if (header != nullptr && header->count() == 3)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
  }
}
