#ifndef SALARY_H
#define SALARY_H

#include "image.h"
#include "jitemsql.h"
#include "salaryemployee.h"

struct Salary : public JItemSQL
{
  Salary();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  QString m_name;
  Image m_image;
  QVector<SalaryEmployee> m_vse;

  QString name() const { return m_name; }
  QByteArray image() const { return m_image.m_image; }
};

#endif // SALARY_H
