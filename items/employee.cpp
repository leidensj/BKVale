#include "employee.h"

Employee::Employee()
{
  Employee::clear();
}

void Employee::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_form.clear(bClearId);
  m_pincode.clear();
  m_hours.clear();
  m_permissions.clear();
}

bool Employee::operator !=(const JItem& other) const
{
  const Employee& another = dynamic_cast<const Employee&>(other);
  bool b =  m_form.m_id != another.m_form.m_id ||
            m_pincode != another.m_pincode ||
            m_permissions != another.m_permissions ||
            m_hours != another.m_hours;
  return b;
}

bool Employee::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Employee::isValid() const
{
  bool b = m_form.m_id.isValid();
  return b;
}

QString Employee::SQL_tableName() const
{
  return EMPLOYEE_SQL_TABLE_NAME;
}

bool Employee::SQL_insert_proc(QSqlQuery& query) const
{
  bool ok = m_form.SQL_insert_proc(query);
  if (ok)
  {
    query.prepare("INSERT INTO " EMPLOYEE_SQL_TABLE_NAME " ("
                  EMPLOYEE_SQL_COL_FID ","
                  EMPLOYEE_SQL_COL_PIN
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02))");
    query.bindValue(":_v01", m_form.m_id.get());
    query.bindValue(":_v02", getPincodeNull());
    ok = query.exec();
    if (ok)
    {
      m_id.set(query.lastInsertId().toLongLong());
      for (int i = 0; i != m_hours.size(); ++i)
      {
        query.prepare("INSERT INTO " EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                      EMPLOYEE_HOURS_SQL_COL_EID ","
                      EMPLOYEE_HOURS_SQL_COL_DAY ","
                      EMPLOYEE_HOURS_SQL_COL_BEG ","
                      EMPLOYEE_HOURS_SQL_COL_END
                      ") VALUES ("
                      "(:_v01),"
                      "(:_v02),"
                      "(:_v03),"
                      "(:_v04))");
        query.bindValue(":_v01", m_id.get());
        query.bindValue(":_v02", m_hours.at(i).m_day);
        query.bindValue(":_v03", m_hours.at(i).m_tmBegin);
        query.bindValue(":_v04", m_hours.at(i).m_tmEnd);
        ok = query.exec();
        if (!ok)
          break;
      }
    }
    if (ok)
    {
      for (int i = 0; i != m_permissions.size() && ok; ++i)
      {
        m_permissions[i].m_ownerId = m_id;
        ok = m_permissions.at(i).SQL_insert_proc(query);
      }
    }
  }
  return ok;
}

bool Employee::SQL_update_proc(QSqlQuery& query) const
{
  bool ok = m_form.SQL_update_proc(query);
  if (ok)
  {
    query.prepare("UPDATE " EMPLOYEE_SQL_TABLE_NAME " SET "
                  EMPLOYEE_SQL_COL_FID " = (:_v01),"
                  EMPLOYEE_SQL_COL_PIN " = (:_v02)"
                  " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    query.bindValue(":_v01", m_form.m_id.get());
    query.bindValue(":_v02", getPincodeNull());

    ok = query.exec();
    if (ok)
    {
      query.prepare("DELETE FROM " EMPLOYEE_HOURS_SQL_TABLE_NAME
                    " WHERE " EMPLOYEE_HOURS_SQL_COL_EID " = (:_v01)");
      query.bindValue(":_v01", m_id.get());
      ok = query.exec();
      if (ok)
      {
        for (int i = 0; i != m_hours.size(); ++i)
        {
          query.prepare("INSERT INTO " EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                        EMPLOYEE_HOURS_SQL_COL_EID ","
                        EMPLOYEE_HOURS_SQL_COL_DAY ","
                        EMPLOYEE_HOURS_SQL_COL_BEG ","
                        EMPLOYEE_HOURS_SQL_COL_END
                        ") VALUES ("
                        "(:_v01),"
                        "(:_v02),"
                        "(:_v03),"
                        "(:_v04))");
          query.bindValue(":_v01", m_id.get());
          query.bindValue(":_v02", m_hours.at(i).m_day);
          query.bindValue(":_v03", m_hours.at(i).m_tmBegin);
          query.bindValue(":_v04", m_hours.at(i).m_tmEnd);
          ok = query.exec();
          if (!ok)
            break;
        }
      }
    }

    if (ok)
    {
      ok = EmployeePermission::SQL_remove_by_owner_id_proc(query, m_id);
      for (int i = 0; i != m_permissions.size() && ok; ++i)
      {
        m_permissions[i].m_ownerId = m_id;
        ok = m_permissions.at(i).SQL_insert_proc(query);
      }
    }
  }
  return ok;
}

bool Employee::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                EMPLOYEE_SQL_COL_FID ","
                EMPLOYEE_SQL_COL_PIN
                " FROM " EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_form.m_id.set(query.value(0).toLongLong());
      m_pincode = query.value(1).toString();

    }
    else
    {
      error = "Funcionário não encontrado.";
      ok = false;
    }
  }

  if (ok)
  {
    query.prepare("SELECT "
                  EMPLOYEE_HOURS_SQL_COL_DAY ","
                  EMPLOYEE_HOURS_SQL_COL_BEG ","
                  EMPLOYEE_HOURS_SQL_COL_END
                  " FROM " EMPLOYEE_HOURS_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_HOURS_SQL_COL_EID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    ok = query.exec();
    while (ok && query.next())
    {
      TimeInterval h;
      h.m_day = query.value(0).toInt();
      h.m_tmBegin = query.value(1).toTime();
      h.m_tmEnd = query.value(2).toTime();
      m_hours.push_back(h);
    }
  }

  if (ok)
    ok = m_form.SQL_select_proc(query, error);

  if (ok)
    ok = EmployeePermission::SQL_select_by_owner_id_proc(query, m_id, m_permissions, error);

  return ok;
}

