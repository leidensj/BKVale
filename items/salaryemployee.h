#ifndef SALARYEMPLOYEE_H
#define SALARYEMPLOYEE_H

#include "jsubitemsql.h"
#include "employee.h"
#include <QVector>

struct SalaryEmployee : public JSubItemSQL
{
  SalaryEmployee();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<SalaryEmployee>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  double m_salary;
  Employee m_employee;
};

#endif // SALARYEMPLOYEE_H
