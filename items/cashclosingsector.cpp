#include "cashclosingsector.h"

CashClosingSector::CashClosingSector()
{
  CashClosingSector::clear();
}

void CashClosingSector::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_sname.clear();
  m_simage.clear();
  m_value = 0.0;
  m_nvalue = 0;
}

bool CashClosingSector::isValid() const
{
  return true;
}

bool CashClosingSector::operator !=(const JItem& other) const
{
  const CashClosingSector& another = dynamic_cast<const CashClosingSector&>(other);
  return
      m_sname != another.m_sname ||
      m_simage != another.m_simage ||
      m_value != another.m_value ||
      m_nvalue != another.m_nvalue;
}

bool CashClosingSector::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashClosingSector::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_CLOSING_SECTORS_SQL_TABLE_NAME " ("
                CASH_CLOSING_SECTORS_SQL_COL_OID ","
                CASH_CLOSING_SECTORS_SQL_COL_SNA ","
                CASH_CLOSING_SECTORS_SQL_COL_SIM ","
                CASH_CLOSING_SECTORS_SQL_COL_VAL ","
                CASH_CLOSING_SECTORS_SQL_COL_NVA
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_sname);
  query.bindValue(":_v03", m_simage.m_id.getIdNull());
  query.bindValue(":_v04", m_value);
  query.bindValue(":_v05", m_nvalue);
  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashClosingSector::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashClosingSector>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                CASH_CLOSING_SECTORS_SQL_COL_SNA ","
                CASH_CLOSING_SECTORS_SQL_COL_SIM ","
                CASH_CLOSING_SECTORS_SQL_COL_VAL ","
                CASH_CLOSING_SECTORS_SQL_COL_NVA
                " FROM " CASH_CLOSING_SECTORS_SQL_TABLE_NAME
                " WHERE " CASH_CLOSING_SECTORS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    while (query.next())
    {
      CashClosingSector o;
      o.m_id.set(query.value(0).toLongLong());
      o.m_sname = query.value(1).toString();
      o.m_simage.m_id.set(query.value(2).toLongLong());
      o.m_value = query.value(3).toDouble();
      o.m_nvalue = query.value(4).toInt();
      v.push_back(o);
    }
  }

  for (int i = 0; i != v.size() && bSuccess; ++i)
    if (v.at(i).m_simage.m_id.isValid())
      v[i].m_simage.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashClosingSector::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_CLOSING_SECTORS_SQL_COL_OID ","
                CASH_CLOSING_SECTORS_SQL_COL_SNA ","
                CASH_CLOSING_SECTORS_SQL_COL_SIM ","
                CASH_CLOSING_SECTORS_SQL_COL_VAL ","
                CASH_CLOSING_SECTORS_SQL_COL_NVA
                " FROM " CASH_CLOSING_SECTORS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_sname = query.value(1).toString();
      m_simage.m_id.set(query.value(2).toLongLong());
      m_value = query.value(3).toDouble();
      m_nvalue = query.value(4).toInt();
    }
    else
    {
      error = "Setor do fechamento de caixa não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_simage.m_id.isValid())
    m_simage.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashClosingSector::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_CLOSING_SECTORS_SQL_TABLE_NAME
                " WHERE " CASH_CLOSING_SECTORS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
