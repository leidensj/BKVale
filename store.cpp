#include "store.h"

StoreEmployee::StoreEmployee()
{
  clear();
}

void StoreEmployee::clear()
{
  m_employee.clear();
  m_hours.clear();
}

bool StoreEmployee:: operator != (const JItem& other) const
{
  const StoreEmployee& another = dynamic_cast<const StoreEmployee&>(other);
  return m_employee.m_id != another.m_employee.m_id ||
         m_hours != another.m_hours;
}

bool StoreEmployee:: operator == (const JItem& other) const
{
  return !(*this != other);
}

bool StoreEmployee::isValid() const
{
  return m_employee.m_id.isValid();
}

QString StoreEmployee::strTableName() const
{
  return STORE_EMPLOYEES_SQL_TABLE_NAME;
}

QString StoreEmployee::strHours() const
{
  QString str;
  for (int i = 0; i != m_hours.size(); ++i)
    str += m_hours.at(i).m_tmBegin.toString("hh:mm") + "-" + m_hours.at(i).m_tmEnd.toString("hh:mm") + " ";
  if (m_hours.size() > 0)
    str.chop(1);
  return str;
}

Store::Store()
{
  clear();
}

void Store::clear()
{
  m_person.clear();
  m_address.clear();
  m_phone.clear();
  m_name.clear();
  m_vEmployee.clear();
}

bool Store:: operator != (const JItem& other) const
{
  const Store& another = dynamic_cast<const Store&>(other);
  return m_person.m_id != another.m_person.m_id ||
                           m_address.m_id != another.m_address.m_id ||
                           m_phone.m_id != another.m_phone.m_id ||
                           m_name != another.m_name ||
                           m_vEmployee != another.m_vEmployee;
}

bool Store:: operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Store::isValid() const
{
  return m_person.m_id.isValid();
}

QString Store::SQL_tableName() const
{
  return STORE_SQL_TABLE_NAME;
}

bool Store::SQL_insert_proc(QSqlQuery& query)
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

  query.bindValue(":_v01", m_person.m_id.getIdNull());
  query.bindValue(":_v02", m_address.m_id.getIdNull());
  query.bindValue(":_v03", m_phone.m_id.getIdNull());
  query.bindValue(":_v04", m_name);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vEmployee.size(); ++i)
    {
      query.prepare("INSERT INTO " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                    STORE_EMPLOYEES_SQL_COL01 ","
                    STORE_EMPLOYEES_SQL_COL02
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vEmployee.at(i).m_employee.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        m_vEmployee.at(i).m_id.set(query.lastInsertId().toLongLong());
        for (int j = 0; j != m_vEmployee.at(i).m_hours.size(); ++j)
        {
          query.prepare("INSERT INTO " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                        STORE_EMPLOYEE_HOURS_SQL_COL01 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL02 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL03 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL04
                        ") VALUES ("
                        "(:_v01),"
                        "(:_v02),"
                        "(:_v03),"
                        "(:_v04))");
          query.bindValue(":_v01", m_vEmployee.at(i).m_id.get());
          query.bindValue(":_v02", m_vEmployee.at(i).m_hours.at(j).m_day);
          query.bindValue(":_v03", m_vEmployee.at(i).m_hours.at(j).m_tmBegin);
          query.bindValue(":_v04", m_vEmployee.at(i).m_hours.at(j).m_tmEnd);
          bSuccess = query.exec();
          if (!bSuccess)
            break;
        }
      }

      if (!bSuccess)
        break;
    }
  }

  return bSuccess;
}

