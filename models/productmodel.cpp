#include "productmodel.h"

ProductModel::ProductModel(QObject *parent)
  : JModel(parent)
{

}

QString ProductModel::getStrQuery()
{
  QString strQuery("SELECT "
                   PRODUCT_SQL_TABLE_NAME "." SQL_COLID ","
                   PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_NAM ","
                   PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_UNI ","
                   CATEGORY_SQL_TABLE_NAME "." CATEGORY_SQL_COL_NAM
                   " FROM "
                   PRODUCT_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   CATEGORY_SQL_TABLE_NAME
                   " ON "
                   PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_CID
                   " = "
                   CATEGORY_SQL_TABLE_NAME "." SQL_COLID);
  return strQuery;
}

void ProductModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Nome"));
  setHeaderData(2, Qt::Horizontal, tr("Unidade"));
  setHeaderData(3, Qt::Horizontal, tr("Categoria"));
  if (header != nullptr && header->count() == 4)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
  }
}
