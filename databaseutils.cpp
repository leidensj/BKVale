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

bool NoteSQL::insert(QSqlDatabase db,
                     const Note& note,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO _PROMISSORYNOTES ("
                "_NUMBER,"
                "_DATE,"
                "_SUPPLIER,"
                "_TOTAL) "
                "VALUES ("
                "(:_number),"
                "(:_date),"
                "(:_supplier),"
                "(:_total))");
  query.bindValue(":_number", note.m_number);
  query.bindValue(":_date", note.m_date);
  query.bindValue(":_supplier", note.m_supplier);
  query.bindValue(":_total", note.m_total);
  query.exec();

  note.m_id = query.lastInsertId().toInt();

  for (int i = 0; i != note.m_items.size(); ++i)
  {
    query.prepare("INSERT INTO _PROMISSORYNOTESITEMS ("
                  "_NOTEID,"
                  "_AMMOUNT,"
                  "_PRICE,"
                  "_UNITY,"
                  "_DESCRIPTION) "
                  "VALUES ("
                  "(:_noteid),"
                  "(:_ammount),"
                  "(:_price),"
                  "(:_unity),"
                  "(:_description))");
    query.bindValue(":_noteid", note.m_id);
    query.bindValue(":_ammount", note.m_items.at(i).m_ammount);
    query.bindValue(":_price", note.m_items.at(i).m_price);
    query.bindValue(":_unity", note.m_items.at(i).m_unity);
    query.bindValue(":_description", note.m_items.at(i).m_description);
    query.exec();
    note.m_items.at(i).m_id = query.lastInsertId().toInt();
  }

  query.exec("SELECT _LASTNUMBER FROM _SETTINGS");
  int n = query.next()
          ? query.value(query.record().indexOf("_LASTNUMBER")).toInt()
          : DEFAULT_NUMBER;
  n++;
  query.prepare("UPDATE _SETTINGS SET _LASTNUMBER = :_number");
  query.bindValue(":_number", n);
  query.exec();

  if (db.commit())
    return true;

  error = query.lastError().text();
  return false;
}

bool NoteSQL::update(QSqlDatabase db,
                     const Note& note,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE _PROMISSORYNOTES SET "
                "_NUMBER = (:_number),"
                "_DATE = (:_date),"
                "_SUPPLIER = (:_supplier),"
                "_TOTAL = (:_total) "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", note.m_id);
  query.bindValue(":_number", note.m_number);
  query.bindValue(":_date", note.m_date);
  query.bindValue(":_supplier", note.m_supplier);
  query.bindValue(":_total", note.m_total);
  query.exec();

  for (int i = 0; i != note.m_items.size(); ++i)
  {
    if (Note::isValidID(note.m_items.at(i).m_id))
    {
      query.prepare("UPDATE _PROMISSORYNOTESITEMS SET "
                    "_NOTEID = (:_noteid),"
                    "_AMMOUNT = (:_ammount),"
                    "_PRICE = (:_price),"
                    "_UNITY = (:_price),"
                    "_DESCRIPTION = (:_description) "
                    "WHERE _ID = (:_id)");
      query.bindValue(":_id", note.m_items.at(i).m_id);
      query.bindValue(":_noteid", note.m_id);
      query.bindValue(":_ammount", note.m_items.at(i).m_ammount);
      query.bindValue(":_price", note.m_items.at(i).m_price);
      query.bindValue(":_unity", note.m_items.at(i).m_unity);
      query.bindValue(":_description", note.m_items.at(i).m_description);
      query.exec();
    }
    else
    {
      query.prepare("INSERT INTO _PROMISSORYNOTESITEMS ("
                    "_NOTEID,"
                    "_AMMOUNT,"
                    "_PRICE,"
                    "_UNITY,"
                    "_DESCRIPTION) "
                    "VALUES ("
                    "(:_noteid),"
                    "(:_ammount),"
                    "(:_price),"
                    "(:_unity),"
                    "(:_description))");
      query.bindValue(":_noteid", note.m_id);
      query.bindValue(":_ammount", note.m_items.at(i).m_ammount);
      query.bindValue(":_price", note.m_items.at(i).m_price);
      query.bindValue(":_unity", note.m_items.at(i).m_unity);
      query.bindValue(":_description", note.m_items.at(i).m_description);
      query.exec();
      note.m_items.at(i).m_id = query.lastInsertId().toInt();
    }

  }

  if (db.commit())
    return true;

  error = query.lastError().text();
  return false;
}

