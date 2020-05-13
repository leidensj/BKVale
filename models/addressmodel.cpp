#include "addressmodel.h"
#include "items/address.h"

AddressModel::AddressModel(QObject *parent)
  : JModel(parent)
{

}

QString AddressModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   ADDRESS_SQL_COL_STR ","
                   ADDRESS_SQL_COL_NUM ","
                   ADDRESS_SQL_COL_NEI ","
                   ADDRESS_SQL_COL_CIT ","
                   ADDRESS_SQL_COL_STA
                   " FROM "
                   ADDRESS_SQL_TABLE_NAME);
  return strQuery;
}

void AddressModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Rua"));
  setHeaderData(2, Qt::Horizontal, tr("Nº"));
  setHeaderData(3, Qt::Horizontal, tr("Bairro"));
  setHeaderData(4, Qt::Horizontal, tr("Cidade"));
  setHeaderData(5, Qt::Horizontal, tr("Estado"));
  if (header != nullptr && header->count() == 6)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
  }
}

QVariant AddressModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DisplayRole)
    if (idx.column() == 5)
      value = Address::st_getBRState((Address::EBRState)value.toInt()).m_abv;
  return value;
}

