#include "price.h"

Price::Price()
{
  Price::clear();
}

void Price::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_product.clear();
}

bool Price::operator != (const JItem& other) const
{
  const Price& another = dynamic_cast<const Price&>(other);
  return m_value != another.m_value ||
         m_product.m_id != another.m_product.m_id;
}

bool Price::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Price::isValid() const
{
  return true;
}

QString Price::SQL_tableName() const
{
  return CATEGORY_SQL_TABLE_NAME;
}

bool Price::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PRICE_SQL_TABLE_NAME " ("
                PRICE_SQL_COL_PID ","
                PRICE_SQL_COL_VAL ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");

  query.bindValue(":_v01", m_product.m_id.get());
  query.bindValue(":_v02", m_value);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Price::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " PRICE_SQL_TABLE_NAME " SET "
                PRICE_SQL_COL_PID " = (:_v01),"
                PRICE_SQL_COL_VAL " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_product.m_id.get());
  query.bindValue(":_v02", m_value);

  return query.exec();
}

bool Price::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                PRICE_SQL_COL_PID ","
                PRICE_SQL_COL_VAL
                " FROM " PRICE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_product.m_id.set(query.value(0).toLongLong());
      m_value = query.value(1).toDouble();
    }
    else
    {
      error = "Preço não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_product.m_id.isValid())
    bSuccess = m_product.SQL_select_proc(query, error);

  return bSuccess;
}

bool Price::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " PRICE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
