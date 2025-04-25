#include "cashcoin.h"

CashCoin::CashCoin()
{
  CashCoin::clear();
}

void CashCoin::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_coin.clear();
}

bool CashCoin::isValid() const
{
  return m_coin.m_id.isValid();
}

bool CashCoin::operator !=(const JItem& other) const
{
  const CashCoin& another = dynamic_cast<const CashCoin&>(other);
  return
      m_coin.m_id != another.m_coin.m_id;
}

bool CashCoin::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashCoin::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_COINS_SQL_TABLE_NAME " ("
                CASH_COINS_SQL_COL_OID ","
                CASH_COINS_SQL_COL_CID
                ") VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_coin.m_id.get());

  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashCoin::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashCoin>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                CASH_COINS_SQL_COL_CID
                " FROM " CASH_COINS_SQL_TABLE_NAME
                " WHERE " CASH_COINS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      CashCoin o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  for (int i = 0; i != v.size() && bSuccess; ++i)
    if (v.at(i).m_coin.m_id.isValid())
      bSuccess = v[i].m_coin.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashCoin::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_COINS_SQL_COL_OID ","
                CASH_COINS_SQL_COL_CID
                " FROM " CASH_COINS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_coin.m_id.set(query.value(1).toLongLong());
    }
    else
    {
      error = "Moeda do caixa não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = m_coin.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashCoin::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_COINS_SQL_TABLE_NAME
                " WHERE " CASH_COINS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
