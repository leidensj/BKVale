#include "sectormodel.h"

SectorModel::SectorModel(QObject *parent)
  : JModel(parent)
{

}

QString SectorModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   SECTOR_SQL_COL_NAM
                   " FROM "
                   SECTOR_SQL_TABLE_NAME);
  return strQuery;
}

void SectorModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Nome"));
  if (header != nullptr && header->count() == 2)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
  }
}
