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
  return PRICE_SQL_TABLE_NAME;
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

bool Price::SQL_add_prices(QString& error)
{
  error.clear();
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  error.clear();
  query.prepare("INSERT INTO " PRICE_SQL_TABLE_NAME
                " ( " PRICE_SQL_COL_PID "," PRICE_SQL_COL_VAL ") "
                "SELECT " SQL_COLID ",0 FROM " PRODUCT_SQL_TABLE_NAME " WHERE "
                SQL_COLID " NOT IN (SELECT _PRODUCTID FROM " PRICE_SQL_TABLE_NAME ")");
  bool bSuccess = query.exec();
  return SQL_finish(db, query, bSuccess, error);
}
