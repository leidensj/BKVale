#ifndef USERPERMISSION_H
#define USERPERMISSION_H

#include "jsubitemsql.h"
#include <QVector>

struct UserPermission : public JSubItemSQL
{
  UserPermission();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<UserPermission>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  QString strAmmount() const;
  QString strFmt() const;

  Functionality::Idx m_func;
  bool m_bHasAccess;
};

#endif // USERPERMISSION_H