bool Employee::SQL_remove_proc(QSqlQuery& query) const
{
  bool ok = SQL_select_formid_proc(query);
  if (ok)
    ok = m_form.SQL_remove_proc(query);
  return ok;
}

bool Employee::SQL_select_by_pincode(QString& error)
{
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool ok = SQL_select_by_pincode_proc(query, error);
  return SQL_finish(db, query, ok, error);
}


bool Employee::SQL_select_by_pincode_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  bool ok = true;
  if (m_pincode.isEmpty())
  {
    error = "PIN nulo";
    ok = false;
  }

  if (ok)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " EMPLOYEE_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_SQL_COL_PIN " = (:_v02) LIMIT 1");
    query.bindValue(":_v02", m_pincode);
    ok = query.exec();
    if (ok)
    {
      if (query.next())
      {
        m_id.set(query.value(0).toLongLong());
        ok = SQL_select_proc(query, error);
      }
      else
      {
        error = "PIN inválido";
        ok = false;
      }
    }
  }

  return ok;
}

bool Employee::SQL_select_formid_proc(QSqlQuery& query) const
{
  query.prepare("SELECT "
                EMPLOYEE_SQL_COL_FID
                " FROM " EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();
  if (ok && query.next())
    m_form.m_id.set(query.value(0).toLongLong());
  return ok;
}

QString Employee::strHours() const
{
  QString str;
  for (int i = 0; i != m_hours.size(); ++i)
    str += m_hours.at(i).m_tmBegin.toString("hh:mm") + "-" + m_hours.at(i).m_tmEnd.toString("hh:mm") + " ";
  if (m_hours.size() > 0)
    str.chop(1);
  return str;
}

bool Employee::hasPermissionToCreate(Functionality::Idx idx) const
{
  for (const auto& up : m_permissions)
  {
    if (up.m_func == idx)
      return up.m_bHasAccessToCreate;
  }
  return false;
}

bool Employee::hasPermissionToCreate(const QString& tableName) const
{
  return hasPermissionToCreate(Functionality::tableNameToIdx(tableName));
}

bool Employee::hasPermissionToEdit(Functionality::Idx idx) const
{
  for (const auto& up : m_permissions)
  {
    if (up.m_func == idx)
      return up.m_bHasAccessToEdit;
  }
  return false;
}

bool Employee::hasPermissionToEdit(const QString& tableName) const
{
  return hasPermissionToEdit(Functionality::tableNameToIdx(tableName));
}

bool Employee::hasPermissionToRemove(Functionality::Idx idx) const
{
  for (const auto& up : m_permissions)
  {
    if (up.m_func == idx)
      return up.m_bHasAccessToRemove;
  }
  return false;
}

bool Employee::hasPermissionToRemove(const QString& tableName) const
{
  return hasPermissionToRemove(Functionality::tableNameToIdx(tableName));
}

void Employee::setPermissionToCreate(Functionality::Idx idx, bool bSet)
{
  for (auto& ep : m_permissions)
  {
    if (ep.m_func == idx)
    {
      ep.m_bHasAccessToCreate = bSet;
      return;
    }
  }
  EmployeePermission ep;
  ep.m_func = idx;
  ep.m_bHasAccessToCreate = bSet;
  m_permissions.push_back(ep);
}

void Employee::setPermissionToEdit(Functionality::Idx idx, bool bSet)
{
  for (auto& ep : m_permissions)
  {
    if (ep.m_func == idx)
    {
      ep.m_bHasAccessToEdit = bSet;
      return;
    }
  }
  EmployeePermission ep;
  ep.m_func = idx;
  ep.m_bHasAccessToEdit = bSet;
  m_permissions.push_back(ep);
}

void Employee::setPermissionToRemove(Functionality::Idx idx, bool bSet)
{
  for (auto& ep : m_permissions)
  {
    if (ep.m_func == idx)
    {
      ep.m_bHasAccessToRemove = bSet;
      return;
    }
  }
  EmployeePermission ep;
  ep.m_func = idx;
  ep.m_bHasAccessToRemove = bSet;
  m_permissions.push_back(ep);
}

void Employee::setPermissionToCreate(const QString& tableName, bool bSet)
{
  setPermissionToCreate(Functionality::tableNameToIdx(tableName), bSet);
}

void Employee::setPermissionToEdit(const QString& tableName, bool bSet)
{
  setPermissionToEdit(Functionality::tableNameToIdx(tableName), bSet);
}

void Employee::setPermissionToRemove(const QString& tableName, bool bSet)
{
  setPermissionToRemove(Functionality::tableNameToIdx(tableName), bSet);
}

bool Employee::SQL_select_all(Ids& ids, QString& error)
{
  ids.clear();
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                SQL_COLID
                " FROM " EMPLOYEE_SQL_TABLE_NAME);

  bool ok = query.exec();
  if (ok)
    while (query.next())
      ids.push_back(query.value(0).toLongLong());

  if (!ok)
    ids.clear();

  return SQL_finish(db, query, ok, error);
}
