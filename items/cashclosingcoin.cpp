#include "cashclosingcoin.h"

CashClosingCoin::CashClosingCoin()
{
  CashClosingCoin::clear();
}

void CashClosingCoin::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_cname.clear();
  m_ctax = 0.0;
  m_cimage.clear();
  m_value = 0.0;
}

bool CashClosingCoin::isValid() const
{
  return true;
}

bool CashClosingCoin::operator !=(const JItem& other) const
{
  const CashClosingCoin& another = dynamic_cast<const CashClosingCoin&>(other);
  return
      m_cname != another.m_cname ||
      m_ctax != another.m_ctax ||
      m_ctax != another.m_ctax ||
      m_cimage != another.m_cimage ||
      m_value != another.m_value;
}

bool CashClosingCoin::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashClosingCoin::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_CLOSING_COINS_SQL_TABLE_NAME " ("
                CASH_CLOSING_COINS_SQL_COL_OID ","
                CASH_CLOSING_COINS_SQL_COL_CNA ","
                CASH_CLOSING_COINS_SQL_COL_CTA ","
                CASH_CLOSING_COINS_SQL_COL_CIM ","
                CASH_CLOSING_COINS_SQL_COL_VAL
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_cname);
  query.bindValue(":_v03", m_ctax);
  query.bindValue(":_v04", m_cimage.m_id.getIdNull());
  query.bindValue(":_v05", m_value);

  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashClosingCoin::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashClosingCoin>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                CASH_CLOSING_COINS_SQL_COL_CNA ","
                CASH_CLOSING_COINS_SQL_COL_CTA ","
                CASH_CLOSING_COINS_SQL_COL_CIM ","
                CASH_CLOSING_COINS_SQL_COL_VAL
                " FROM " CASH_CLOSING_COINS_SQL_TABLE_NAME
                " WHERE " CASH_CLOSING_COINS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    while (query.next())
    {
      CashClosingCoin o;
      o.m_id.set(query.value(0).toLongLong());
      o.m_cname = query.value(1).toString();
      o.m_ctax = query.value(2).toDouble();
      o.m_cimage.m_id.set(query.value(3).toLongLong());
      o.m_value = query.value(4).toDouble();
      v.push_back(o);
    }
  }

  for (int i = 0; i != v.size() && bSuccess; ++i)
    if (v.at(i).m_cimage.m_id.isValid())
      v[i].m_cimage.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashClosingCoin::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_CLOSING_COINS_SQL_COL_OID ","
                CASH_CLOSING_COINS_SQL_COL_CNA ","
                CASH_CLOSING_COINS_SQL_COL_CTA ","
                CASH_CLOSING_COINS_SQL_COL_CIM ","
                CASH_CLOSING_COINS_SQL_COL_VAL
                " FROM " CASH_CLOSING_COINS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_cname = query.value(1).toString();
      m_ctax = query.value(2).toDouble();
      m_cimage.m_id.set(query.value(3).toLongLong());
      m_value = query.value(4).toDouble();
    }
    else
    {
      error = "Moeda do fechamento de caixa não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_cimage.m_id.isValid())
    m_cimage.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashClosingCoin::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_CLOSING_COINS_SQL_TABLE_NAME
                " WHERE " CASH_CLOSING_COINS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}

double CashClosingCoin::valueWithTaxes() const
{
  return m_value * (1 - m_ctax/100);
}

double CashClosingCoin::taxesDifference() const
{
  return m_value - valueWithTaxes();
}
