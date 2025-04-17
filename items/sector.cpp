#include "sector.h"

Sector::Sector()
{
  clear();
}

void Sector::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_image.clear();
}

bool Sector::operator != (const JItem& other) const
{
  const Sector& another = dynamic_cast<const Sector&>(other);
  return m_name != another.m_name ||
         m_image.m_id != another.m_image.m_id;
}

bool Sector::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Sector::isValid() const
{
  return !m_name.isEmpty();
}

QString Sector::SQL_tableName() const
{
  return SECTOR_SQL_TABLE_NAME;
}

bool Sector::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " SECTOR_SQL_TABLE_NAME " ("
                SECTOR_SQL_COL_IID ","
                SECTOR_SQL_COL_NAM ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");

  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Sector::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " SECTOR_SQL_TABLE_NAME " SET "
                SECTOR_SQL_COL_IID " = (:_v01),"
                SECTOR_SQL_COL_NAM " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);

  return query.exec();
}

bool Sector::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                SECTOR_SQL_COL_IID ","
                SECTOR_SQL_COL_NAM
                " FROM " SECTOR_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_image.m_id.set(query.value(0).toLongLong());
      m_name = query.value(1).toString();
    }
    else
    {
      error = "Setor não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_image.m_id.isValid())
    bSuccess = m_image.SQL_select_proc(query, error);

  return bSuccess;
}

bool Sector::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " SECTOR_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
