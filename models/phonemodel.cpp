#include "phonemodel.h"

PhoneModel::PhoneModel(QObject *parent)
  : JModel(parent)
{

}

QString PhoneModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   PHONE_SQL_COL_COU ","
                   PHONE_SQL_COL_COD ","
                   PHONE_SQL_COL_NUM ","
                   PHONE_SQL_COL_NAM
                   " FROM "
                   PHONE_SQL_TABLE_NAME);
  return strQuery;
}

void PhoneModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("País"));
  setHeaderData(2, Qt::Horizontal, tr("Código"));
  setHeaderData(3, Qt::Horizontal, tr("Número"));
  setHeaderData(4, Qt::Horizontal, tr("Nome"));
  if (header != nullptr && header->count() == 5)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch);
  }
}
