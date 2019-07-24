#include "image.h"

Image::Image()
{
  clear();
}

void Image::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_image.clear();
}

bool Image::operator !=(const JItem& other) const
{
  const Image& another = dynamic_cast<const Image&>(other);
  return
      m_name != another.m_name ||
      m_image != another.m_image;
}

bool Image::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Image::isValid() const
{
  return !m_name.isEmpty() && !m_image.isEmpty();
}

QString Image::SQL_tableName() const
{
  return IMAGE_SQL_TABLE_NAME;
}

bool Image::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " IMAGE_SQL_TABLE_NAME " ("
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", m_name);
  query.bindValue(":_v02", m_image);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Image::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " IMAGE_SQL_TABLE_NAME " SET "
                IMAGE_SQL_COL01 " = (:_v01),"
                IMAGE_SQL_COL02 " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_name);
  query.bindValue(":_v02", m_image);
  return query.exec();
}

bool Image::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02
                " FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_name = query.value(0).toString();
      m_image = query.value(1).toByteArray();
    }
    else
    {
      error = "Imagem não encontrada.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool Image::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
