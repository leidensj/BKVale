#include "supplier.h"

Supplier::Supplier()
{
  clear();
}

void Supplier::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_form.clear(bClearId);
}

bool Supplier::operator !=(const JItem& other) const
{
  const Supplier& another = dynamic_cast<const Supplier&>(other);
  bool b =  m_form.m_id != another.m_form.m_id;
  return b;
}

bool Supplier::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Supplier::isValid() const
{
  bool b = m_form.m_id.isValid();
  return b;
}

QString Supplier::SQL_tableName() const
{
  return SUPPLIER_SQL_TABLE_NAME;
}

bool Supplier::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_insert_proc(query);
  if (bSuccess)
  {
    query.prepare("INSERT INTO " SUPPLIER_SQL_TABLE_NAME " ("
                  SUPPLIER_SQL_COL01 ")"
                  " VALUES ("
                  "(:_v01))");

    query.bindValue(":_v01", m_form.m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
      m_id.set(query.lastInsertId().toLongLong());
  }
  return bSuccess;
}

bool Supplier::SQL_update_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_update_proc(query);
  if (bSuccess)
  {
    query.prepare("UPDATE " SUPPLIER_SQL_TABLE_NAME " SET "
                  SUPPLIER_SQL_COL01 " = (:_v01)"
                  " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    query.bindValue(":_v01", m_form.m_id.get());
    bSuccess = query.exec();
  }
  return bSuccess;
}

bool Supplier::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SUPPLIER_SQL_COL01
                " FROM " SUPPLIER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_form.m_id.set(query.value(0).toLongLong());
    }
    else
    {
      error = "Fornecedor não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = m_form.SQL_select_proc(query, error);

  return bSuccess;
}

bool Supplier::SQL_remove_proc(QSqlQuery& query) const
{
  bool bSuccess = SQL_select_formid_proc(query);
  if (bSuccess)
    bSuccess = m_form.SQL_remove_proc(query);
  return bSuccess;
}

bool Supplier::SQL_select_formid_proc(QSqlQuery& query) const
{
  query.prepare("SELECT "
                SUPPLIER_SQL_COL01
                " FROM " SUPPLIER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess && query.next())
    m_form.m_id.set(query.value(0).toLongLong());
  return bSuccess;
}
