#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
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
                "ITEMS TEXT);");

  bool bSuccess = query.exec();
  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}

<<<<<<< HEAD
<<<<<<< HEAD
bool Database::insert(const PromissoryNoteWidget& note,
=======
bool Database::insert(PromissoryNote note,
>>>>>>> parent of da3042d... criando banco de dados
=======
bool Database::insert(PromissoryNote note,
>>>>>>> parent of da3042d... criando banco de dados
                      QString& error)
{
  error.clear();

  if (!isOpen(error))
    return false;

  QSqlQuery query;
  query.prepare("INSERT INTO PROMISSORYNOTES "
<<<<<<< HEAD
<<<<<<< HEAD
                "(NUMBER, DATE, SUPPLIER, ITEMS) VALUES "
                "(:number), (:date), (:supplier), (:items);");
  query.bindValue(":number", note.getNumber());
  query.bindValue(":date", note.getDate().toJulianDay());
  query.bindValue(":supplier", note.getSupplier());
  query.bindValue(":items", note.serializeTable());
=======
=======
>>>>>>> parent of da3042d... criando banco de dados
                "(NUMBER, SUPPLIER, ITEMS, TOTAL) VALUES "
                "(:number), (:supplier), (:items), (:total);");
  query.bindValue(":number", note.m_number);
  query.bindValue(":supplier", note.m_supplier);
  query.bindValue(":items", note.serializeItems());
  query.bindValue(":total", note.m_total);
>>>>>>> parent of da3042d... criando banco de dados

  bool bSuccess = query.exec();
  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}

bool Database::select(int id,
                      PromissoryNoteWidget& note,
                      QString& error)
{
  error.clear();
  note.clear();
  QSqlQuery query;
<<<<<<< HEAD
<<<<<<< HEAD
  query.prepare("SELECT"
                "NUMBER,"
                "DATE,"
                "SUPPLIER,"
                "ITEMS"
                "FROM PROMISSORYNOTES"
                "WHERE ID = (:id);");
=======
  query.prepare("SELECT NUMBER, SUPPLIER, ITEMS, TOTAL FROM PROMISSORYNOTES WHERE ID = (:id)");
>>>>>>> parent of da3042d... criando banco de dados
=======
  query.prepare("SELECT NUMBER, SUPPLIER, ITEMS, TOTAL FROM PROMISSORYNOTES WHERE ID = (:id)");
>>>>>>> parent of da3042d... criando banco de dados
  query.bindValue(":id", id);

  if (query.exec())
  {
     if (query.next())
     {
        note.m_number = query.value(query.record().indexOf("NUMBER")).toInt();
        note.m_supplier = query.value(query.record().indexOf("SUPPLIER")).toString();
        note.deserializeItems(query.value(query.record().indexOf("ITEMS")).toString());
        note.m_supplier = query.value(query.record().indexOf("TOTAL")).toString();
     }
  }
}