bool NoteSQL::select(QSqlDatabase db,
                     Note& note,
                     QString& error)
{
  error.clear();
  int id = note.m_id;
  note.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  bool bFound = false;
  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                "_NUMBER,"
                "_DATE,"
                "_SUPPLIER,"
                "_TOTAL "
                "FROM _PROMISSORYNOTES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  query.exec();
  if (query.next())
  {
    note.m_id = id;
    note.m_number = query.value(query.record().indexOf("_NUMBER")).toInt();
    note.m_date = query.value(query.record().indexOf("_DATE")).toLongLong();
    note.m_supplier = query.value(query.record().indexOf("_SUPPLIER")).toString();
    note.m_total = query.value(query.record().indexOf("_TOTAL")).toDouble();
    bFound = true;
  }

  query.prepare("SELECT "
                "_ID,"
                "_AMMOUNT,"
                "_PRICE,"
                "_UNITY,"
                "_DESCRIPTION "
                "FROM _PROMISSORYNOTESITEMS "
                "WHERE _NOTEID = (:_noteId)");
  query.bindValue(":_noteId", id);
  query.exec();
  while (query.next())
  {
    NoteItem item;
    item.m_id = query.value(query.record().indexOf("_ID")).toInt();
    item.m_ammount = query.value(query.record().indexOf("_AMMOUNT")).toDouble();
    item.m_price = query.value(query.record().indexOf("_PRICE")).toDouble();
    item.m_unity = query.value(query.record().indexOf("_UNITY")).toString();
    item.m_description = query.value(query.record().indexOf("_DESCRIPTION")).toString();
    note.m_items.push_back(item);
  }

  if (db.commit())
  {
    if (!bFound)
      error = "Vale não encontrado.";
    return bFound;
  }
  else
  {
    error = query.lastError().text();
    return false;
  }
}

bool NoteSQL::remove(QSqlDatabase db,
                     int id,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM _PROMISSORYNOTES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  query.exec();
  query.prepare("DELETE FROM _PROMISSORYNOTESITEMS "
                "WHERE _NOTEID = (:_id)");
  query.bindValue(":_id", id);
  query.exec();

  if (db.commit())
    return true;

  error = query.lastError().text();
  return false;
}

bool BaitaSQL::isOpen(QSqlDatabase db,
                      QString& error)
{
  error.clear();
  if (!db.isOpen())
     error = "Arquivo de configuração não foi aberto.";
  return db.isOpen();
}

bool BaitaSQL::open(QSqlDatabase db,
                    const QString& path,
                    QString& error)
{
  error.clear();
  if (db.isOpen())
    db.close();
  db.addDatabase("QSQLITE");
  db.setDatabaseName(path);
  bool bSuccess = db.open();
  if (!bSuccess)
    error = db.lastError().text();
  return bSuccess;
}

void BaitaSQL::close(QSqlDatabase db)
{
  db.close();
}

