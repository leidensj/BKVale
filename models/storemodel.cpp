#include "storemodel.h"

StoreModel::StoreModel(QObject *parent)
  : JModel(parent)
{

}

QString StoreModel::getStrQuery()
{
  QString strQuery("SELECT "
                   STORE_SQL_TABLE_NAME "." SQL_COLID ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL03 ","
                   STORE_SQL_TABLE_NAME "." STORE_SQL_COL04
                   " FROM "
                   STORE_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   FORM_SQL_TABLE_NAME
                   " ON " STORE_SQL_TABLE_NAME "." STORE_SQL_COL01
                   " = " FORM_SQL_TABLE_NAME "." SQL_COLID);
  return strQuery;
}

void StoreModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Razão Social"));
  setHeaderData(2, Qt::Horizontal, tr("Nome Fantasia"));
  setHeaderData(3, Qt::Horizontal, tr("Descrição"));
  if (header != nullptr && header->count() == 4)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
  }
}
