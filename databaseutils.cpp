#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

Database::Database()
  : m_db(QSqlDatabase::addDatabase("QSQLITE"))
{

}

bool Database::isOpen(QString& error)
{
  error.clear();
  if (!m_db.isOpen())
  {
    error = "O arquivo de configuração não está aberto.";
    return false;
  }
  return true;
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
    error = "A conexão com o arquivo de configuração falhou.";
  return bSuccess;
}

bool Database::createTables(QString& error)
{
  error.clear();

  if (!isOpen(error))
    return false;

  QSqlQuery query;
  query.prepare("CREATE TABLE IF NOT EXISTS SETTINGS ("
                "PROMISSORYNOTENUMBER INT NOT NULL,"
                "SERIALPORT TEXT);"

                "CREATE TABLE IF NOT EXISTS PROMISSORYNOTES ("
                "ID INT PRIMARY KEY NOT NULL,"
                "NUMBER INT NOT NULL,"
                "SUPPLIER TEXT NOT NULL,"
                "ITEMS TEXT,"
                "TOTAL TEXT);");

  bool bSuccess = query.exec();
  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}

bool Database::insertPromissoryNote(PromissoryNote note,
                                    QString& error)
{
  error.clear();

  if (!isOpen(error))
    return false;

  QString serializedTable;
  for (size_t i = 0; i != note.tableContent.size(); ++i)
  {
    for (size_t j = 0; j != note.tableContent.size(); ++j)
    {
      serializedTable += note.tableContent.at(i).at(j);
      serializedTable += ";";
    }
  }

  QSqlQuery query;
  query.prepare("INSERT INTO PROMISSORYNOTES "
                "(NUMBER, SUPPLIER, ITEMS, TOTAL) VALUES "
                "(:number), (:supplier), (:items), (:total);");
  query.bindValue(":number", note.number);
  query.bindValue(":supplier", note.supplier);
  query.bindValue(":items", serializedTable);
  query.bindValue(":total", note.total);

  bool bSuccess = query.exec();
  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}
