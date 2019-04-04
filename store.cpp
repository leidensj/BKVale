#include "store.h"

Store::Store()
{
  clear();
}

void Store::clear()
{
  m_form.clear();
  m_address.clear();
  m_phone.clear();
  m_description.clear();
}

bool Store:: operator != (const JItem& other) const
{
  const Store& another = dynamic_cast<const Store&>(other);
  return m_form.m_id != another.m_form.m_id ||
         m_address.m_id != another.m_address.m_id ||
         m_phone.m_id != another.m_phone.m_id ||
         m_description != another.m_description;
}

bool Store:: operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Store::isValid() const
{
  return m_form.m_id.isValid();
}

QString Store::SQL_tableName() const
{
  return STORE_SQL_TABLE_NAME;
}

bool Store::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " STORE_SQL_TABLE_NAME " ("
                STORE_SQL_COL01 ","
                STORE_SQL_COL02 ","
                STORE_SQL_COL03 ","
                STORE_SQL_COL04 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04))");

  query.bindValue(":_v01", m_form.m_id.getIdNull());
  query.bindValue(":_v02", m_address.m_id.getIdNull());
  query.bindValue(":_v03", m_phone.m_id.getIdNull());
  query.bindValue(":_v04", m_description);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Store::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " STORE_SQL_TABLE_NAME " SET "
                STORE_SQL_COL01 " = (:_v01),"
                STORE_SQL_COL02 " = (:_v02),"
                STORE_SQL_COL03 " = (:_v03),"
                STORE_SQL_COL04 " = (:_v04)"
                " WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_form.m_id.getIdNull());
  query.bindValue(":_v02", m_address.m_id.getIdNull());
  query.bindValue(":_v03", m_phone.m_id.getIdNull());
  query.bindValue(":_v04", m_description);
  return query.exec();
}

bool Store::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                STORE_SQL_COL01 ","
                STORE_SQL_COL02 ","
                STORE_SQL_COL03 ","
                STORE_SQL_COL04
                " FROM " STORE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_form.m_id = query.value(0).toLongLong();
      m_address.m_id = query.value(1).toLongLong();
      m_phone.m_id = query.value(2).toLongLong();
      m_description = query.value(3).toString();
    }
    else
    {
      error = "Loja não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_form.m_id.isValid())
    bSuccess = m_form.SQL_select_proc(query, error);

  if (bSuccess && m_phone.m_id.isValid())
    bSuccess = m_phone.SQL_select_proc(query, error);

  if (bSuccess && m_address.m_id.isValid())
    bSuccess = m_address.SQL_select_proc(query, error);

  return bSuccess;
}

bool Store::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " STORE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}


