#include "cashsector.h"

CashSector::CashSector()
{
  CashSector::clear();
}

void CashSector::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_sector.clear();
}

bool CashSector::isValid() const
{
  return m_sector.m_id.isValid();
}

bool CashSector::operator !=(const JItem& other) const
{
  const CashSector& another = dynamic_cast<const CashSector&>(other);
  return
      m_sector.m_id != another.m_sector.m_id;
}

bool CashSector::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashSector::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_SECTORS_SQL_TABLE_NAME " ("
                CASH_SECTORS_SQL_COL_OID ","
                CASH_SECTORS_SQL_COL_SID
                ") VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_sector.m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashSector::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashSector>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                CASH_SECTORS_SQL_COL_SID
                " FROM " CASH_SECTORS_SQL_TABLE_NAME
                " WHERE " CASH_SECTORS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      CashSector o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  for (int i = 0; i != v.size() && bSuccess; ++i)
    if (v.at(i).m_sector.m_id.isValid())
      bSuccess = v[i].m_sector.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashSector::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_SECTORS_SQL_COL_OID ","
                CASH_SECTORS_SQL_COL_SID
                " FROM " CASH_SECTORS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_sector.m_id.set(query.value(1).toLongLong());
    }
    else
    {
      error = "Setor do caixa não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = m_sector.SQL_select_proc(query, error);

  return bSuccess;
}

bool CashSector::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_SECTORS_SQL_TABLE_NAME
                " WHERE " CASH_SECTORS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
