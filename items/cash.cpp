#include "cash.h"
#include <QSqlRecord>

Cash::Cash(Id id)
{
  Cash::clear();
  m_id = id;
}

void Cash::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_vcoins.clear();
  m_vsectors.clear();
  m_vinfos.clear();
}

bool Cash::operator != (const JItem& other) const
{
  const Cash& another = dynamic_cast<const Cash&>(other);
  return m_name != another.m_name ||
         m_vcoins != another.m_vcoins ||
         m_vsectors != another.m_vsectors ||
         m_vinfos != another.m_vinfos;
}

bool Cash::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Cash::isValid() const
{
  return !m_name.isEmpty() &&
         !m_vcoins.isEmpty() &&
         !m_vsectors.isEmpty();
}

QString Cash::SQL_tableName() const
{
  return CASH_SQL_TABLE_NAME;
}

bool Cash::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_SQL_TABLE_NAME " ("
                CASH_SQL_COL_NAM
                ") VALUES ("
                "(:_v01))");
  query.bindValue(":_v01", m_name);

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

bool Cash::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " CASH_SQL_TABLE_NAME " SET "
                CASH_SQL_COL_NAM " = (:_v01)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_name);

  bool ok = query.exec();
  if (ok)
  {
    ok = CashCoin::SQL_remove_by_owner_id_proc(query, m_id);
    if (ok)
      ok = CashSector::SQL_remove_by_owner_id_proc(query, m_id);
    if (ok)
      ok = CashInfo::SQL_remove_by_owner_id_proc(query, m_id);
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

bool Cash::SQL_select_proc(QSqlQuery& query, QString& error)
{
  query.prepare("SELECT "
                CASH_SQL_COL_NAM
                " FROM " CASH_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();

  if (ok)
  {
    if (query.next())
    {
      m_name = query.value(0).toString();
    }
    else
    {
      error = "Caixa não encontrado.";
      ok = false;
    }
  }

  if (ok)
    ok = CashCoin::SQL_select_by_owner_id_proc(query, m_id, m_vcoins, error);
  if (ok)
    ok = CashSector::SQL_select_by_owner_id_proc(query, m_id, m_vsectors, error);
      if (ok)
    ok = CashInfo::SQL_select_by_owner_id_proc(query, m_id, m_vinfos, error);

  return ok;
}

bool Cash::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " CASH_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
