#include "salaryformula.h"

SalaryFormula::SalaryFormula()
{
  clear();
}

void SalaryFormula::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_image.clear();
  m_formula.clear();
  m_description.clear();
}

bool SalaryFormula::operator != (const JItem& other) const
{
  const SalaryFormula& another = dynamic_cast<const SalaryFormula&>(other);
  return m_name != another.m_name ||
         m_image.m_id != another.m_image.m_id ||
         m_formula != another.m_formula ||
         m_description != another.m_description;
}

bool SalaryFormula::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool SalaryFormula::isValid() const
{
  return !m_name.isEmpty();
}

QString SalaryFormula::SQL_tableName() const
{
  return SALARY_FORMULA_SQL_TABLE_NAME;
}

bool SalaryFormula::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " SALARY_FORMULA_SQL_TABLE_NAME " ("
                SALARY_FORMULA_SQL_COL_IID ","
                SALARY_FORMULA_SQL_COL_NAM ","
                SALARY_FORMULA_SQL_COL_FOR ","
                SALARY_FORMULA_SQL_COL_DES ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04))");

  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", m_formula);
  query.bindValue(":_v04", m_description);

  bool ok = query.exec();
  if (ok)
    m_id.set(query.lastInsertId().toLongLong());

  return ok;
}

bool SalaryFormula::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " SALARY_FORMULA_SQL_TABLE_NAME " SET "
                SALARY_FORMULA_SQL_COL_IID " = (:_v01),"
                SALARY_FORMULA_SQL_COL_NAM " = (:_v02),"
                SALARY_FORMULA_SQL_COL_FOR " = (:_v03),"
                SALARY_FORMULA_SQL_COL_DES " = (:_v04)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", m_formula);
  query.bindValue(":_v04", m_description);
  bool ok = query.exec();

  return ok;
}

bool SalaryFormula::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                SALARY_FORMULA_SQL_COL_IID ","
                SALARY_FORMULA_SQL_COL_NAM ","
                SALARY_FORMULA_SQL_COL_FOR ","
                SALARY_FORMULA_SQL_COL_DES
                " FROM " SALARY_FORMULA_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_image.m_id.set(query.value(0).toLongLong());
      m_name = query.value(1).toString();
      m_formula = query.value(2).toString();
      m_description = query.value(3).toString();
    }
    else
    {
      error = "Fórmula de salário não encontrado.";
      ok = false;
    }
  }

  if (ok && m_image.m_id.isValid())
    ok = m_image.SQL_select_proc(query, error);

  return ok;
}

bool SalaryFormula::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " SALARY_FORMULA_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
