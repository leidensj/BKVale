#include "coin.h"

Coin::Coin()
{
  Coin::clear();
}

void Coin::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_tax = 0;
  m_image.clear();
}

bool Coin::operator != (const JItem& other) const
{
  const Coin& another = dynamic_cast<const Coin&>(other);
  return m_name != another.m_name ||
         m_tax != another.m_tax ||
         m_image.m_id != another.m_image.m_id;
}

bool Coin::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Coin::isValid() const
{
  return !m_name.isEmpty();
}

QString Coin::SQL_tableName() const
{
  return COIN_SQL_TABLE_NAME;
}

bool Coin::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " COIN_SQL_TABLE_NAME " ("
                COIN_SQL_COL_IID ","
                COIN_SQL_COL_TAX ","
                COIN_SQL_COL_NAM ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");

  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_tax);
  query.bindValue(":_v03", m_name);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Coin::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " COIN_SQL_TABLE_NAME " SET "
                COIN_SQL_COL_IID " = (:_v01),"
                COIN_SQL_COL_TAX " = (:_v02),"
                COIN_SQL_COL_NAM " = (:_v03)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_tax);
  query.bindValue(":_v03", m_name);

  return query.exec();
}

bool Coin::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                COIN_SQL_COL_IID ","
                COIN_SQL_COL_TAX ","
                COIN_SQL_COL_NAM
                " FROM " COIN_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_image.m_id.set(query.value(0).toLongLong());
      m_tax = query.value(1).toDouble();
      m_name = query.value(2).toString();
    }
    else
    {
      error = "Moeda não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_image.m_id.isValid())
    bSuccess = m_image.SQL_select_proc(query, error);

  return bSuccess;
}

bool Coin::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " COIN_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
