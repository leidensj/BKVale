#include "coinmodel.h"
#include "common.h"

CoinModel::CoinModel(QObject *parent)
  : JModel(parent)
{

}

QString CoinModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   COIN_SQL_COL_NAM ","
                   COIN_SQL_COL_TAX
                   " FROM "
                   COIN_SQL_TABLE_NAME);
  return strQuery;
}

void CoinModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Nome"));
  setHeaderData(2, Qt::Horizontal, tr("Taxa"));
  if (header != nullptr && header->count() == 3)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
  }
}

QVariant CoinModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DisplayRole)
  {
    if (idx.column() == (int)Column::Tax)
      value = Data::strPercentage(value.toDouble());
  }
  return value;
}
