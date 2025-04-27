#include "cashclosingmodel.h"
#include "items/cashclosing.h".h"
#include <QDate>

CashClosingModel::CashClosingModel(QObject *parent)
  : JModel(parent)
{

}

QString CashClosingModel::getStrQuery()
{
  QString str;
  str += "SELECT cc." SQL_COLID ", c." CASH_SQL_COL_NAM ", cc." CASH_CLOSING_SQL_COL_DAT " AS CASHCLOSING "
         "FROM " CASH_SQL_TABLE_NAME " AS c "
         "JOIN " CASH_CLOSING_SQL_TABLE_NAME " AS cc ON c." SQL_COLID " = cc." CASH_CLOSING_SQL_COL_CID;
  return str;
}

void CashClosingModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData((int)Column::Id, Qt::Horizontal, tr("ID"));
  setHeaderData((int)Column::Cash, Qt::Horizontal, tr("Caixa"));
  setHeaderData((int)Column::Date, Qt::Horizontal, tr("Data"));

  header->hideSection((int)Column::Id);
  header->setSectionResizeMode((int)Column::Cash, QHeaderView::ResizeMode::Stretch);
  header->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
}

QVariant CashClosingModel::data(const QModelIndex &idx, int role) const
{
  return QSqlQueryModel::data(idx, role);
}
