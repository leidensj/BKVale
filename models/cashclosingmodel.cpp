#include "cashclosingmodel.h"
#include "items/cashclosing.h"
#include <QDate>

CashClosingModel::CashClosingModel(QObject *parent)
  : JModel(parent)
{

}

QString CashClosingModel::getStrQuery()
{
  QString str;
  str += "SELECT cc._id, cc._date, c._name, s.svalue as sales, co.covalue as gross, ROUND(co.cofvalue::numeric,2) as net, ROUND(co.tax::numeric,2) as tax,  ROUND((co.covalue - s.svalue)::numeric,2) as discrepancy, ROUND((co.cofvalue - s.svalue)::numeric,2) as difference  FROM "
         "_CASH_CLOSINGS AS cc LEFT JOIN _CASH AS c on cc._CASHID = c._id "
         "LEFT JOIN (SELECT _CASHCLOSINGID as ccid, SUM(_VALUE) as svalue FROM _CASH_CLOSING_SECTORS GROUP BY ccid) AS s on s.ccid = cc._id "
         "LEFT JOIN (SELECT _CASHCLOSINGID as ccid, SUM(_VALUE) as covalue, SUM(_VALUE*(1-_COINTAX/100)) as cofvalue, SUM(_VALUE - _VALUE*(1-_COINTAX/100)) as tax FROM _CASH_CLOSING_COINS GROUP BY ccid) AS co on co.ccid = cc._id";
  return str;
}

void CashClosingModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData((int)Column::Id, Qt::Horizontal, tr("ID"));
  setHeaderData((int)Column::Date, Qt::Horizontal, tr("Data"));
  setHeaderData((int)Column::Cash, Qt::Horizontal, tr("Caixa"));
  setHeaderData((int)Column::Sales, Qt::Horizontal, tr("Vendas"));
  setHeaderData((int)Column::Gross, Qt::Horizontal, tr("Bruto"));
  setHeaderData((int)Column::Net, Qt::Horizontal, tr("Líquido"));
  setHeaderData((int)Column::Tax, Qt::Horizontal, tr("Taxas"));
  setHeaderData((int)Column::Discrepancy, Qt::Horizontal, tr("Quebra"));
  setHeaderData((int)Column::Difference, Qt::Horizontal, tr("Diferença"));

  header->hideSection((int)Column::Id);
  header->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Cash, QHeaderView::ResizeMode::Stretch);
  header->setSectionResizeMode((int)Column::Sales, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Gross, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Net, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Tax, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Discrepancy, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Difference, QHeaderView::ResizeMode::ResizeToContents);
}

QVariant CashClosingModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DisplayRole)
    if (idx.column() == (int)Column::Date)
      value = value.toDateTime().toLocalTime();
  return value;
}