bool BaitaSQL::init(QSqlDatabase db,
                    QString& error)
{
  error.clear();

  if (!isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.exec("CREATE TABLE IF NOT EXISTS _SETTINGS ("
             "_LASTNUMBER INTEGER DEFAULT " DEFAULT_NUMBER_STR ","
             "_SERIALPORT TEXT,"
             "_BAUDRATE INTEGER DEFAULT 9600,"
             "_DATABITS INTEGER DEFAULT 8,"
             "_FLOWCONTROL INTEGER DEFAULT 0,"
             "_PARITY INTEGER DEFAULT 0,"
             "_STOPBITS INTEGER DEFAULT 1)");

  query.exec("SELECT * FROM _SETTINGS LIMIT 1");
  if (!query.next())
    query.exec("INSERT INTO _SETTINGS DEFAULT VALUES");

  query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NUMBER INTEGER NOT NULL,"
             "_DATE INTEGER NOT NULL,"
             "_SUPPLIER TEXT NOT NULL,"
             "_TOTAL REAL)");

  query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTESITEMS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NOTEID INTEGER NOT NULL,"
             "_AMMOUNT REAL,"
             "_PRICE REAL,"
             "_UNITY TEXT,"
             "_DESCRIPTION TEXT)");

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

  query.exec("CREATE TABLE IF NOT EXISTS _USERS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_USER TEXT NOT NULL UNIQUE,"
             "_PASSWORD TEXT NOT NULL,"
             "_ACCESS_NOTE INT,"
             "_ACCESS_REMINDER INT,"
             "_ACCESS_CALCULATOR INT,"
             "_ACCESS_SHOP INT,"
             "_ACCESS_CONSUMPTION INT,"
             "_ACCESS_USER INT,"
             "_ACCESS_ITEM INT,"
             "_ACCESS_SETTINGS INT)");

  query.exec("SELECT * FROM _USERS LIMIT 1");
  if (!query.next())
  {
     query.exec("INSERT INTO _USERS ("
                "_USER,"
                "_PASSWORD,"
                "_ACCESS_NOTE,"
                "_ACCESS_REMINDER,"
                "_ACCESS_CALCULATOR,"
                "_ACCESS_SHOP,"
                "_ACCESS_CONSUMPTION,"
                "_ACCESS_USER,"
                "_ACCESS_ITEM,"
                "_ACCESS_SETTINGS) "
                "VALUES ("
                "'ADMIN',"
                "'a1b2c3d4',"
                "1,1,1,1,1,1,1,1)");
  }

  bool bSuccess = db.commit();
  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
}

