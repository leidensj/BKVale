#include "salaryemployee.h"

SalaryEmployee::SalaryEmployee()
{
  SalaryEmployee::clear();
}

SalaryEmployee::SalaryEmployee(const Employee e)
 : m_employee(e)
 , m_salary(0.0)
{

}

void SalaryEmployee::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_salary = 0.0;
  m_employee.clear(true);
}

bool SalaryEmployee::isValid() const
{
  return m_employee.m_id.isValid();
}

bool SalaryEmployee::operator !=(const JItem& other) const
{
  const SalaryEmployee& another = dynamic_cast<const SalaryEmployee&>(other);
  return
      !Data::areEqual(m_salary, another.m_salary, Data::Type::Money) ||
      m_employee.m_id != another.m_employee.m_id;
}

bool SalaryEmployee::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool SalaryEmployee::operator <(const JItem& other) const
{
  const SalaryEmployee& o = dynamic_cast<const SalaryEmployee&>(other);
  return m_employee.m_form.m_name <o.m_employee.m_form.m_name;
}

bool SalaryEmployee::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " SALARY_EMPLOYEE_SQL_TABLE_NAME " ("
                SALARY_EMPLOYEE_SQL_COL_SID ","
                SALARY_EMPLOYEE_SQL_COL_EID ","
                SALARY_EMPLOYEE_SQL_COL_SAL
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_employee.m_id.get());
  query.bindValue(":_v03", m_salary);
  bool ok = query.exec();

  if (ok)
    m_id.set(query.lastInsertId().toLongLong());
  return ok;
}

bool SalaryEmployee::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<SalaryEmployee>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                SALARY_EMPLOYEE_SQL_COL_EID ","
                SALARY_EMPLOYEE_SQL_COL_SAL
                " FROM " SALARY_EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SALARY_EMPLOYEE_SQL_COL_SID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool ok = query.exec();
  if (ok)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      SalaryEmployee o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  return ok;
}

bool SalaryEmployee::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SALARY_EMPLOYEE_SQL_COL_SID ","
                SALARY_EMPLOYEE_SQL_COL_EID ","
                SALARY_EMPLOYEE_SQL_COL_SAL
                " FROM " SALARY_EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_employee.m_id.set(query.value(1).toLongLong());
      m_salary = query.value(2).toDouble();
    }
    else
    {
      error = "Salário do funcionário não encontrada.";
      ok = false;
    }

    if (ok && m_employee.m_id.isValid())
      ok = m_employee.SQL_select_proc(query, error);
  }

  return ok;
}

bool SalaryEmployee::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " SALARY_EMPLOYEE_SQL_TABLE_NAME
                " WHERE " SALARY_EMPLOYEE_SQL_COL_SID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
