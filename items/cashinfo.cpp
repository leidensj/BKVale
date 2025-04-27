#include "cashinfo.h"

CashInfo::CashInfo()
{
  CashInfo::clear();
}

void CashInfo::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_type = Data::Type::Money;
}

bool CashInfo::isValid() const
{
  return !m_name.isEmpty();
}

bool CashInfo::operator !=(const JItem& other) const
{
  const CashInfo& another = dynamic_cast<const CashInfo&>(other);
  return
      m_name != another.m_name;
}

bool CashInfo::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashInfo::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_INFO_SQL_TABLE_NAME " ("
                CASH_INFO_SQL_COL_OID ","
                CASH_INFO_SQL_COL_NAM ","
                CASH_INFO_SQL_COL_TYP
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", (int)m_type);

  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashInfo::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashInfo>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                CASH_INFO_SQL_COL_NAM ","
                CASH_INFO_SQL_COL_TYP
                " FROM " CASH_INFO_SQL_TABLE_NAME
                " WHERE " CASH_INFO_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      CashInfo o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  return bSuccess;
}

bool CashInfo::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_INFO_SQL_COL_OID ","
                CASH_INFO_SQL_COL_NAM ","
                CASH_INFO_SQL_COL_TYP
                " FROM " CASH_INFO_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_name = query.value(1).toString();
      m_type = (Data::Type) query.value(2).toInt();
    }
    else
    {
      error = "Informação do caixa não encontrado.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool CashInfo::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_INFO_SQL_TABLE_NAME
                " WHERE " CASH_INFO_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
