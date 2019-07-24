#include "shoppinglistmodel.h"

ShoppingListModel::ShoppingListModel(QObject *parent)
  : JModel(parent)
{

}

QString ShoppingListModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SHOPPING_LIST_SQL_TABLE_NAME "." SQL_COLID ","
                   SHOPPING_LIST_SQL_TABLE_NAME "." SHOPPING_LIST_SQL_COL03 ","
                   SHOPPING_LIST_SQL_TABLE_NAME "." SHOPPING_LIST_SQL_COL04
                   " FROM "
                   SHOPPING_LIST_SQL_TABLE_NAME);
  return strQuery;
}

void ShoppingListModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Título"));
  setHeaderData(2, Qt::Horizontal, tr("Descrição"));
  if (header != nullptr && header->count() == 3)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
  }
}
