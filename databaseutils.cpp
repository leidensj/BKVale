#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

#define DATABASE_NOT_OPEN_TXT "O arquivo de configuração não foi aberto."

int NoteSQL::nextNumber(QSqlDatabase db)
{
  if (!db.isOpen())
    return -1;

  QSqlQuery query("SELECT _LASTNUMBER FROM _SETTINGS", db);
  int idx = query.record().indexOf("_LASTNUMBER");
  return query.next() ? query.value(idx).toInt() : DEFAULT_NUMBER;
}

QStringList NoteSQL::suppliers(QSqlDatabase db)
{
  QStringList list;
  if (db.isOpen())
  {
    QSqlQuery query(db);
    if (query.exec("SELECT DISTINCT "
                   "_SUPPLIER "
                   "FROM _PROMISSORYNOTES"))
    {
      while (query.next())
        list << query.value(query.record().indexOf("_SUPPLIER")).toString();
    }
  }
  return list;
}

bool NoteSQL::insertOrUpdate(QSqlDatabase db,
                                  const Note& note,
                                  QString& error)
{
  error.clear();

  if (!db.isOpen())
  {
    error = DATABASE_NOT_OPEN_TXT;
    return false;
  }

  QSqlQuery query(db);
  if (!Note::isValidID(note.m_id))
  {
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
  }
  else
  {
    query.prepare("UPDATE _PROMISSORYNOTES "
                  "SET _NUMBER = (:_number),"
                  "_DATE = (:_date),"
                  "_SUPPLIER = (:_supplier),"
                  "_ITEMS = (:_items),"
                  "_TOTAL = (:_total) "
                  "WHERE _ID = (:_id)");
    query.bindValue(":_id", note.m_id);
  }

  query.bindValue(":_number", note.m_number);
  query.bindValue(":_date", note.m_date);
  query.bindValue(":_supplier", note.m_supplier);
  query.bindValue(":_items", note.m_items);
  query.bindValue(":_total", note.m_total);

  bool bSuccess = query.exec();
  if (!bSuccess)
  {
    error = query.lastError().text();
  }
  else if (!Note::isValidID(note.m_id))
  {
    incNumber(db);
  }

  return bSuccess;
}

void NoteSQL::incNumber(QSqlDatabase db)
{
  int n = nextNumber(db) + 1;
  QSqlQuery query(db);
  query.prepare("UPDATE _SETTINGS SET _LASTNUMBER = :_number");
  query.bindValue(":_number", n);
  query.exec();
}