bool Store::SQL_update_proc(QSqlQuery& query)
{
  query.prepare("UPDATE " STORE_SQL_TABLE_NAME " SET "
                STORE_SQL_COL01 " = (:_v01),"
                STORE_SQL_COL02 " = (:_v02),"
                STORE_SQL_COL03 " = (:_v03),"
                STORE_SQL_COL04 " = (:_v04)"
                " WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_person.m_id.getIdNull());
  query.bindValue(":_v02", m_address.m_id.getIdNull());
  query.bindValue(":_v03", m_phone.m_id.getIdNull());
  query.bindValue(":_v04", m_name);

  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " STORE_EMPLOYEES_SQL_TABLE_NAME
                  " WHERE " STORE_EMPLOYEES_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vEmployee.size(); ++i)
    {
      query.prepare("INSERT INTO " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                    STORE_EMPLOYEES_SQL_COL01 ","
                    STORE_EMPLOYEES_SQL_COL02
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vEmployee.at(i).m_employee.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        m_vEmployee.at(i).m_id.set(query.lastInsertId().toLongLong());
        for (int j = 0; j != o.m_vEmployee.at(i).m_hours.size(); ++j)
        {
          query.prepare("INSERT INTO " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                        STORE_EMPLOYEE_HOURS_SQL_COL01 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL02 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL03 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL04
                        ") VALUES ("
                        "(:_v01),"
                        "(:_v02),"
                        "(:_v03),"
                        "(:_v04))");
          query.bindValue(":_v01", m_vEmployee.at(i).m_id.get());
          query.bindValue(":_v02", m_vEmployee.at(i).m_hours.at(j).m_day);
          query.bindValue(":_v03", m_vEmployee.at(i).m_hours.at(j).m_tmBegin);
          query.bindValue(":_v04", m_vEmployee.at(i).m_hours.at(j).m_tmEnd);
          bSuccess = query.exec();
          if (!bSuccess)
            break;
        }
      }

      if (!bSuccess)
        break;
    }
  }

  return bSuccess;
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
      m_id = id;
      m_person.m_id = query.value(0).toLongLong();
      m_address.m_id = query.value(1).toLongLong();
      m_phone.m_id = query.value(2).toLongLong();
      m_name = query.value(3).toString();

      query.prepare("SELECT "
                    SQL_COLID ","
                    STORE_EMPLOYEES_SQL_COL02
                    " FROM " STORE_EMPLOYEES_SQL_TABLE_NAME
                    " WHERE " STORE_EMPLOYEES_SQL_COL01 " = (:_v01)");
      query.bindValue(":_v01", m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        while (query.next())
        {
          StoreEmployee _o;
          _o.m_id.set(query.value(0).toLongLong());
          _o.m_employee.m_id.set(query.value(1).toLongLong());
          m_vEmployee.push_back(_o);
        }
      }
    }
    else
    {
      error = "Loja não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_person.m_id.isValid())
    bSuccess = m_person.SQL_select_proc(query, error);

  if (bSuccess)
  {
    for (int i = 0; i != m_vEmployee.size(); ++i)
    {
      if (m_vEmployee.at(i).m_id.isValid())
      {
        bSuccess = m_vEmployee[i].m_employee.SQL_select_proc(query, error);
        if (bSuccess)
        {
          query.prepare("SELECT "
                        STORE_EMPLOYEE_HOURS_SQL_COL02 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL03 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL04
                        " FROM " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME
                        " WHERE " STORE_EMPLOYEE_HOURS_SQL_COL01 " = (:_v01)");
          query.bindValue(":_v01", m_vEmployee.at(i).m_id.get());
          bSuccess = query.exec();
          while (bSuccess && query.next())
          {
            TimeInterval h;
            h.m_day = query.value(0).toInt();
            h.m_tmBegin = query.value(1).toTime();
            h.m_tmEnd = query.value(2).toTime();
            m_vEmployee[i].m_hours.push_back(h);
          }
        }
      }
      if (!bSuccess)
        break;
    }
  }

  // TODO Separar Address e Phone e suas funções

  return bSuccess;
}

bool Store::SQL_remove_proc(QSqlQuery& query)
{
  query.prepare("DELETE FROM " STORE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  return bSuccess;
}


