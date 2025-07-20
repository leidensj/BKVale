#ifndef EMPLOYEEPERMISSION_H
#define EMPLOYEEPERMISSION_H

#include "jsubitemsql.h"
#include <QVector>

struct EmployeePermission : public JSubItemSQL
{
  EmployeePermission();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<EmployeePermission>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  Functionality::Idx m_func;
  bool m_bHasAccessToCreate;
  bool m_bHasAccessToEdit;
  bool m_bHasAccessToRemove;
};

#endif // EMPLOYEEPERMISSION_H