bool BaitaSQL::insertSettings(QSqlDatabase db,
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

void BaitaSQL::selectSettings(QSqlDatabase db,
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

bool ItemSQL::select(QSqlDatabase db,
                     Item& item,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
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

  if (!BaitaSQL::isOpen(db, error))
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

  if (!BaitaSQL::isOpen(db, error))
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

  if (!BaitaSQL::isOpen(db, error))
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

  if (!BaitaSQL::isOpen(db, error))
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

  if (!BaitaSQL::isOpen(db, error))
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
    ItemSQL::select(db, item, error);
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

bool UserSQL::insert(QSqlDatabase db,
                     const User& user,
                     const QString& strPassword,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("INSERT INTO _USERS ("
                "_USER,"
                "_PASSWORD,"
                "_ACCESS_NOTE,"
                "_ACCESS_REMINDER,"
                "_ACCESS_CALCULATOR,"
                "_ACCESS_SHOP,"
                "_ACCESS_CONSUMPTION,"
                "_ACCESS_USER,"
                "_ACCESS_ITEM,"
                "_ACCESS_SETTINGS) "
                "VALUES ("
                "(:_user),"
                "(:_password),"
                "(:_accessNote),"
                "(:_accessReminder),"
                "(:_accessCalculator),"
                "(:_accessShop),"
                "(:_accessConsumption),"
                "(:_accessUser),"
                "(:_accessItem),"
                "(:_accessSettings))");
  query.bindValue(":_user", user.m_strUser);
  query.bindValue(":_password", user.strEncryptedPassword(strPassword));
  query.bindValue(":_accessNote", user.m_bAccessNote);
  query.bindValue(":_accessReminder", user.m_bAccessReminder);
  query.bindValue(":_accessCalculator", user.m_bAccessCalculator);
  query.bindValue(":_accessShop", user.m_bAccessShop);
  query.bindValue(":_accessConsumption", user.m_bAccessConsumption);
  query.bindValue(":_accessUser", user.m_bAccessUser);
  query.bindValue(":_accessItem", user.m_bAccessItem);
  query.bindValue(":_accessSettings", user.m_bAccessSettings);

  if (query.exec())
  {
    user.m_id = query.lastInsertId().toInt();
    return true;
  }

  error = query.lastError().text();
  return false;
}

bool UserSQL::update(QSqlDatabase db,
                     const User& user,
                     const QString& strPassword,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QString strQuery("UPDATE _USERS SET "
                   "_USER = (:_user),");
  if (!strPassword.isEmpty())
    strQuery += "_PASSWORD = (:_password),";
  strQuery += "_ACCESS_NOTE = (:_accessNote),"
              "_ACCESS_REMINDER = (:_accessReminder),"
              "_ACCESS_CALCULATOR = (:_accessCalculator),"
              "_ACCESS_SHOP = (:_accessShop),"
              "_ACCESS_CONSUMPTION = (:_accessConsumption),"
              "_ACCESS_USER = (:_accessUser),"
              "_ACCESS_ITEM = (:_accessItem),"
              "_ACCESS_SETTINGS = (:_accessSettings) "
              "WHERE _ID = (:_id)";


  QSqlQuery query(db);
  query.prepare(strQuery);
  query.bindValue(":_id", user.m_id);
  query.bindValue(":_user", user.m_strUser);
  if (!strPassword.isEmpty())
    query.bindValue(":_password", user.strEncryptedPassword(strPassword));
  query.bindValue(":_accessNote", user.m_bAccessNote);
  query.bindValue(":_accessReminder", user.m_bAccessReminder);
  query.bindValue(":_accessCalculator", user.m_bAccessCalculator);
  query.bindValue(":_accessShop", user.m_bAccessShop);
  query.bindValue(":_accessConsumption", user.m_bAccessConsumption);
  query.bindValue(":_accessUser", user.m_bAccessUser);
  query.bindValue(":_accessItem", user.m_bAccessItem);
  query.bindValue(":_accessSettings", user.m_bAccessSettings);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool UserSQL::select(QSqlDatabase db,
                     User& user,
                     QString& error)
{
  error.clear();
  int id = user.m_id;
  user.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  bool bFound = false;
  QSqlQuery query(db);
  query.prepare("SELECT "
                "_USER,"
                "_ACCESS_NOTE,"
                "_ACCESS_REMINDER,"
                "_ACCESS_CALCULATOR,"
                "_ACCESS_SHOP,"
                "_ACCESS_CONSUMPTION,"
                "_ACCESS_USER,"
                "_ACCESS_ITEM,"
                "_ACCESS_SETTINGS "
                "FROM _USERS "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  if (query.exec())
  {
    if (query.next())
    {
      user.m_id = id;
      user.m_strUser = query.value(query.record().indexOf("_USER")).toString();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_NOTE")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_REMINDER")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_CALCULATOR")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_SHOP")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_CONSUMPTION")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_USER")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_ITEM")).toBool();
      user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_SETTINGS")).toBool();
      bFound = true;
    }

    if (!bFound)
      error = "Usuário não encontrado.";
    return bFound;
  }

  error = query.lastError().text();
  return false;
}

bool UserSQL::remove(QSqlDatabase db,
                     int id,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("DELETE FROM _USERS "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

UserLoginSQL::UserLoginSQL(QSqlDatabase db)
  : m_db(db)
{

}

bool UserLoginSQL::login(const QString& strUser,
                         const QString& strPassword,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(m_db, error))
    return false;

  QSqlQuery query(m_db);
  query.prepare("SELECT * FROM _USERS "
                "WHERE _USER = (:_user) AND "
                "_PASSWORD = (:_password) LIMIT 1");
  query.bindValue(":_user", strUser);
  query.bindValue(":_user", User::strEncryptedPassword(strPassword));

  if (query.exec())
  {
    bool bFound = false;
    if (query.next())
    {
      m_user.m_id = query.value(query.record().indexOf("_ID")).toInt();
      m_user.m_strUser = query.value(query.record().indexOf("_USER")).toString();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_NOTE")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_REMINDER")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_CALCULATOR")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_SHOP")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_CONSUMPTION")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_USER")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_ITEM")).toBool();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_SETTINGS")).toBool();
      bFound = true;
    }

    if (!bFound)
      error = "Usuário ou senha inválidos.";
    return bFound;
  }

  error = query.lastError().text();
  return false;
}
