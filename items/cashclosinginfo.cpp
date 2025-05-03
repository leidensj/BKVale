#include "cashclosinginfo.h"

CashClosingInfo::CashClosingInfo()
{
  CashClosingInfo::clear();
}

void CashClosingInfo::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_iname.clear();
  m_itype = 0;
  m_value = 0;
}

bool CashClosingInfo::isValid() const
{
  return true;
}

bool CashClosingInfo::operator !=(const JItem& other) const
{
  const CashClosingInfo& another = dynamic_cast<const CashClosingInfo&>(other);
  return
      m_iname != another.m_iname ||
      m_itype != another.m_itype ||
      m_value != another.m_value;
}

bool CashClosingInfo::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashClosingInfo::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_CLOSING_INFOS_SQL_TABLE_NAME " ("
                CASH_CLOSING_INFOS_SQL_COL_OID ","
                CASH_CLOSING_INFOS_SQL_COL_INA ","
                CASH_CLOSING_INFOS_SQL_COL_ITY ","
                CASH_CLOSING_INFOS_SQL_COL_VAL
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_iname);
  query.bindValue(":_v03", m_itype);
  query.bindValue(":_v04", m_value.toString());

  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashClosingInfo::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashClosingInfo>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                CASH_CLOSING_INFOS_SQL_COL_INA ","
                CASH_CLOSING_INFOS_SQL_COL_ITY ","
                CASH_CLOSING_INFOS_SQL_COL_VAL
                " FROM " CASH_CLOSING_INFOS_SQL_TABLE_NAME
                " WHERE " CASH_CLOSING_INFOS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    while (query.next())
    {
      CashClosingInfo o;
      o.m_id.set(query.value(0).toLongLong());
      o.m_iname = query.value(1).toString();
      o.m_itype = query.value(2).toInt();
      o.m_value = query.value(3);
      v.push_back(o);
    }
  }

  return bSuccess;
}

bool CashClosingInfo::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_CLOSING_INFOS_SQL_COL_OID ","
                CASH_CLOSING_INFOS_SQL_COL_INA ","
                CASH_CLOSING_INFOS_SQL_COL_ITY ","
                CASH_CLOSING_INFOS_SQL_COL_VAL
                " FROM " CASH_CLOSING_INFOS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_iname = query.value(1).toString();
      m_itype = query.value(2).toInt();
      m_value = query.value(3);
    }
    else
    {
      error = "Informação do fechamento de caixa não encontrada.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool CashClosingInfo::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_CLOSING_INFOS_SQL_TABLE_NAME
                " WHERE " CASH_CLOSING_INFOS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}

QString CashClosingInfo::strValue() const
{
  return ((Data::Type)m_itype == Data::Type::Text) ? m_value.toString() : Data::str(m_value.toDouble(), (Data::Type)m_itype);
}
