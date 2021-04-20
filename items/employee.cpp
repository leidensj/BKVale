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
  m_createEditPermissions.clear();
  m_removePermissions.clear();
}

bool Employee::operator !=(const JItem& other) const
{
  const Employee& another = dynamic_cast<const Employee&>(other);
  bool b =  m_form.m_id != another.m_form.m_id ||
            m_pincode != another.m_pincode ||
            m_createEditPermissions != another.m_createEditPermissions ||
            m_removePermissions != another.m_removePermissions ||
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
  bool bSuccess = m_form.SQL_insert_proc(query);
  if (bSuccess)
  {
    query.prepare("INSERT INTO " EMPLOYEE_SQL_TABLE_NAME " ("
                  EMPLOYEE_SQL_COL_FID ","
                  EMPLOYEE_SQL_COL_PIN ","
                  EMPLOYEE_SQL_COL_NED ","
                  EMPLOYEE_SQL_COL_NRE
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04))");
    query.bindValue(":_v01", m_form.m_id.get());
    query.bindValue(":_v02", getPincodeNull());
    query.bindValue(":_v03", hasPermissionToCreateEdit(Functionality::Idx::Purchase));
    query.bindValue(":_v04", hasPermissionToRemove(Functionality::Idx::Purchase));
    bSuccess = query.exec();
    if (bSuccess)
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
        bSuccess = query.exec();
        if (!bSuccess)
          break;
      }
    }
  }
  return bSuccess;
}

bool Employee::SQL_update_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_update_proc(query);
  if (bSuccess)
  {
    query.prepare("UPDATE " EMPLOYEE_SQL_TABLE_NAME " SET "
                  EMPLOYEE_SQL_COL_FID " = (:_v01),"
                  EMPLOYEE_SQL_COL_PIN " = (:_v02),"
                  EMPLOYEE_SQL_COL_NED " = (:_v03),"
                  EMPLOYEE_SQL_COL_NRE " = (:_v04)"
                  " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    query.bindValue(":_v01", m_form.m_id.get());
    query.bindValue(":_v02", getPincodeNull());
    query.bindValue(":_v03", hasPermissionToCreateEdit(Functionality::Idx::Purchase));
    query.bindValue(":_v04", hasPermissionToRemove(Functionality::Idx::Purchase));

    bSuccess = query.exec();
    if (bSuccess)
    {
      query.prepare("DELETE FROM " EMPLOYEE_HOURS_SQL_TABLE_NAME
                    " WHERE " EMPLOYEE_HOURS_SQL_COL_EID " = (:_v01)");
      query.bindValue(":_v01", m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
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
          bSuccess = query.exec();
          if (!bSuccess)
            break;
        }
      }
    }
  }
  return bSuccess;
}

bool Employee::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                EMPLOYEE_SQL_COL_FID ","
                EMPLOYEE_SQL_COL_PIN ","
                EMPLOYEE_SQL_COL_NED ","
                EMPLOYEE_SQL_COL_NRE
                " FROM " EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_form.m_id.set(query.value(0).toLongLong());
      m_pincode = query.value(1).toString();
      setPermissionToCreateEdit(Functionality::Idx::Purchase, query.value(2).toBool());
      setPermissionToRemove(Functionality::Idx::Purchase, query.value(3).toBool());
    }
    else
    {
      error = "Funcionário não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  EMPLOYEE_HOURS_SQL_COL_DAY ","
                  EMPLOYEE_HOURS_SQL_COL_BEG ","
                  EMPLOYEE_HOURS_SQL_COL_END
                  " FROM " EMPLOYEE_HOURS_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_HOURS_SQL_COL_EID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    while (bSuccess && query.next())
    {
      TimeInterval h;
      h.m_day = query.value(0).toInt();
      h.m_tmBegin = query.value(1).toTime();
      h.m_tmEnd = query.value(2).toTime();
      m_hours.push_back(h);
    }
  }

  if (bSuccess)
    bSuccess = m_form.SQL_select_proc(query, error);

  return bSuccess;
}

bool Employee::SQL_remove_proc(QSqlQuery& query) const
{
  bool bSuccess = SQL_select_formid_proc(query);
  if (bSuccess)
    bSuccess = m_form.SQL_remove_proc(query);
  return bSuccess;
}

bool Employee::SQL_select_by_pincode(QString& error)
{
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_select_by_pincode_proc(query, error);
  return SQL_finish(db, query, bSuccess, error);
}


bool Employee::SQL_select_by_pincode_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  bool bSuccess = true;
  if (m_pincode.isEmpty())
  {
    error = "PIN nulo";
    bSuccess = false;
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " EMPLOYEE_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_SQL_COL_PIN " = (:_v02) LIMIT 1");
    query.bindValue(":_v02", m_pincode);
    bSuccess = query.exec();
    if (bSuccess)
    {
      if (query.next())
      {
        m_id.set(query.value(0).toLongLong());
        bSuccess = SQL_select_proc(query, error);
      }
      else
      {
        error = "PIN inválido";
        bSuccess = false;
      }
    }
  }

  return bSuccess;
}

bool Employee::SQL_select_formid_proc(QSqlQuery& query) const
{
  query.prepare("SELECT "
                EMPLOYEE_SQL_COL_FID
                " FROM " EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess && query.next())
    m_form.m_id.set(query.value(0).toLongLong());
  return bSuccess;
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

bool Employee::hasPermissionToCreateEdit(Functionality::Idx idx) const
{
  return m_createEditPermissions.contains(idx) ? m_createEditPermissions.value(idx) : false;
}

bool Employee::hasPermissionToCreateEdit(const QString& tableName) const
{
  return hasPermissionToCreateEdit(Functionality::tableNameToIdx(tableName));
}

bool Employee::hasPermissionToRemove(Functionality::Idx idx) const
{
  return m_removePermissions.contains(idx) ? m_removePermissions.value(idx) : false;
}

bool Employee::hasPermissionToRemove(const QString& tableName) const
{
  return hasPermissionToRemove(Functionality::tableNameToIdx(tableName));
}

void Employee::setPermissionToCreateEdit(Functionality::Idx idx, bool bSet)
{
  m_createEditPermissions[idx] = bSet;
}

void Employee::setPermissionToRemove(Functionality::Idx idx, bool bSet)
{
  m_removePermissions[idx] = bSet;
}
