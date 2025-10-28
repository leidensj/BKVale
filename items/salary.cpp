#include "salary.h"

Salary::Salary()
{
  clear();
}

void Salary::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_image.clear();
  m_vse.clear();
}

bool Salary::operator != (const JItem& other) const
{
  const Salary& another = dynamic_cast<const Salary&>(other);
  return m_name != another.m_name ||
         m_image.m_id != another.m_image.m_id ||
         m_vse != another.m_vse;
}

bool Salary::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Salary::isValid() const
{
  return !m_name.isEmpty();
}

QString Salary::SQL_tableName() const
{
  return SALARY_SQL_TABLE_NAME;
}

bool Salary::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " SALARY_SQL_TABLE_NAME " ("
                SALARY_SQL_COL_IID ","
                SALARY_SQL_COL_NAM ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");

  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);

  bool ok = query.exec();
  if (ok)
    m_id.set(query.lastInsertId().toLongLong());

  for (int i = 0; i != m_vse.size() && ok; ++i)
  {
    m_vse[i].m_ownerId = m_id;
    ok = m_vse.at(i).SQL_insert_proc(query);
  }

  return ok;
}

bool Salary::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " SALARY_SQL_TABLE_NAME " SET "
                SALARY_SQL_COL_IID " = (:_v01),"
                SALARY_SQL_COL_NAM " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);
  bool ok = query.exec();

  if (ok)
  {
    ok = SalaryEmployee::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_vse.size() && ok; ++i)
    {
      m_vse[i].m_ownerId = m_id;
      ok = m_vse.at(i).SQL_insert_proc(query);
    }
  }

  return ok;
}

bool Salary::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                SALARY_SQL_COL_IID ","
                SALARY_SQL_COL_NAM
                " FROM " SALARY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_image.m_id.set(query.value(0).toLongLong());
      m_name = query.value(1).toString();
    }
    else
    {
      error = "Salário não encontrado.";
      ok = false;
    }
  }

  if (ok && m_image.m_id.isValid())
    ok = m_image.SQL_select_proc(query, error);

  if (ok)
    ok = SalaryEmployee::SQL_select_by_owner_id_proc(query, m_id, m_vse, error);

  return ok;
}

bool Salary::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " SALARY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool Salary::SQL_select_all_employee_salaries(const Id& employeeId, QVector<QString>& vname, QVector<double>& vvalue, QString& error)
{
  vname.clear();
  vvalue.clear();
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                SALARY_SQL_COL_NAM ","
                SALARY_EMPLOYEE_SQL_COL_SAL
                " FROM " SALARY_SQL_TABLE_NAME " LEFT JOIN "
                SALARY_EMPLOYEE_SQL_TABLE_NAME " ON "
                SALARY_SQL_TABLE_NAME "." SQL_COLID " = "
                SALARY_EMPLOYEE_SQL_TABLE_NAME "." SALARY_EMPLOYEE_SQL_COL_SID " WHERE "
                SALARY_EMPLOYEE_SQL_COL_EID " = (:_v00)");

  query.bindValue(":_v00", employeeId.get());
  bool ok = query.exec();
  if (ok)
  {
    while (query.next())
    {
      vname.push_back(query.value(0).toString());
      vvalue.push_back(query.value(1).toDouble());
    }
  }

  if (!ok)
  {
    vname.clear();
    vvalue.clear();
  }

  return SQL_finish(db, query, ok, error);
}
