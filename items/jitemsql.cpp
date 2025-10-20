#include "jitemsql.h"

JItemSQL::~JItemSQL()
{

}

bool JItemSQL::operator ==(const JItemSQL& other) const
{
  return m_id == other.m_id;
}

bool JItemSQL::operator !=(const JItemSQL& other) const
{
  return !(*this == other);
}

bool JItemSQL::SQL_insert_update(QString& error) const
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = m_id.isValid() ? SQL_update_proc(query) : SQL_insert_proc(query);
  return SQL_finish(db, query, bSuccess, error);
}

bool JItemSQL::SQL_insert_update_proc(QSqlQuery& query) const
{
  return m_id.isValid() ? SQL_update_proc(query) : SQL_insert_proc(query);
}

bool JItemSQL::SQL_select(QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_select_proc(query, error);
  return SQL_finish(db, query, bSuccess, error);
}

bool JItemSQL::SQL_remove(QString& error) const
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_remove_proc(query);
  return SQL_finish(db, query, bSuccess, error);
}

bool JItemSQL::SQL_isOpen(QString& error)
{
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  error.clear();
  if (!db.isOpen())
     error = "Banco de dados não foi aberto.";
  return db.isOpen();
}

bool JItemSQL::SQL_finish(QSqlDatabase db,
                          const QSqlQuery& query,
                          bool bExecSelectResult,
                          QString& error)
{
  if (!bExecSelectResult)
  {
    if (error.isEmpty())
    {
      error = query.lastError().text();
      if (error.isEmpty())
        error = db.lastError().text();
    }
    db.rollback();
    return false;
  }
  else
    bExecSelectResult = db.commit();

  if (!bExecSelectResult && error.isEmpty())
  {
    error = query.lastError().text();
    if (error.isEmpty())
      error = db.lastError().text();
  }

  return bExecSelectResult;
}

QString JItemSQL::firstName() const
{
  QString str = name();
  if (!str.isEmpty())
  {
    auto lst = str.split(" ");
    str = lst.first();
  }
  return str;
}

QString JItemSQL::firstNameInitials() const
{
  QString str = name();
  if (!str.isEmpty())
  {
    auto lst = str.split(" ");
    str = lst.first();
    for (int i = 1; i != lst.size(); ++i)
      str += QString(" ") + lst.at(i).at(0) + ".";
  }
  return str;
}
