#ifndef ACTIVEUSER_H
#define ACTIVEUSER_H

#include "user.h"
#include <QTime>

class ActiveUser : public JItemSQL
{
public:
  ActiveUser();
  ~ActiveUser();

  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject* parent) const;

  bool SQL_login(const QString& strUser, const QString& strPassword, QString& error);
  bool SQL_logout(QString& error);
  bool SQL_logout_proc(QSqlQuery& query);

private:
  User m_user;
  qlonglong m_pid;
  QString m_machine;
  QTime m_time;

public:
  const User& getUser() const { return m_user; }
};

#endif // ACTIVEUSER_H
