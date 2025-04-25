#include "cashmodel.h"
#include "items/cash.h"

CashModel::CashModel(QObject *parent)
  : JModel(parent)
{

}

QString CashModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   CASH_SQL_COL_NAM
                   " FROM "
                   CASH_SQL_TABLE_NAME);
  return strQuery;
}

void CashModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData((int)Column::Id, Qt::Horizontal, tr("ID"));
  setHeaderData((int)Column::Name, Qt::Horizontal, tr("Nome"));

  header->hideSection((int)Column::Id);
  header->setSectionResizeMode((int)Column::Name, QHeaderView::ResizeMode::Stretch);
}

QVariant CashModel::data(const QModelIndex &idx, int role) const
{
  return QSqlQueryModel::data(idx, role);
}
