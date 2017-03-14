#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

Database::Database()
  : m_db(QSqlDatabase::addDatabase("QSQLITE"))
{

}

bool Database::isOpen(QString& error) const
{
  error.clear();
  if (!m_db.isOpen())
     error = "Arquivo de configuração não foi aberto.";
  return m_db.isOpen();
}

bool Database::open(const QString& path,
                    QString& error)
{
  error.clear();
  if (m_db.isOpen())
    m_db.close();
  m_db.setDatabaseName(path);
  bool bSuccess = m_db.open();
  if (!bSuccess)
    error = m_db.lastError().text();
  return bSuccess;
}

bool Database::createTables(QString& error)
{
  error.clear();

  if (!isOpen(error))
    return false;

  QSqlQuery query;
  bool bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _SETTINGS ("
                             "_LASTNUMBER INTEGER AUTOINCREMENT DEFAULT 10000,"
                             "_SERIALPORT TEXT)");
  if (bSuccess)
  {
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTES ("
                          "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "_NUMBER INTEGER NOT NULL,"
                          "_DATE INTEGER NOT NULL,"
                          "_SUPPLIER TEXT NOT NULL,"
                          "_ITEMS TEXT,"
                          "_TOTAL REAL)");
  }

  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}

bool Database::insert(const Note& note,
                      QString& error)
{
  error.clear();

  if (!isOpen(error))
    return false;

  QSqlQuery query;
  query.prepare("INSERT INTO _PROMISSORYNOTES ("
                "_NUMBER,"
                "_DATE,"
                "_SUPPLIER,"
                "_ITEMS,"
                "_TOTAL) "
                "VALUES ("
                "(:_number),"
                "(:_date),"
                "(:_supplier),"
                "(:_items),"
                "(:_total))");

  query.bindValue(":_number", note.m_number);
  query.bindValue(":_date", note.m_date);
  query.bindValue(":_supplier", note.m_supplier);
  query.bindValue(":_items", note.m_items);
  query.bindValue(":_total", note.m_total);

  bool bSuccess = query.exec();
  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}

bool Database::select(int id,
                      Note& note,
                      QString& error)
{
  error.clear();
  note.clear();

  if (!isOpen(error))
    return false;

  QSqlQuery query;
  query.prepare("SELECT "
                "_NUMBER,"
                "_DATE,"
                "_SUPPLIER,"
                "_ITEMS,"
                "_TOTAL "
                "FROM PROMISSORYNOTES "
                "WHERE _ID = (:_id);");

  query.bindValue(":_id", id);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
     bSuccess = query.next();
     if (bSuccess)
     {
        note.m_number = query.value(query.record().indexOf("_NUMBER")).toInt();
        note.m_date = query.value(query.record().indexOf("_DATE")).toLongLong();
        note.m_supplier = query.value(query.record().indexOf("_SUPPLIER")).toString();
        note.m_items = query.value(query.record().indexOf("_ITEMS")).toString();
        note.m_supplier = query.value(query.record().indexOf("_TOTAL")).toString();
     }
  }

  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}
