#include "categorymodel.h"

CategoryModel::CategoryModel(QObject *parent)
  : JModel(parent)
{

}

QString CategoryModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   CATEGORY_SQL_COL_NAM
                   " FROM "
                   CATEGORY_SQL_TABLE_NAME);
  return strQuery;
}

void CategoryModel::select(QHeaderView* header)
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
