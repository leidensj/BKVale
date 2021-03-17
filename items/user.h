#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include "jitemsql.h"
#include "defines.h"

struct User : public JItemSQL
{
  User();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  bool SQL_select_password_proc(QSqlQuery& query, QString& error);

  QString strEncryptedPassword() const;
  static QString st_strEncryptedPassword(const QString& strPassword);

  bool hasPermission(const QString& tableName) const;
  bool hasPermission(Functionality::Idx idx) const;
  void setPermission(Functionality::Idx idx, bool bSet);

  QString m_strUser;
  QString m_password;
  QMap<Functionality::Idx, bool> m_permissions;
};

#endif // USER_H
