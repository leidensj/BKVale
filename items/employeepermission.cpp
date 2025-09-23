#include "employeepermission.h"

EmployeePermission::EmployeePermission()
{
  EmployeePermission::clear();
}

void EmployeePermission::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_func = Functionality::Idx::_END;
  m_bHasAccessToCreate = false;
  m_bHasAccessToEdit = false;
  m_bHasAccessToRemove = false;
}

bool EmployeePermission::isValid() const
{
  return m_func != Functionality::Idx::_END;
}

bool EmployeePermission::operator !=(const JItem& other) const
{
  const EmployeePermission& another = dynamic_cast<const EmployeePermission&>(other);
  return
      m_func != another.m_func ||
      m_bHasAccessToCreate != another.m_bHasAccessToCreate ||
      m_bHasAccessToEdit != another.m_bHasAccessToEdit ||
      m_bHasAccessToRemove != another.m_bHasAccessToRemove;
}

bool EmployeePermission::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool EmployeePermission::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " EMPLOYEE_PERMISSIONS_SQL_TABLE_NAME " ("
                EMPLOYEE_PERMISSIONS_SQL_COL_OID ","
                EMPLOYEE_PERMISSIONS_SQL_COL_FUN ","
                EMPLOYEE_PERMISSIONS_SQL_COL_CRE ","
                EMPLOYEE_PERMISSIONS_SQL_COL_EDI ","
                EMPLOYEE_PERMISSIONS_SQL_COL_REM
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", (int)m_func);
  query.bindValue(":_v03", m_bHasAccessToCreate);
  query.bindValue(":_v04", m_bHasAccessToEdit);
  query.bindValue(":_v05", m_bHasAccessToRemove);
  bool ok = query.exec();

  if (ok)
    m_id.set(query.lastInsertId().toLongLong());
  return ok;
}

bool EmployeePermission::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<EmployeePermission>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                EMPLOYEE_PERMISSIONS_SQL_COL_FUN ","
                EMPLOYEE_PERMISSIONS_SQL_COL_CRE ","
                EMPLOYEE_PERMISSIONS_SQL_COL_EDI ","
                EMPLOYEE_PERMISSIONS_SQL_COL_REM
                " FROM " EMPLOYEE_PERMISSIONS_SQL_TABLE_NAME
                " WHERE " EMPLOYEE_PERMISSIONS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool ok = query.exec();
  if (ok)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      EmployeePermission o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  return ok;
}

bool EmployeePermission::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                EMPLOYEE_PERMISSIONS_SQL_COL_OID ","
                EMPLOYEE_PERMISSIONS_SQL_COL_FUN ","
                EMPLOYEE_PERMISSIONS_SQL_COL_CRE ","
                EMPLOYEE_PERMISSIONS_SQL_COL_EDI ","
                EMPLOYEE_PERMISSIONS_SQL_COL_REM
                " FROM " EMPLOYEE_PERMISSIONS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_func = (Functionality::Idx)query.value(1).toInt();
      m_bHasAccessToCreate = query.value(2).toBool();
      m_bHasAccessToEdit = query.value(3).toBool();
      m_bHasAccessToRemove = query.value(4).toBool();
    }
    else
    {
      error = "Permissão do funcionário não encontrada.";
      ok = false;
    }
  }

  return ok;
}

bool EmployeePermission::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " EMPLOYEE_PERMISSIONS_SQL_TABLE_NAME
                " WHERE " EMPLOYEE_PERMISSIONS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
