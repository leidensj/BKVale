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
                USER_SQL_COL_PAS ","
                USER_SQL_COL_APU ","
                USER_SQL_COL_ARE ","
                USER_SQL_COL_ACA ","
                USER_SQL_COL_ASH ","
                USER_SQL_COL_AUS ","
                USER_SQL_COL_APR ","
                USER_SQL_COL_ASE ","
                USER_SQL_COL_ACT ","
                USER_SQL_COL_AIM ","
                USER_SQL_COL_ASL ","
                USER_SQL_COL_AEM ","
                USER_SQL_COL_ASU ","
                USER_SQL_COL_AST ","
                USER_SQL_COL_ATI ","
                USER_SQL_COL_ACO ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07),"
                "(:_v08),"
                "(:_v09),"
                "(:_v10),"
                "(:_v11),"
                "(:_v12),"
                "(:_v13),"
                "(:_v14),"
                "(:_v15),"
                "(:_v16),"
                "(:_v17))");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", hasPermission(Functionality::Idx::Purchase));
  query.bindValue(":_v04", hasPermission(Functionality::Idx::Reminder));
  query.bindValue(":_v05", hasPermission(Functionality::Idx::Calculator));
  query.bindValue(":_v06", hasPermission(Functionality::Idx::Shop));
  query.bindValue(":_v07", hasPermission(Functionality::Idx::User));
  query.bindValue(":_v08", hasPermission(Functionality::Idx::Product));
  query.bindValue(":_v09", hasPermission(Functionality::Idx::Settings));
  query.bindValue(":_v10", hasPermission(Functionality::Idx::Category));
  query.bindValue(":_v11", hasPermission(Functionality::Idx::Image));
  query.bindValue(":_v12", hasPermission(Functionality::Idx::ShoppingList));
  query.bindValue(":_v13", hasPermission(Functionality::Idx::Employee));
  query.bindValue(":_v14", hasPermission(Functionality::Idx::Supplier));
  query.bindValue(":_v15", hasPermission(Functionality::Idx::Store));
  query.bindValue(":_v16", hasPermission(Functionality::Idx::TimeCard));
  query.bindValue(":_v17", hasPermission(Functionality::Idx::Coupon));

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool User::SQL_update_proc(QSqlQuery& query) const
{
  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL_USE " = (:_v01),");
  if (!m_password.isEmpty())
    strQuery += USER_SQL_COL_PAS " = (:_v02),";
  strQuery += USER_SQL_COL_APU " = (:_v03),"
              USER_SQL_COL_ARE " = (:_v04),"
              USER_SQL_COL_ACA " = (:_v05),"
              USER_SQL_COL_ASH " = (:_v06),"
              USER_SQL_COL_AUS " = (:_v07),"
              USER_SQL_COL_APR " = (:_v08),"
              USER_SQL_COL_ASE " = (:_v09),"
              USER_SQL_COL_ACT " = (:_v10),"
              USER_SQL_COL_AIM " = (:_v11),"
              USER_SQL_COL_ASL " = (:_v12),"
              USER_SQL_COL_AEM " = (:_v13),"
              USER_SQL_COL_ASU " = (:_v14),"
              USER_SQL_COL_AST " = (:_v15),"
              USER_SQL_COL_ATI " = (:_v16),"
              USER_SQL_COL_ACO " = (:_v17)"
              " WHERE " SQL_COLID " = (:_v00)";

  query.prepare(strQuery);
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_strUser);
  if (!m_password.isEmpty())
    query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", hasPermission(Functionality::Idx::Purchase));
  query.bindValue(":_v04", hasPermission(Functionality::Idx::Reminder));
  query.bindValue(":_v05", hasPermission(Functionality::Idx::Calculator));
  query.bindValue(":_v06", hasPermission(Functionality::Idx::Shop));
  query.bindValue(":_v07", hasPermission(Functionality::Idx::User));
  query.bindValue(":_v08", hasPermission(Functionality::Idx::Product));
  query.bindValue(":_v09", hasPermission(Functionality::Idx::Settings));
  query.bindValue(":_v10", hasPermission(Functionality::Idx::Category));
  query.bindValue(":_v11", hasPermission(Functionality::Idx::Image));
  query.bindValue(":_v12", hasPermission(Functionality::Idx::ShoppingList));
  query.bindValue(":_v13", hasPermission(Functionality::Idx::Employee));
  query.bindValue(":_v14", hasPermission(Functionality::Idx::Supplier));
  query.bindValue(":_v15", hasPermission(Functionality::Idx::Store));
  query.bindValue(":_v16", hasPermission(Functionality::Idx::TimeCard));
  query.bindValue(":_v17", hasPermission(Functionality::Idx::Coupon));

  return query.exec();
}

bool User::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                USER_SQL_COL_USE ","
                USER_SQL_COL_PAS ","
                USER_SQL_COL_APU ","
                USER_SQL_COL_ARE ","
                USER_SQL_COL_ACA ","
                USER_SQL_COL_ASH ","
                USER_SQL_COL_AUS ","
                USER_SQL_COL_APR ","
                USER_SQL_COL_ASE ","
                USER_SQL_COL_ACT ","
                USER_SQL_COL_AIM ","
                USER_SQL_COL_ASL ","
                USER_SQL_COL_AEM ","
                USER_SQL_COL_ASU ","
                USER_SQL_COL_AST ","
                USER_SQL_COL_ATI ","
                USER_SQL_COL_ACO
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_strUser = query.value(0).toString();
      query.value(1).toString(); // password nao precisamos
      setPermission(Functionality::Idx::Purchase, query.value(2).toBool());
      setPermission(Functionality::Idx::Reminder, query.value(3).toBool());
      setPermission(Functionality::Idx::Calculator, query.value(4).toBool());
      setPermission(Functionality::Idx::Shop, query.value(5).toBool());
      setPermission(Functionality::Idx::User, query.value(6).toBool());
      setPermission(Functionality::Idx::Product, query.value(7).toBool());
      setPermission(Functionality::Idx::Settings, query.value(8).toBool());
      setPermission(Functionality::Idx::Category, query.value(9).toBool());
      setPermission(Functionality::Idx::Image, query.value(10).toBool());
      setPermission(Functionality::Idx::ShoppingList, query.value(11).toBool());
      setPermission(Functionality::Idx::Employee, query.value(12).toBool());
      setPermission(Functionality::Idx::Supplier, query.value(13).toBool());
      setPermission(Functionality::Idx::Store, query.value(14).toBool());
      setPermission(Functionality::Idx::TimeCard, query.value(15).toBool());
      setPermission(Functionality::Idx::Coupon, query.value(16).toBool());
    }
    else
    {
      bSuccess = false;
      error = "Usuário não encontrado.";
    }
  }

  return bSuccess;
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
  return m_permissions.contains(idx) ? m_permissions.value(idx) : false;

}

bool User::hasPermission(const QString& tableName) const
{
  return hasPermission(Functionality::tableNameToIdx(tableName));
}

void User::setPermission(Functionality::Idx idx, bool bSet)
{
  m_permissions[idx] = bSet;
}
