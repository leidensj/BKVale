#include "inventorymodel.h"

InventoryModel::InventoryModel(QObject *parent)
  : JModel(parent)
{

}

QString InventoryModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   INVENTORY_SQL_COL_DAT
                   INVENTORY_SQL_COL_DES
                   " FROM "
                   INVENTORY_SQL_TABLE_NAME);
  return strQuery;
}

void InventoryModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Data"));
  setHeaderData(2, Qt::Horizontal, tr("Descrição"));
  if (header != nullptr && header->count() == 2)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
  }
}
