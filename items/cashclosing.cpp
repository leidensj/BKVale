#include "cashclosing.h"
#include <QSqlRecord>
#include "common.h"

CashClosing::CashClosing(Id id)
{
  CashClosing::clear();
  m_id = id;
}

void CashClosing::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_cash.clear();
  m_dt = DateTime::server();
  m_vcoins.clear();
  m_vsectors.clear();
  m_vinfos.clear();
}

bool CashClosing::operator != (const JItem& other) const
{
  const CashClosing& another = dynamic_cast<const CashClosing&>(other);
  return m_cash != another.m_cash ||
         m_vcoins != another.m_vcoins ||
         m_vsectors != another.m_vsectors ||
         m_vinfos != another.m_vinfos;
}

bool CashClosing::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool CashClosing::isValid() const
{
  return m_cash.m_id.isValid();
}

QString CashClosing::SQL_tableName() const
{
  return CASH_CLOSING_SQL_TABLE_NAME;
}

bool CashClosing::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_CLOSING_SQL_TABLE_NAME " ("
                CASH_CLOSING_SQL_COL_CID ", "
                CASH_CLOSING_SQL_COL_DAT
                ") VALUES ("
                "(:_v01) ,"
                "(:_v02))");
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);

  bool ok = query.exec();
  if (ok)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vcoins.size() && ok; ++i)
    {
      m_vcoins[i].m_ownerId = m_id;
      ok = m_vcoins.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vsectors.size() && ok; ++i)
    {
      m_vsectors[i].m_ownerId = m_id;
      ok = m_vsectors.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vinfos.size() && ok; ++i)
    {
      m_vinfos[i].m_ownerId = m_id;
      ok = m_vinfos.at(i).SQL_insert_proc(query);
    }
  }
  return ok;
}

bool CashClosing::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " CASH_CLOSING_SQL_TABLE_NAME " SET "
                CASH_CLOSING_SQL_COL_CID " = (:_v01),"
                CASH_CLOSING_SQL_COL_DAT " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);

  bool ok = query.exec();
  if (ok)
  {
    ok = CashClosingCoin::SQL_remove_by_owner_id_proc(query, m_id);
    if (ok)
      ok = CashClosingSector::SQL_remove_by_owner_id_proc(query, m_id);
    if (ok)
      ok = CashClosingInfo::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_vcoins.size() && ok; ++i)
    {
      m_vcoins[i].m_ownerId = m_id;
      ok = m_vcoins.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vsectors.size() && ok; ++i)
    {
      m_vsectors[i].m_ownerId = m_id;
      ok = m_vsectors.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vinfos.size() && ok; ++i)
    {
      m_vinfos[i].m_ownerId = m_id;
      ok = m_vinfos.at(i).SQL_insert_proc(query);
    }
  }
  return ok;
}

bool CashClosing::SQL_select_proc(QSqlQuery& query, QString& error)
{
  query.prepare("SELECT "
                CASH_CLOSING_SQL_COL_CID ", "
                CASH_CLOSING_SQL_COL_DAT ", "
                " FROM " CASH_CLOSING_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();

  if (ok)
  {
    if (query.next())
    {
      m_cash.m_id.set(query.value(0).toLongLong());
      m_dt = query.value(1).toDateTime();
    }
    else
    {
      error = "Fechamento de Caixa não encontrado.";
      ok = false;
    }
  }

  if (ok)
    ok = CashClosingCoin::SQL_select_by_owner_id_proc(query, m_id, m_vcoins, error);
  if (ok)
    ok = CashClosingSector::SQL_select_by_owner_id_proc(query, m_id, m_vsectors, error);
      if (ok)
    ok = CashClosingInfo::SQL_select_by_owner_id_proc(query, m_id, m_vinfos, error);

  return ok;
}

bool CashClosing::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " CASH_CLOSING_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
