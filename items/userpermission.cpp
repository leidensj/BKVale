#include "userpermission.h"

UserPermission::UserPermission()
{
  UserPermission::clear();
}

void UserPermission::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_func = Functionality::Idx::_END;
  m_bHasAccess = false;
}

bool UserPermission::isValid() const
{
  return m_func != Functionality::Idx::_END;
}

bool UserPermission::operator !=(const JItem& other) const
{
  const UserPermission& another = dynamic_cast<const UserPermission&>(other);
  return
      m_func != another.m_func ||
      m_bHasAccess != another.m_bHasAccess;
}

bool UserPermission::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool UserPermission::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " USER_PERMISSIONS_SQL_TABLE_NAME " ("
                USER_PERMISSIONS_SQL_COL_OID ","
                USER_PERMISSIONS_SQL_COL_FUN ","
                USER_PERMISSIONS_SQL_COL_ACC
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", (int)m_func);
  query.bindValue(":_v03", m_bHasAccess);
  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool UserPermission::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<UserPermission>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                USER_PERMISSIONS_SQL_COL_FUN ","
                USER_PERMISSIONS_SQL_COL_ACC
                " FROM " USER_PERMISSIONS_SQL_TABLE_NAME
                " WHERE " USER_PERMISSIONS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      UserPermission o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  return bSuccess;
}

bool UserPermission::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                USER_PERMISSIONS_SQL_COL_OID ","
                USER_PERMISSIONS_SQL_COL_FUN ","
                USER_PERMISSIONS_SQL_COL_ACC
                " FROM " USER_PERMISSIONS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_func = (Functionality::Idx)query.value(1).toInt();
      m_bHasAccess = query.value(2).toBool();
    }
    else
    {
      error = "Permissão do usuário não encontrada.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool UserPermission::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " USER_PERMISSIONS_SQL_TABLE_NAME
                " WHERE " USER_PERMISSIONS_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