bool NoteSQL::select(QSqlDatabase db,
                     int id,
                     Note& note,
                     QString& error)
{
  error.clear();
  note.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("SELECT "
                "_NUMBER,"
                "_DATE,"
                "_SUPPLIER,"
                "_ITEMS,"
                "_TOTAL "
                "FROM _PROMISSORYNOTES "
                "WHERE _ID = (:_id)");

  query.bindValue(":_id", id);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    bSuccess = query.next();
    if (bSuccess)
    {
      note.m_id = id;
      note.m_number = query.value(query.record().indexOf("_NUMBER")).toInt();
      note.m_date = query.value(query.record().indexOf("_DATE")).toLongLong();
      note.m_supplier = query.value(query.record().indexOf("_SUPPLIER")).toString();
      note.m_items = query.value(query.record().indexOf("_ITEMS")).toString();
      note.m_total = query.value(query.record().indexOf("_TOTAL")).toDouble();
    }
    else
    {
      error = "Vale não encontrado.";
    }
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

bool NoteSQL::selectAll(QSqlDatabase db,
                             Notes& notes,
                             QString& error)
{
  error.clear();
  notes.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  bool bSuccess = query.exec("SELECT "
                             "_ID,"
                             "_NUMBER,"
                             "_DATE,"
                             "_SUPPLIER,"
                             "_ITEMS,"
                             "_TOTAL "
                             "FROM _PROMISSORYNOTES");

  if (bSuccess)
  {
    while (query.next())
    {
      notes.emplace_back(Note(query.value(query.record().indexOf("_ID")).toInt(),
                              query.value(query.record().indexOf("_NUMBER")).toInt(),
                              (qint64)query.value(query.record().indexOf("_DATE")).toLongLong(),
                              query.value(query.record().indexOf("_SUPPLIER")).toString(),
                              query.value(query.record().indexOf("_ITEMS")).toString(),
                              query.value(query.record().indexOf("_TOTAL")).toDouble()));
    }
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

bool BaitaDatabase::isOpen(QSqlDatabase db,
                           QString& error)
{
  error.clear();
  if (!db.isOpen())
     error = "Arquivo de configuração não foi aberto.";
  return db.isOpen();
}

bool BaitaDatabase::open(QSqlDatabase db,
                         const QString& path,
                         QString& error)
{
  error.clear();
  if (db.isOpen())
    db.close();
  db.setDatabaseName(path);
  bool bSuccess = db.open();
  if (!bSuccess)
    error = db.lastError().text();
  return bSuccess;
}

void BaitaDatabase::close(QSqlDatabase db)
{
  db.close();
}

bool BaitaDatabase::init(QSqlDatabase db,
                         QString& error)
{
  error.clear();

  if (!isOpen(db, error))
    return false;

  bool bHasConfig = BaitaDatabase::hasConfig(db);

  QSqlQuery query(db);
  db.transaction();
  query.exec("CREATE TABLE IF NOT EXISTS _SETTINGS ("
             "_LASTNUMBER INTEGER DEFAULT " DEFAULT_NUMBER_STR ","
             "_SERIALPORT TEXT,"
             "_BAUDRATE INTEGER DEFAULT 9600,"
             "_DATABITS INTEGER DEFAULT 8,"
             "_FLOWCONTROL INTEGER DEFAULT 0,"
             "_PARITY INTEGER DEFAULT 0,"
             "_STOPBITS INTEGER DEFAULT 1)");

  query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NUMBER INTEGER NOT NULL,"
             "_DATE INTEGER NOT NULL,"
             "_SUPPLIER TEXT NOT NULL,"
             "_ITEMS TEXT,"
             "_TOTAL REAL)");

  query.exec("CREATE TABLE IF NOT EXISTS _ITEMS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_DESCRIPTION TEXT NOT NULL UNIQUE,"
             "_UNITY TEXT NOT NULL,"
             "_SUPPLIER TEXT,"
             "_PRICE REAL,"
             "_DETAILS TEXT,"
             "_MIDASCODE TEXT,"
             "_ICON INT)");

  query.exec("CREATE TABLE IF NOT EXISTS _REMINDERS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_TITLE TEXT,"
             "_MESSAGE TEXT,"
             "_FAVORITE INT,"
             "_CAPITALIZATION INT,"
             "_SIZE INT)");

  query.exec("CREATE TABLE IF NOT EXISTS _CONSUMPTION ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_DATE INTEGER,"
             "_ITEMID INTEGER,"
             "_PRICE REAL,"
             "_AMMOUNT REAL,"
             "_TOTAL REAL)");

  if (!bHasConfig)
    query.exec("INSERT INTO _SETTINGS DEFAULT VALUES");

  bool bSuccess = db.commit();
  if (!bSuccess)
    error = db.lastError().text();

  return true;
}

bool BaitaDatabase::hasConfig(QSqlDatabase db)
{
  QString error;
  if (!isOpen(db, error))
    return false;

  QSqlQuery query(db);
  if (query.exec("SELECT * FROM _SETTINGS LIMIT 1"))
    return query.next();

  return false;
}

bool BaitaDatabase::insertSettings(QSqlDatabase db,
                              const Settings& settings,
                              QString& error)
{
  if (!isOpen(db, error))
    return false;

  QSqlQuery query(db);
  bool bSuccess = query.prepare("UPDATE _SETTINGS SET "
                                "_BAUDRATE = :_baudrate,"
                                "_DATABITS = :_databits,"
                                "_FLOWCONTROL = :_flowcontrol,"
                                "_PARITY = :_parity,"
                                "_STOPBITS = :_stopbits,"
                                "_SERIALPORT = :_serialport");
  if (bSuccess)
  {
    query.bindValue(":_baudrate", (int)settings.baudRate);
    query.bindValue(":_databits", (int)settings.dataBits);
    query.bindValue(":_flowcontrol", (int)settings.flowControl);
    query.bindValue(":_parity", (int)settings.parity);
    query.bindValue(":_stopbits", (int)settings.stopBits);
    query.bindValue(":_serialport", settings.port);
    bSuccess = query.exec();
  }

  if (!bSuccess)
    error = query.lastError().text();
  return bSuccess;
}

void BaitaDatabase::selectSettings(QSqlDatabase db,
                                   Settings& settings)
{
  settings.clear();
  QString error;
  if (isOpen(db, error))
  {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM _SETTINGS LIMIT 1");
    if (query.exec())
    {
      if (query.next())
      {
        settings.baudRate = (QSerialPort::BaudRate)
                            query.value(query.record().indexOf("_BAUDRATE")).toInt();
        settings.dataBits = (QSerialPort::DataBits)
                            query.value(query.record().indexOf("_DATABITS")).toInt();
        settings.flowControl = (QSerialPort::FlowControl)
                               query.value(query.record().indexOf("_FLOWCONTROL")).toInt();
        settings.parity = (QSerialPort::Parity)
                               query.value(query.record().indexOf("_PARITY")).toInt();
        settings.stopBits = (QSerialPort::StopBits)
                               query.value(query.record().indexOf("_STOPBITS")).toInt();
        settings.port = query.value(query.record().indexOf("_SERIALPORT")).toString();
      }
    }
    else
    {
      qDebug() << query.lastError().text();
    }
  }
}

bool ItemDatabase::select(QSqlDatabase db,
                          Item& item,
                          QString& error)
{
  error.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("SELECT "
                "_DESCRIPTION,"
                "_UNITY,"
                "_SUPPLIER,"
                "_PRICE,"
                "_DETAILS,"
                "_MIDASCODE,"
                "_ICON "
                "FROM _ITEMS "
                "WHERE _ID = (:_id)");

  query.bindValue(":_id", item.m_id);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    bSuccess = query.next();
    if (bSuccess)
    {
      item.m_description = query.value(query.record().indexOf("_DESCRIPTION")).toString();
      item.m_unity = query.value(query.record().indexOf("_UNITY")).toString();
      item.m_supplier = query.value(query.record().indexOf("_SUPPLIER")).toString();
      item.m_price = query.value(query.record().indexOf("_PRICE")).toDouble();
      item.m_details = query.value(query.record().indexOf("_DETAILS")).toString();
      item.m_midasCode = query.value(query.record().indexOf("_MIDASCODE")).toString();
      item.m_icon = query.value(query.record().indexOf("_ICON")).toInt();
    }
    else
    {
      item.clear();
      item.m_description = "Item não encontrado.";
      item.m_unity = "?";
      error = "Item não encontrado.";
    }
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

bool ReminderSQL::select(QSqlDatabase db,
                         Reminder& r,
                         QString error)
{
  error.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("SELECT "
                "_TITLE,"
                "_MESSAGE,"
                "_FAVORITE,"
                "_CAPITALIZATION,"
                "_SIZE "
                "FROM _REMINDERS "
                "WHERE _ID = (:_id)");

  query.bindValue(":_id", r.m_id);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    bSuccess = query.next();
    if (bSuccess)
    {
      r.m_title = query.value(query.record().indexOf("_TITLE")).toString();
      r.m_message = query.value(query.record().indexOf("_MESSAGE")).toString();
      r.m_bFavorite = query.value(query.record().indexOf("_FAVORITE")).toBool();
      r.m_capitalization = (Reminder::Capitalization)
                           query.value(query.record().indexOf("_CAPITALIZATION")).toInt();
      r.m_size = (Reminder::Size)
                 query.value(query.record().indexOf("_SIZE")).toInt();
    }
    else
    {
      r.clear();
      r.m_title = "Lembrete não encontrado.";
      error = "Lembrete não encontrado.";
    }
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

bool ReminderSQL::insertOrUpdate(QSqlDatabase db,
                                 const Reminder& r,
                                 QString& error)
{
  error.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  bool bSuccess = false;
  QSqlQuery query(db);
  if (r.isValidID())
  {
    bSuccess = query.prepare("UPDATE _REMINDERS SET "
                             "_TITLE = :_title,"
                             "_MESSAGE = :_msg,"
                             "_FAVORITE = :_fav,"
                             "_CAPITALIZATION = :_cap,"
                             "_SIZE = :_size"
                             " WHERE _ID = (:_id)");
    if (bSuccess)
      query.bindValue(":_id", r.m_id);
  }
  else
  {
    bSuccess = query.prepare("INSERT INTO _REMINDERS ("
                             "_TITLE,"
                             "_MESSAGE,"
                             "_FAVORITE,"
                             "_CAPITALIZATION,"
                             "_SIZE) "
                             "VALUES ("
                             "(:_title),"
                             "(:_msg),"
                             "(:_fav),"
                             "(:_cap),"
                             "(:_size))");
  }

  if (bSuccess)
  {
    query.bindValue(":_id", r.m_id);
    query.bindValue(":_title", r.m_title);
    query.bindValue(":_msg", r.m_message);
    query.bindValue(":_fav", r.m_bFavorite);
    query.bindValue(":_cap", (int)r.m_capitalization);
    query.bindValue(":_size", (int)r.m_size);
    bSuccess = query.exec();
  }

  if (!bSuccess)
    error = query.lastError().text();

  return bSuccess;
}

void ReminderSQL::setFavorite(QSqlDatabase db,
                              int id,
                              bool bFav)
{
    QSqlQuery query(db);
    if (query.prepare("UPDATE _REMINDERS SET "
                      "_FAVORITE = :_fav"
                      " WHERE _ID = (:_id)"))
    {
      query.bindValue(":_id", id);
      query.bindValue(":_fav", bFav);
    }
    query.exec();
}

bool ReminderSQL::isFavorite(QSqlDatabase db,
                             int id)
{
  QSqlQuery query(db);
  if (query.prepare("SELECT "
                    "_FAVORITE "
                    "FROM _REMINDERS "
                    "WHERE _ID = (:_id)"))
  {
    query.bindValue(":_id", id);
    if (query.exec())
      if (query.next())
        return query.value(0).toBool();
  }
  return false;
}

bool ConsumptionSQL::selectTotal(QSqlDatabase db,
                                 const Consumption::Filter& filter,
                                 double& total,
                                 QString& error)
{
  error.clear();
  total = 0.0;

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QString str("SELECT SUM(_TOTAL) FROM _CONSUMPTION");
  if (filter.m_bDate)
  {
    str += " WHERE _DATE BETWEEN " +
           QString::number(filter.m_datei) +
           " AND " +
           QString::number(filter.m_datef);
  }

  QSqlQuery query(db);
  bool bSuccess = query.exec(str);
  if (bSuccess)
  {
    bSuccess = query.next();
    if (bSuccess)
      total = query.value(0).toDouble();
    else
      error = "Valor não encontrado.";
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

bool ConsumptionSQL::selectSubTotal(QSqlDatabase db,
                                    const Consumption::Filter& filter,
                                    QVector<qint64>& dates,
                                    QVector<double>& totals,
                                    QString &error)
{
  dates.clear();
  totals.clear();
  error.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QString str("SELECT SUM(_TOTAL), _DATE FROM _CONSUMPTION");
  if (filter.m_bDate)
  {
    str += " WHERE _DATE BETWEEN " +
           QString::number(filter.m_datei) +
           " AND " +
           QString::number(filter.m_datef);
  }
  str += " GROUP BY _DATE";

  QSqlQuery query(db);
  bool bSuccess = query.exec(str);
  if (bSuccess)
  {
    bSuccess = false;
    while (query.next())
    {
      totals.push_back(query.value(0).toDouble());
      dates.push_back(query.value(1).toLongLong());
      bSuccess = true;
    }
    if (!bSuccess)
      error = "Valor não encontrado.";
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

bool ConsumptionSQL::selectByDate(QSqlDatabase db,
                                  qint64 date,
                                  QVector<Consumption>& consumptions,
                                  QString& error)
{
  consumptions.clear();
  error.clear();

  if (!BaitaDatabase::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  bool bSuccess = query.exec("SELECT "
                             "_ID,"
                             "_DATE,"
                             "_ITEMID,"
                             "_PRICE,"
                             "_AMMOUNT,"
                             "_TOTAL "
                             "FROM _CONSUMPTION "
                             "WHERE _DATE = " + QString::number(date));

  if (bSuccess)
  {
    while (query.next())
    {
      Consumption c;
      c.m_id = query.value(0).toInt();
      c.m_date = query.value(1).toLongLong();
      c.m_itemID = query.value(2).toInt();
      c.m_price = query.value(3).toDouble();
      c.m_ammount = query.value(4).toDouble();
      c.m_total = query.value(5).toDouble();
      consumptions.push_back(c);
    }
  }
  else
  {
    error = query.lastError().text();
  }

  return bSuccess;
}

void ConsumptionSQL::getConsumption(QSqlDatabase db,
                                    qint64 date,
                                    QVector<Consumption>& vConsumption,
                                    QVector<Item>& vItem)
{
  vConsumption.clear();
  vItem.clear();

  QString error;
  selectByDate(db, date, vConsumption, error);

  for (int i = 0; i != vConsumption.size(); ++i)
  {
    Item item;
    item.m_id = vConsumption.at(i).m_itemID;
    ItemDatabase::select(db, item, error);
    vItem.push_back(item);
  }
}

double ConsumptionSQL::getTotal(QSqlDatabase db,
                                const Consumption::Filter& filter)
{
  double total = 0.0;
  QString error;
  ConsumptionSQL::selectTotal(db, filter, total, error);
  return total;
}

double ConsumptionSQL::getTotal(QSqlDatabase db,
                                qint64 date)
{
  double total = 0.0;
  QString error;
  Consumption::Filter filter;
  filter.m_bDate = true;
  filter.m_datei = date;
  filter.m_datef = date;
  ConsumptionSQL::selectTotal(db, filter, total, error);
  return total;
}
