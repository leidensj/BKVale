#ifndef LOGIN_H
#define LOGIN_H

#include "user.h"
#include <QTime>

class Login : public JItemSQL
{
public:
  Login(bool bSelectCurrentLogin = false);
  ~Login();

  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  bool SQL_login(const QString& strUser, const QString& strPassword, QString& error);
  bool SQL_logout(QString& error);
  bool SQL_logout_proc(QSqlQuery& query);
  bool SQL_select_current_login(QString& error);

private:
  User m_user;
  qlonglong m_pid;
  QString m_machine;
  QTime m_time;

public:
  const User& getUser() const { return m_user; }
};

#endif // LOGIN_H
