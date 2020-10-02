#include "login.h"
#include <QHostInfo>

Login::Login(bool bSelectCurrentLogin)
{
  clear();
  if (bSelectCurrentLogin)
  {
    QString error;
    SQL_select_current_login(error);
  }
}

Login::~Login()
{
}

void Login::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_user.clear();
}

bool Login::operator != (const JItem& other) const
{
  const Login& another = dynamic_cast<const Login&>(other);
  return m_user.m_id != another.m_user.m_id;
}

bool Login::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Login::isValid() const
{
  return m_user.m_id.isValid();
}

QString Login::SQL_tableName() const
{
  return LOGIN_SQL_TABLE_NAME;
}

bool Login::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " LOGIN_SQL_TABLE_NAME " ("
                LOGIN_SQL_COL_PID ","
                LOGIN_SQL_COL_UID ","
                LOGIN_SQL_COL_MAC ","
                LOGIN_SQL_COL_LOG ")"
                " VALUES ("
                "(SELECT pg_backend_pid()),"
                "(:_v01),"
                "(:_v02),"
                "current_timestamp)");
  query.bindValue(":_v01", m_user.m_id.get());
  query.bindValue(":_v02", QHostInfo::localHostName().toUpper());
  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool Login::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " LOGIN_SQL_TABLE_NAME " SET "
                LOGIN_SQL_COL_PID " = (SELECT pg_backend_pid()),"
                LOGIN_SQL_COL_UID " = (:_v01),"
                LOGIN_SQL_COL_MAC " = (:_v02),"
                LOGIN_SQL_COL_LOG " = current_timestamp"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_user.m_id.get());
  query.bindValue(":_v02", QHostInfo::localHostName().toUpper());
  return query.exec();
}

bool Login::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                LOGIN_SQL_COL_PID ","
                LOGIN_SQL_COL_UID ","
                LOGIN_SQL_COL_MAC ","
                LOGIN_SQL_COL_LOG
                " FROM " LOGIN_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_pid = query.value(0).toLongLong();
      m_user.m_id.set(query.value(1).toLongLong());
      m_machine = query.value(2).toString();
      m_time = query.value(3).toTime();
    }
    else
    {
      error = "Login não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    m_user.SQL_select_proc(query, error);

  return bSuccess;
}

bool Login::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " LOGIN_SQL_TABLE_NAME " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool Login::SQL_login(const QString& strUser, const QString& strPassword, QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  m_user.clear();
  m_user.m_strUser = strUser;
  m_user.m_password = strPassword;
  bool bSuccess = m_user.SQL_select_password_proc(query, error);

  if (bSuccess)
  {
    bSuccess = SQL_logout_proc(query);
    if (bSuccess)
    {
      query.prepare("SELECT " LOGIN_SQL_COL_MAC
                    " FROM "
                    LOGIN_SQL_TABLE_NAME " WHERE "
                    LOGIN_SQL_COL_UID " = (:v02) LIMIT 1");
      query.bindValue(":v02", m_user.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        if (query.next())
        {
          bSuccess = false;
          error = "Usuário " + m_user.m_strUser  + " já logado na máquina " + query.value(0).toString();
        }
        else
        {
          bSuccess = SQL_insert_proc(query);
        }
      }
    }
  }

  return SQL_finish(db, query, bSuccess, error);
}

bool Login::SQL_logout(QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_logout_proc(query);
  return SQL_finish(db, query, bSuccess, error);
}

bool Login::SQL_logout_proc(QSqlQuery& query)
{
  query.prepare("DELETE FROM " LOGIN_SQL_TABLE_NAME
                " WHERE " LOGIN_SQL_COL_PID " = pg_backend_pid()");
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    query.prepare("DELETE FROM " LOGIN_SQL_TABLE_NAME
                  " WHERE " LOGIN_SQL_COL_PID " NOT IN "
                  "(SELECT pid FROM pg_stat_activity)");
    bSuccess = query.exec();
  }
  return bSuccess;
}

bool Login::SQL_select_current_login(QString& error)
{
  error.clear();
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                SQL_COLID
                " FROM " LOGIN_SQL_TABLE_NAME
                " WHERE " LOGIN_SQL_COL_PID " IN (SELECT pg_backend_pid())");
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
    }
    else
    {
      error = "Login não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = SQL_select_proc(query, error);

  if (bSuccess)
    m_user.SQL_select_proc(query, error);

  return SQL_finish(db, query, bSuccess, error);
}
