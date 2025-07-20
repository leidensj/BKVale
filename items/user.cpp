#include "user.h"
#include <QCryptographicHash>
#include <QObject>

User::User()
{
  User::clear();
}

void User::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_strUser.clear();
  m_password.clear();
  m_permissions.clear();
}

QString User::st_strEncryptedPassword(const QString& strPassword)
{
  return QString(QCryptographicHash::hash(strPassword.toUtf8(), QCryptographicHash::Md5));
}

QString User::strEncryptedPassword() const
{
  return st_strEncryptedPassword(m_password);
}

bool User::operator != (const JItem& other) const
{
  const User& another = dynamic_cast<const User&>(other);
  return
      m_strUser != another.m_strUser ||
      m_permissions != another.m_permissions;
}

bool User::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool User::isValid() const
{
  return m_strUser.length() > 4;
}

QString User::SQL_tableName() const
{
  return USER_SQL_TABLE_NAME;
}

bool User::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " USER_SQL_TABLE_NAME " ("
                USER_SQL_COL_USE ","
                USER_SQL_COL_PAS ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());

  bool ok = query.exec();
  if (ok)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_permissions.size() && ok; ++i)
    {
      m_permissions[i].m_ownerId = m_id;
      ok = m_permissions.at(i).SQL_insert_proc(query);
    }
  }

  return ok;
}

bool User::SQL_update_proc(QSqlQuery& query) const
{
  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL_USE " = (:_v01)");
  if (!m_password.isEmpty())
    strQuery += "," USER_SQL_COL_PAS " = (:_v02)";
  strQuery += " WHERE " SQL_COLID " = (:_v00)";

  query.prepare(strQuery);
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_strUser);
  if (!m_password.isEmpty())
    query.bindValue(":_v02", strEncryptedPassword());

  bool ok = query.exec();
  if (ok)
  {
    ok = UserPermission::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_permissions.size() && ok; ++i)
    {
      m_permissions[i].m_ownerId = m_id;
      ok = m_permissions.at(i).SQL_insert_proc(query);
    }
  }
  return ok;
}

bool User::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                USER_SQL_COL_USE ","
                USER_SQL_COL_PAS
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_strUser = query.value(0).toString();
      query.value(1).toString(); // password nao precisamos
    }
    else
    {
      ok = false;
      error = "Usuário não encontrado.";
    }
  }

  if (ok)
    ok = UserPermission::SQL_select_by_owner_id_proc(query, m_id, m_permissions, error);

  return ok;
}

bool User::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool User::SQL_select_password_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SQL_COLID
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " USER_SQL_COL_USE " = (:_v01) AND "
                USER_SQL_COL_PAS " = (:_v02) LIMIT 1");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
      bSuccess = SQL_select_proc(query, error);
    }
    else
    {
      error = "Usuário ou senha inválidos.";
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool User::hasPermission(Functionality::Idx idx) const
{
  for (const auto& up : m_permissions)
  {
    if (up.m_func == idx)
      return up.m_bHasAccess;
  }
  return false;

}

bool User::hasPermission(const QString& tableName) const
{
  return hasPermission(Functionality::tableNameToIdx(tableName));
}

void User::setPermission(Functionality::Idx idx, bool bSet)
{
  for (auto& up : m_permissions)
  {
    if (up.m_func == idx)
    {
      up.m_bHasAccess = bSet;
      return;
    }
  }
  UserPermission up;
  up.m_func = idx;
  up.m_bHasAccess = bSet;
  m_permissions.push_back(up);
}

bool User::SQL_change_password(const QString& newPassword, QString& error)
{
  error.clear();
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL_PAS " = (:_v01)"
                   " WHERE " SQL_COLID " = (:_v00)");

  query.prepare(strQuery);
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", st_strEncryptedPassword(newPassword));

  bool bSuccess = query.exec();
  return SQL_finish(db, query, bSuccess, error);
}
