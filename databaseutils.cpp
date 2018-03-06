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

  QSqlQuery query("SELECT MAX(_NUMBER) FROM _PROMISSORYNOTES", db);
  int number = query.next() ? query.value(0).toInt() + 1 : DEFAULT_NUMBER;
  return number > DEFAULT_NUMBER ? number : DEFAULT_NUMBER;
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

  query.exec("SELECT MAX(_NUMBER) FROM _PROMISSORYNOTES");
  int number = query.next()
               ? query.value(0).toInt() + 1
               : DEFAULT_NUMBER;
  number = number > DEFAULT_NUMBER ? number : DEFAULT_NUMBER;

  query.prepare("INSERT INTO _PROMISSORYNOTES ("
                "_NUMBER,"
                "_DATE,"
                "_SUPPLIER,"
                "_TOTAL,"
                "_CASH) "
                "VALUES ("
                "(:_number),"
                "(:_date),"
                "(:_supplier),"
                "(:_total),"
                "(:_cash))");
  query.bindValue(":_number", number);
  query.bindValue(":_date", note.m_date);
  query.bindValue(":_supplier", note.m_supplier);
  query.bindValue(":_total", note.m_total);
  query.bindValue(":_cash", note.m_bCash);
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
                "_DATE = (:_date),"
                "_SUPPLIER = (:_supplier),"
                "_TOTAL = (:_total),"
                "_CASH = (:_cash) "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", note.m_id);
  query.bindValue(":_date", note.m_date);
  query.bindValue(":_supplier", note.m_supplier);
  query.bindValue(":_total", note.m_total);
  query.bindValue(":_cash", note.m_bCash);
  query.exec();

  {
    QString strQuery = "DELETE FROM _PROMISSORYNOTESITEMS "
                       "WHERE _NOTEID = (:_noteid) AND "
                       "_ID NOT IN (";
    for (int i = 0; i != note.m_items.size(); ++i)
      strQuery += "(:_id" + QString::number(i) + "),";
    strQuery.replace(strQuery.length() - 1, 1, ")");
    query.prepare(strQuery);
  }

  query.bindValue(":_noteid", note.m_id);
  for (int i = 0; i != note.m_items.size(); ++i)
    query.bindValue("_id" + QString::number(i), note.m_items.at(i).m_id);
  query.exec();

  for (int i = 0; i != note.m_items.size(); ++i)
  {
    if (Note::isValidID(note.m_items.at(i).m_id))
    {
      query.prepare("UPDATE _PROMISSORYNOTESITEMS SET "
                    "_NOTEID = (:_noteid),"
                    "_AMMOUNT = (:_ammount),"
                    "_PRICE = (:_price),"
                    "_UNITY = (:_unity),"
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
                     int& number,
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
                "_TOTAL,"
                "_CASH "
                "FROM _PROMISSORYNOTES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  query.exec();
  if (query.next())
  {
    note.m_id = id;
    number = query.value(query.record().indexOf("_NUMBER")).toInt();
    note.m_date = query.value(query.record().indexOf("_DATE")).toLongLong();
    note.m_supplier = query.value(query.record().indexOf("_SUPPLIER")).toString();
    note.m_total = query.value(query.record().indexOf("_TOTAL")).toDouble();
    note.m_bCash = query.value(query.record().indexOf("_CASH")).toBool();
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
     error = "Banco de dados não foi aberto.";
  return db.isOpen();
}

bool BaitaSQL::open(QSqlDatabase db,
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
  else
  {
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");
  }
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

  bool bSuccess = true;

  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTES ("
                        "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "_NUMBER INTEGER NOT NULL,"
                        "_DATE INTEGER NOT NULL,"
                        "_SUPPLIER TEXT NOT NULL,"
                        "_TOTAL REAL,"
                        "_CASH INT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTESITEMS ("
                          "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "_NOTEID INTEGER NOT NULL,"
                          "_AMMOUNT REAL,"
                          "_PRICE REAL,"
                          "_UNITY TEXT,"
                          "_DESCRIPTION TEXT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_IMAGE_TABLE_NAME " ("
                          SQL_IMAGE_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          SQL_IMAGE_COL01 " TEXT NOT NULL UNIQUE,"
                          SQL_IMAGE_COL02 " BLOB)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_CATEGORY_TABLE_NAME " ("
                          SQL_CATEGORY_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          SQL_CATEGORY_COL01 " INTEGER,"
                          SQL_CATEGORY_COL02 " TEXT NOT NULL UNIQUE,"
                          "FOREIGN KEY(" SQL_CATEGORY_COL00 ") REFERENCES "
                          SQL_IMAGE_TABLE_NAME "(" SQL_IMAGE_COL00 "))");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _REMINDERS ("
                          "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "_TITLE TEXT,"
                          "_MESSAGE TEXT,"
                          "_FAVORITE INT,"
                          "_CAPITALIZATION INT,"
                          "_SIZE INT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _CONSUMPTION ("
                          "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "_DATE INTEGER,"
                          "_PRODUCTID INTEGER,"
                          "_PRICE REAL,"
                          "_AMMOUNT REAL,"
                          "_TOTAL REAL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_USER_TABLE_NAME " ("
                          SQL_USER_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          SQL_USER_COL01 " TEXT NOT NULL UNIQUE,"
                          SQL_USER_COL02 " TEXT NOT NULL,"
                          SQL_USER_COL03 " INT,"
                          SQL_USER_COL04 " INT,"
                          SQL_USER_COL05 " INT,"
                          SQL_USER_COL06 " INT,"
                          SQL_USER_COL07 " INT,"
                          SQL_USER_COL08 " INT,"
                          SQL_USER_COL09 " INT,"
                          SQL_USER_COL10 " INT)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_PRODUCT_TABLE_NAME " ("
                        SQL_PRODUCT_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        SQL_PRODUCT_COL01 " TEXT NOT NULL UNIQUE,"
                        SQL_PRODUCT_COL02 " INTEGER,"
                        SQL_PRODUCT_COL03 " INTEGER,"
                        SQL_PRODUCT_COL04 " TEXT NOT NULL,"
                        SQL_PRODUCT_COL05 " TEXT,"
                        SQL_PRODUCT_COL06 " REAL,"
                        SQL_PRODUCT_COL07 " TEXT,"
                        SQL_PRODUCT_COL08 " TEXT,"
                        SQL_PRODUCT_COL09 " INT,"
                        SQL_PRODUCT_COL10 " INT,"
                        SQL_PRODUCT_COL11 " INT,"
                        SQL_PRODUCT_COL12 " INT,"
                        SQL_PRODUCT_COL13 " INT,"
                        "FOREIGN KEY(" SQL_PRODUCT_COL02 ") REFERENCES "
                        SQL_CATEGORY_TABLE_NAME "(" SQL_CATEGORY_COL00 "),"
                        "FOREIGN KEY(" SQL_PRODUCT_COL03 ") REFERENCES "
                        SQL_IMAGE_TABLE_NAME "(" SQL_IMAGE_COL00 "))");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_PERSON_TABLE_NAME " ("
                          SQL_PERSON_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          SQL_PERSON_COL01 " INTEGER,"
                          SQL_PERSON_COL02 " TEXT NOT NULL UNIQUE,"
                          SQL_PERSON_COL03 " TEXT,"
                          SQL_PERSON_COL04 " TEXT,"
                          SQL_PERSON_COL05 " TEXT,"
                          SQL_PERSON_COL06 " TEXT,"
                          SQL_PERSON_COL07 " TEXT,"
                          SQL_PERSON_COL08 " TEXT,"
                          SQL_PERSON_COL09 " TEXT,"
                          SQL_PERSON_COL10 " INT,"
                          SQL_PERSON_COL11 " INT,"
                          SQL_PERSON_COL12 " INT,"
                          SQL_PERSON_COL13 " INT,"
                          "FOREIGN KEY(" SQL_PERSON_COL01 ") REFERENCES "
                          SQL_IMAGE_TABLE_NAME "(" SQL_IMAGE_COL00 "))");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_ADDRESS_TABLE_NAME " ("
                        SQL_ADDRESS_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        SQL_ADDRESS_COL01 " INTEGER,"
                        SQL_ADDRESS_COL02 " TEXT,"
                        SQL_ADDRESS_COL03 " TEXT,"
                        SQL_ADDRESS_COL04 " TEXT,"
                        SQL_ADDRESS_COL05 " INT,"
                        SQL_ADDRESS_COL06 " TEXT,"
                        SQL_ADDRESS_COL07 " INT,"
                        SQL_ADDRESS_COL08 " TEXT,"
                        SQL_ADDRESS_COL09 " TEXT,"
                        "FOREIGN KEY(" SQL_ADDRESS_COL01 ") REFERENCES "
                        SQL_PERSON_TABLE_NAME "(" SQL_ADDRESS_COL00 "))");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_PHONE_TABLE_NAME " ("
                        SQL_PHONE_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        SQL_PHONE_COL01 " INTEGER,"
                        SQL_PHONE_COL02 " INT DEFAULT " DEFAULT_PHONE_COUNTRY_CODE_VALUE_STR ","
                        SQL_PHONE_COL03 " INT DEFAULT " DEFAULT_PHONE_CODE_VALUE_STR ","
                        SQL_PHONE_COL04 " TEXT,"
                        "FOREIGN KEY(" SQL_PHONE_COL01 ") REFERENCES "
                        SQL_PERSON_TABLE_NAME "(" SQL_PERSON_COL00 "))");

  if (bSuccess)
  {
    query.exec("SELECT * FROM " SQL_USER_TABLE_NAME " LIMIT 1");
    if (!query.next())
    {
      query.prepare("INSERT INTO " SQL_USER_TABLE_NAME " ("
                    SQL_USER_COL01 ","
                    SQL_USER_COL02 ","
                    SQL_USER_COL03 ","
                    SQL_USER_COL04 ","
                    SQL_USER_COL05 ","
                    SQL_USER_COL06 ","
                    SQL_USER_COL07 ","
                    SQL_USER_COL08 ","
                    SQL_USER_COL09 ","
                    SQL_USER_COL10 ")"
                    " VALUES ('"
                    SQL_USER_DEFAULT_NAME "',"
                    "(:_password),"
                    "1,1,1,1,1,1,1,1)");
      query.bindValue(":_password", User::st_strEncryptedPassword(SQL_USER_DEFAULT_PASSWORD));
      bSuccess = query.exec();
    }
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    db.rollback();
    return false;
  }
  else
  {
    bSuccess = db.commit();
    if (!bSuccess)
      error = db.lastError().text();
    return bSuccess;
  }
}

bool ProductSQL::select(QSqlDatabase db,
                        Product& product,
                        QString& error)
{
  error.clear();
  int id = product.m_id;
  product.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("SELECT "
                SQL_PRODUCT_COL01 ","
                SQL_PRODUCT_COL02 ","
                SQL_PRODUCT_COL03 ","
                SQL_PRODUCT_COL04 ","
                SQL_PRODUCT_COL05 ","
                SQL_PRODUCT_COL06 ","
                SQL_PRODUCT_COL07 ","
                SQL_PRODUCT_COL08 ","
                SQL_PRODUCT_COL09 ","
                SQL_PRODUCT_COL10 ","
                SQL_PRODUCT_COL11 ","
                SQL_PRODUCT_COL12 ","
                SQL_PRODUCT_COL13
                " FROM " SQL_PRODUCT_TABLE_NAME
                " WHERE " SQL_PRODUCT_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  if (query.exec())
  {
    if (query.next())
    {
      product.m_id = id;
      product.m_name = query.value(0).toString();
      product.m_categoryId = query.value(1).toInt();
      product.m_imageId = query.value(2).toInt();
      product.m_unity = query.value(3).toString();
      product.m_packageUnity = query.value(4).toString();
      product.m_packageAmmount = query.value(5).toDouble();
      product.m_details = query.value(6).toString();
      product.m_code = query.value(7).toString();
      product.m_bAvailableAtNotes = query.value(8).toBool();
      product.m_bAvailableAtShop = query.value(9).toBool();
      product.m_bAvailableAtConsumption = query.value(10).toBool();
      product.m_bAvailableToBuy = query.value(11).toBool();
      product.m_bAvailableToSell = query.value(12).toBool();
      return true;
    }
    else
    {
      error = "Produto não encontrado.";
      return false;
    }
  }
  else
  {
    error = query.lastError().text();
    return false;
  }
}

bool ProductSQL::insert(QSqlDatabase db,
                        const Product& product,
                        QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("INSERT INTO " SQL_PRODUCT_TABLE_NAME " ("
                SQL_PRODUCT_COL01 ","
                SQL_PRODUCT_COL02 ","
                SQL_PRODUCT_COL03 ","
                SQL_PRODUCT_COL04 ","
                SQL_PRODUCT_COL05 ","
                SQL_PRODUCT_COL06 ","
                SQL_PRODUCT_COL07 ","
                SQL_PRODUCT_COL08 ","
                SQL_PRODUCT_COL09 ","
                SQL_PRODUCT_COL10 ","
                SQL_PRODUCT_COL11 ","
                SQL_PRODUCT_COL12 ","
                SQL_PRODUCT_COL13 ")"
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
                "(:_v13))");
  query.bindValue(":_v01", product.m_name);
  query.bindValue(":_v02", product.m_categoryId);
  query.bindValue(":_v03", product.m_imageId);
  query.bindValue(":_v04", product.m_unity);
  query.bindValue(":_v05", product.m_packageUnity);
  query.bindValue(":_v06", product.m_packageAmmount);
  query.bindValue(":_v07", product.m_details);
  query.bindValue(":_v08", product.m_code);
  query.bindValue(":_v09", product.m_bAvailableAtNotes);
  query.bindValue(":_v10", product.m_bAvailableAtShop);
  query.bindValue(":_v11", product.m_bAvailableAtConsumption);
  query.bindValue(":_v12", product.m_bAvailableToBuy);
  query.bindValue(":_v13", product.m_bAvailableToSell);

  if (query.exec())
  {
    product.m_id = query.lastInsertId().toInt();
    return true;
  }

  error = query.lastError().text();
  return false;
}

bool ProductSQL::update(QSqlDatabase db,
                        const Product& product,
                        QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("UPDATE " SQL_PRODUCT_TABLE_NAME " SET "
                SQL_PRODUCT_COL01 " = (:_v01),"
                SQL_PRODUCT_COL02 " = (:_v02),"
                SQL_PRODUCT_COL03 " = (:_v03),"
                SQL_PRODUCT_COL04 " = (:_v04),"
                SQL_PRODUCT_COL05 " = (:_v05),"
                SQL_PRODUCT_COL06 " = (:_v06),"
                SQL_PRODUCT_COL07 " = (:_v07),"
                SQL_PRODUCT_COL08 " = (:_v08),"
                SQL_PRODUCT_COL09 " = (:_v09),"
                SQL_PRODUCT_COL10 " = (:_v10),"
                SQL_PRODUCT_COL11 " = (:_v11),"
                SQL_PRODUCT_COL12 " = (:_v12),"
                SQL_PRODUCT_COL13 " = (:_v13)"
                " WHERE " SQL_PRODUCT_COL00 " = (:_v00)");
  query.bindValue(":_v00", product.m_id);
  query.bindValue(":_v01", product.m_name);
  query.bindValue(":_v02", product.m_categoryId);
  query.bindValue(":_v03", product.m_imageId);
  query.bindValue(":_v04", product.m_unity);
  query.bindValue(":_v05", product.m_packageUnity);
  query.bindValue(":_v06", product.m_packageAmmount);
  query.bindValue(":_v07", product.m_details);
  query.bindValue(":_v08", product.m_code);
  query.bindValue(":_v09", product.m_bAvailableAtNotes);
  query.bindValue(":_v10", product.m_bAvailableAtShop);
  query.bindValue(":_v11", product.m_bAvailableAtConsumption);
  query.bindValue(":_v12", product.m_bAvailableToBuy);
  query.bindValue(":_v13", product.m_bAvailableToSell);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool ProductSQL::remove(QSqlDatabase db,
                        int id,
                        QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("DELETE FROM " SQL_PRODUCT_TABLE_NAME
                " WHERE " SQL_PRODUCT_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool CategorySQL::select(QSqlDatabase db,
                         Category& category,
                         QString& error)
{
  error.clear();
  int id = category.m_id;
  category.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("SELECT "
                SQL_CATEGORY_COL01 ","
                SQL_CATEGORY_COL02
                " FROM " SQL_CATEGORY_TABLE_NAME
                " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
  {
    if (query.next())
    {
      category.m_id = id;
      category.m_imageId = query.value(0).toInt();
      category.m_name = query.value(1).toString();
      return true;
    }
    else
    {
      error = "Categoria não encontrada.";
      return false;
    }
  }
  else
  {
    error = query.lastError().text();
    return false;
  }
}

bool CategorySQL::insert(QSqlDatabase db,
                         const Category& category,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("INSERT INTO " SQL_CATEGORY_TABLE_NAME " ("
                SQL_CATEGORY_COL01 ","
                SQL_CATEGORY_COL02 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", category.m_imageId);
  query.bindValue(":_v02", category.m_name);

  if (query.exec())
  {
    category.m_id = query.lastInsertId().toInt();
    return true;
  }

  error = query.lastError().text();
  return false;
}

bool CategorySQL::update(QSqlDatabase db,
                         const Category& category,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("UPDATE " SQL_CATEGORY_TABLE_NAME " SET "
                SQL_CATEGORY_COL01 " = (:_v01),"
                SQL_CATEGORY_COL02 " = (:_v02)"
                " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
  query.bindValue(":_v00", category.m_id);
  query.bindValue(":_v01", category.m_imageId);
  query.bindValue(":_v02", category.m_name);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool CategorySQL::remove(QSqlDatabase db,
                         int id,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("DELETE FROM " SQL_CATEGORY_TABLE_NAME
                " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool ImageSQL::select(QSqlDatabase db,
                      Image& image,
                      QString& error)
{
  error.clear();
  int id = image.m_id;
  image.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("SELECT "
                SQL_IMAGE_COL01 ","
                SQL_IMAGE_COL02
                " FROM " SQL_IMAGE_TABLE_NAME
                " WHERE " SQL_IMAGE_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
  {
    if (query.next())
    {
      image.m_id = id;
      image.m_name = query.value(0).toString();
      image.m_image = query.value(1).toByteArray();
      return true;
    }
    else
    {
      error = "Imagem não encontrada.";
      return false;
    }
  }
  else
  {
    error = query.lastError().text();
    return false;
  }
}

bool ImageSQL::insert(QSqlDatabase db,
                      const Image& image,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("INSERT INTO " SQL_IMAGE_TABLE_NAME " ("
                SQL_IMAGE_COL00 ","
                SQL_IMAGE_COL01 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", image.m_name);
  query.bindValue(":_v02", image.m_image);

  if (query.exec())
  {
    image.m_id = query.lastInsertId().toInt();
    return true;
  }

  error = query.lastError().text();
  return false;
}

bool ImageSQL::update(QSqlDatabase db,
                      const Image& image,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("UPDATE " SQL_IMAGE_TABLE_NAME " SET "
                SQL_IMAGE_COL01 " = (:_v01),"
                SQL_IMAGE_COL02 " = (:_v02)"
                " WHERE " SQL_IMAGE_COL00 " = (:_v00)");
  query.bindValue(":_v00", image.m_id);
  query.bindValue(":_v01", image.m_name);
  query.bindValue(":_v02", image.m_image);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool ImageSQL::remove(QSqlDatabase db,
                      int id,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("DELETE FROM " SQL_IMAGE_TABLE_NAME
                " WHERE " SQL_IMAGE_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
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
                             "_PRODUCTID,"
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
                                    QVector<Product>& vProduct)
{
  vConsumption.clear();
  vProduct.clear();

  QString error;
  selectByDate(db, date, vConsumption, error);

  for (int i = 0; i != vConsumption.size(); ++i)
  {
    Product product;
    product.m_id = vConsumption.at(i).m_itemID;
    ProductSQL::select(db, product, error);
    vProduct.push_back(product);
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
  query.prepare("INSERT INTO " SQL_USER_TABLE_NAME " ("
                SQL_USER_COL01 ","
                SQL_USER_COL02 ","
                SQL_USER_COL03 ","
                SQL_USER_COL04 ","
                SQL_USER_COL05 ","
                SQL_USER_COL06 ","
                SQL_USER_COL07 ","
                SQL_USER_COL08 ","
                SQL_USER_COL09 ","
                SQL_USER_COL10 ")"
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
                "(:_v10))");
  query.bindValue(":_v01", user.m_strUser);
  query.bindValue(":_v02", user.st_strEncryptedPassword(strPassword));
  query.bindValue(":_v03", user.m_bAccessNote);
  query.bindValue(":_v04", user.m_bAccessReminder);
  query.bindValue(":_v05", user.m_bAccessCalculator);
  query.bindValue(":_v06", user.m_bAccessShop);
  query.bindValue(":_v07", user.m_bAccessConsumption);
  query.bindValue(":_v08", user.m_bAccessUser);
  query.bindValue(":_v09", user.m_bAccessProduct);
  query.bindValue(":_v10", user.m_bAccessSettings);

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

  QString strQuery("UPDATE " SQL_USER_TABLE_NAME " SET "
                   SQL_USER_COL01 " = (:_v01),");
  if (!strPassword.isEmpty())
    strQuery += SQL_USER_COL02 " = (:_v02),";
  strQuery += SQL_USER_COL03" = (:_v03),"
              SQL_USER_COL04" = (:_v04),"
              SQL_USER_COL05" = (:_v05),"
              SQL_USER_COL06" = (:_v06),"
              SQL_USER_COL07" = (:_v07),"
              SQL_USER_COL08" = (:_v08),"
              SQL_USER_COL09" = (:_v09),"
              SQL_USER_COL10" = (:_v10)"
              " WHERE " SQL_USER_COL00 " = (:_v00)";


  QSqlQuery query(db);
  query.prepare(strQuery);
  query.bindValue(":_v00", user.m_id);
  query.bindValue(":_v01", user.m_strUser);
  if (!strPassword.isEmpty())
    query.bindValue(":_v02", User::st_strEncryptedPassword(strPassword));
  query.bindValue(":_v03", user.m_bAccessNote);
  query.bindValue(":_v04", user.m_bAccessReminder);
  query.bindValue(":_v05", user.m_bAccessCalculator);
  query.bindValue(":_v06", user.m_bAccessShop);
  query.bindValue(":_v07", user.m_bAccessConsumption);
  query.bindValue(":_v08", user.m_bAccessUser);
  query.bindValue(":_v09", user.m_bAccessProduct);
  query.bindValue(":_v10", user.m_bAccessSettings);

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
                SQL_USER_COL01 ","
                SQL_USER_COL02 ","
                SQL_USER_COL03 ","
                SQL_USER_COL04 ","
                SQL_USER_COL05 ","
                SQL_USER_COL06 ","
                SQL_USER_COL07 ","
                SQL_USER_COL08 ","
                SQL_USER_COL09 ","
                SQL_USER_COL10
                " FROM " SQL_USER_TABLE_NAME
                " WHERE " SQL_ADDRESS_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  if (query.exec())
  {
    if (query.next())
    {
      user.m_id = id;
      user.m_strUser = query.value(0).toString();
      query.value(1).toString(); // password nao precisamos
      user.m_bAccessNote = query.value(2).toBool();
      user.m_bAccessReminder = query.value(3).toBool();
      user.m_bAccessCalculator = query.value(4).toBool();
      user.m_bAccessShop = query.value(5).toBool();
      user.m_bAccessConsumption = query.value(6).toBool();
      user.m_bAccessUser = query.value(7).toBool();
      user.m_bAccessProduct = query.value(8).toBool();
      user.m_bAccessSettings = query.value(9).toBool();
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
  query.prepare("DELETE FROM " SQL_USER_TABLE_NAME
                " WHERE " SQL_USER_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

UserLoginSQL::UserLoginSQL()
{

}

void UserLoginSQL::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

QSqlDatabase UserLoginSQL::getDatabase() const
{
  return m_db;
}

bool UserLoginSQL::login(const QString& strUser,
                         const QString& strPassword,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(m_db, error))
    return false;

  QSqlQuery query(m_db);
  query.prepare("SELECT "
                SQL_USER_COL00 ","
                SQL_USER_COL01 ","
                SQL_USER_COL02 ","
                SQL_USER_COL03 ","
                SQL_USER_COL04 ","
                SQL_USER_COL05 ","
                SQL_USER_COL06 ","
                SQL_USER_COL07 ","
                SQL_USER_COL08 ","
                SQL_USER_COL09 ","
                SQL_USER_COL10
                " FROM " SQL_USER_TABLE_NAME
                " WHERE " SQL_USER_COL01 " = (:_v01) AND "
                SQL_USER_COL02 " = (:_v02) LIMIT 1");
  query.bindValue(":_v01", strUser);
  query.bindValue(":_v02", User::st_strEncryptedPassword(strPassword));

  if (query.exec())
  {
    bool bFound = false;
    if (query.next())
    {
      m_user.m_id = query.value(0).toInt();
      m_user.m_strUser = query.value(1).toString();
      query.value(2).toString(); // password nao precisamos
      m_user.m_bAccessNote = query.value(3).toBool();
      m_user.m_bAccessReminder = query.value(4).toBool();
      m_user.m_bAccessCalculator = query.value(5).toBool();
      m_user.m_bAccessShop = query.value(6).toBool();
      m_user.m_bAccessConsumption = query.value(7).toBool();
      m_user.m_bAccessUser = query.value(8).toBool();
      m_user.m_bAccessProduct = query.value(9).toBool();
      m_user.m_bAccessSettings = query.value(10).toBool();
      bFound = true;
    }

    if (!bFound)
      error = "Usuário ou senha inválidos.";
    return bFound;
  }

  error = query.lastError().text();
  return false;
}

bool PersonSQL::select(QSqlDatabase db,
                       Person& person,
                       QVector<Phone>& vPhone,
                       QVector<Address>& vAddress,
                       QString& error)
{
  error.clear();
  vPhone.clear();
  vAddress.clear();
  int id = person.m_id;
  person.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                SQL_PERSON_COL01 ","
                SQL_PERSON_COL02 ","
                SQL_PERSON_COL03 ","
                SQL_PERSON_COL04 ","
                SQL_PERSON_COL05 ","
                SQL_PERSON_COL06 ","
                SQL_PERSON_COL07 ","
                SQL_PERSON_COL08 ","
                SQL_PERSON_COL09 ","
                SQL_PERSON_COL10 ","
                SQL_PERSON_COL11 ","
                SQL_PERSON_COL12 ","
                SQL_PERSON_COL13 ","
                " FROM " SQL_PERSON_TABLE_NAME
                " WHERE " SQL_PERSON_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();

  if (bSuccess && query.next())
  {
    person.m_id = id;
    person.m_imageId = query.value(0).toLongLong();
    person.m_name = query.value(1).toString();
    person.m_alias = query.value(2).toString();
    person.m_email = query.value(3).toString();
    person.m_CPF_CNPJ = query.value(4).toString();
    person.m_RG_IE = query.value(5).toString();
    person.m_details = query.value(6).toString();
    person.m_birthDate = query.value(7).toString();
    person.m_creationDate = query.value(8).toString();
    person.m_bCompany = query.value(9).toBool();
    person.m_bCustomer = query.value(10).toBool();
    person.m_bSupplier = query.value(11).toBool();
    person.m_bEmployee = query.value(12).toBool();
    return true;
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_ADDRESS_COL00 ","
                  SQL_ADDRESS_COL01 ","
                  SQL_ADDRESS_COL02 ","
                  SQL_ADDRESS_COL03 ","
                  SQL_ADDRESS_COL04 ","
                  SQL_ADDRESS_COL05 ","
                  SQL_ADDRESS_COL06 ","
                  SQL_ADDRESS_COL07 ","
                  SQL_ADDRESS_COL08 ","
                  SQL_ADDRESS_COL09
                  " FROM " SQL_ADDRESS_TABLE_NAME
                  " WHERE " SQL_ADDRESS_COL01 " = (:_v01)");
    query.bindValue(":_v01", id);
    bSuccess = query.exec();
    while (bSuccess && query.next())
    {
      Address address;
      address.m_id = query.value(0).toLongLong();
      query.value(1).toLongLong(); // personId não usamos
      address.m_cep = query.value(2).toString();
      address.m_neighborhood = query.value(3).toString();
      address.m_street = query.value(4).toString();
      address.m_number = query.value(5).toInt();
      address.m_city = query.value(6).toString();
      address.m_state = (Address::EBRState)query.value(7).toInt();
      address.m_complement = query.value(8).toString();
      address.m_reference = query.value(9).toString();
      vAddress.push_back(address);
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_PHONE_COL00 ","
                  SQL_PHONE_COL01 ","
                  SQL_PHONE_COL02 ","
                  SQL_PHONE_COL03 ","
                  SQL_PHONE_COL04
                  " FROM " SQL_PHONE_TABLE_NAME
                  " WHERE " SQL_PHONE_COL01 " = (:_v01)");
    query.bindValue(":_v01", id);
    bSuccess = query.exec();
    while (bSuccess && query.next())
    {
      Phone phone;
      phone.m_id = query.value(0).toLongLong();
      query.value(1).toLongLong(); //personId não usamos
      phone.m_countryCode = query.value(2).toInt();
      phone.m_code = query.value(3).toInt();
      phone.m_number = query.value(4).toString();
      vPhone.push_back(phone);
    }
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    db.rollback();
    return false;
  }
  else
    bSuccess = db.commit();

  if (!bSuccess)
    error = db.lastError().text();
  else if (person.m_id != id)
  {
    bSuccess = false;
    error = "Pessoa não encontrada.";
  }

  if (!bSuccess)
  {
    person.clear();
    vPhone.clear();
    vAddress.clear();
  }

  return bSuccess;
}

bool PersonSQL::insert(QSqlDatabase db,
                       const Person& person,
                       const QVector<Phone>& vPhone,
                       const QVector<Address>& vAddress,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " SQL_PERSON_TABLE_NAME " ("
                SQL_PERSON_COL02 ","
                SQL_PERSON_COL03 ","
                SQL_PERSON_COL04 ","
                SQL_PERSON_COL05 ","
                SQL_PERSON_COL06 ","
                SQL_PERSON_COL07 ","
                SQL_PERSON_COL08 ","
                SQL_PERSON_COL09 ","
                SQL_PERSON_COL10 ","
                SQL_PERSON_COL11 ","
                SQL_PERSON_COL12 ","
                SQL_PERSON_COL13  ")"
                " VALUES ("
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
                "(:_v13))");
  query.bindValue(":_v02", person.m_name);
  query.bindValue(":_v03", person.m_alias);
  query.bindValue(":_v04", person.m_email);
  query.bindValue(":_v05", person.m_CPF_CNPJ);
  query.bindValue(":_v06", person.m_RG_IE);
  query.bindValue(":_v07", person.m_details);
  query.bindValue(":_v08", person.m_birthDate);
  query.bindValue(":_v09", person.m_creationDate);
  query.bindValue(":_v10", person.m_bCompany);
  query.bindValue(":_v11", person.m_bCustomer);
  query.bindValue(":_v12", person.m_bSupplier);
  query.bindValue(":_v13", person.m_bEmployee);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    person.m_id = query.lastInsertId().toInt();
    if (Image::st_isValidId(person.m_imageId))
    {
      query.prepare("UPDATE " SQL_PERSON_TABLE_NAME " SET "
                    SQL_PERSON_COL01 " = (:_v01)"
                    " WHERE " SQL_PERSON_COL00 " = (:_v00)");
      query.bindValue(":_v00", person.m_id);
      query.bindValue(":_v01", person.m_imageId);
      bSuccess = query.exec();
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != vPhone.size(); ++i)
    {
      query.prepare("INSER INTO " SQL_PHONE_TABLE_NAME " ("
                    SQL_PHONE_COL01 ","
                    SQL_PHONE_COL02 ","
                    SQL_PHONE_COL03 ","
                    SQL_PHONE_COL04 ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04))");
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", vPhone.at(i).m_code);
      query.bindValue(":_v04", vPhone.at(i).m_number);
      bSuccess = query.exec();
      if (bSuccess)
        vPhone.at(i).m_id = query.lastInsertId().toInt();
      else
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != vAddress.size(); ++i)
    {
      query.prepare("INSER INTO " SQL_ADDRESS_TABLE_NAME " ("
                    SQL_ADDRESS_COL01 ","
                    SQL_ADDRESS_COL02 ","
                    SQL_ADDRESS_COL03 ","
                    SQL_ADDRESS_COL04 ","
                    SQL_ADDRESS_COL05 ","
                    SQL_ADDRESS_COL06 ","
                    SQL_ADDRESS_COL07 ","
                    SQL_ADDRESS_COL08 ","
                    SQL_ADDRESS_COL09 ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05),"
                    "(:_v06),"
                    "(:_v07),"
                    "(:_v08),"
                    "(:_v09))");
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", vAddress.at(i).m_cep);
      query.bindValue(":_v03", vAddress.at(i).m_neighborhood);
      query.bindValue(":_v04", vAddress.at(i).m_street);
      query.bindValue(":_v05", vAddress.at(i).m_number);
      query.bindValue(":_v06", vAddress.at(i).m_city);
      query.bindValue(":_v07", (int)vAddress.at(i).m_state);
      query.bindValue(":_v08", vAddress.at(i).m_complement);
      query.bindValue(":_v09", vAddress.at(i).m_reference);
      bSuccess = query.exec();
      if (bSuccess)
        vAddress.at(i).m_id = query.lastInsertId().toInt();
      else
        break;
    }
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    db.rollback();
    return false;
  }
  else
    bSuccess = db.commit();

  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
}

bool PersonSQL::update(QSqlDatabase db,
                       const Person& person,
                       const QVector<Phone>& vPhone,
                       const QVector<Address>& vAddress,
                       const QVector<int>& vRemovedPhoneId,
                       const QVector<int>& vRemovedAddressId,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);

  query.prepare("UPDATE " SQL_PERSON_TABLE_NAME " SET "
                SQL_PERSON_COL01 " = (:_v01),"
                SQL_PERSON_COL02 " = (:_v02),"
                SQL_PERSON_COL03 " = (:_v03),"
                SQL_PERSON_COL04 " = (:_v04),"
                SQL_PERSON_COL05 " = (:_v05),"
                SQL_PERSON_COL06 " = (:_v06),"
                SQL_PERSON_COL07 " = (:_v07),"
                SQL_PERSON_COL08 " = (:_v08),"
                SQL_PERSON_COL09 " = (:_v09),"
                SQL_PERSON_COL10 " = (:_v10),"
                SQL_PERSON_COL11 " = (:_v11),"
                SQL_PERSON_COL12 " = (:_v12),"
                SQL_PERSON_COL13 " = (:_v13)"
                " WHERE " SQL_PERSON_COL00 " = (:_v00)");
  query.bindValue(":_v00", person.m_id);
  query.bindValue(":_v01", person.m_imageId);
  query.bindValue(":_v02", person.m_name);
  query.bindValue(":_v03", person.m_alias);
  query.bindValue(":_v04", person.m_email);
  query.bindValue(":_v05", person.m_CPF_CNPJ);
  query.bindValue(":_v06", person.m_RG_IE);
  query.bindValue(":_v07", person.m_details);
  query.bindValue(":_v08", person.m_birthDate);
  query.bindValue(":_v09", person.m_creationDate);
  query.bindValue(":_v10", person.m_bCompany);
  query.bindValue(":_v11", person.m_bCustomer);
  query.bindValue(":_v12", person.m_bSupplier);
  query.bindValue(":_v13", person.m_bEmployee);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != vPhone.size(); ++i)
    {
      query.prepare("UPDATE " SQL_PHONE_TABLE_NAME " SET "
                    SQL_PHONE_COL01 " = (:_v01),"
                    SQL_PHONE_COL02 " = (:_v02),"
                    SQL_PHONE_COL03 " = (:_v03),"
                    SQL_PHONE_COL04 " = (:_v04) "
                    " WHERE " SQL_PHONE_COL00 " = (:_v00)");
      query.bindValue(":_v00", vPhone.at(i).m_id);
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", vPhone.at(i).m_code);
      query.bindValue(":_v04", vPhone.at(i).m_number);
      bSuccess = query.exec();
      if (!bSuccess)
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != vAddress.size(); ++i)
    {
      query.prepare("UPDATE " SQL_ADDRESS_TABLE_NAME " SET "
                    SQL_ADDRESS_COL01 " = (:_v01),"
                    SQL_ADDRESS_COL02 " = (:_v02),"
                    SQL_ADDRESS_COL03 " = (:_v03),"
                    SQL_ADDRESS_COL04 " = (:_v04),"
                    SQL_ADDRESS_COL05 " = (:_v05),"
                    SQL_ADDRESS_COL06 " = (:_v06),"
                    SQL_ADDRESS_COL07 " = (:_v07),"
                    SQL_ADDRESS_COL08 " = (:_v08),"
                    SQL_ADDRESS_COL09 " = (:_v09) "
                    "WHERE " SQL_ADDRESS_COL00 " = (:_v00)");
      query.bindValue(":_v00", vAddress.at(i).m_id);
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", vAddress.at(i).m_cep);
      query.bindValue(":_v03", vAddress.at(i).m_neighborhood);
      query.bindValue(":_v04", vAddress.at(i).m_street);
      query.bindValue(":_v05", vAddress.at(i).m_number);
      query.bindValue(":_v06", vAddress.at(i).m_city);
      query.bindValue(":_v07", (int)vAddress.at(i).m_state);
      query.bindValue(":_v08", vAddress.at(i).m_complement);
      query.bindValue(":_v09", vAddress.at(i).m_reference);
      bSuccess = query.exec();
      if (!bSuccess)
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != vRemovedPhoneId.size(); ++i)
    {
      query.prepare("DELETE FROM " SQL_PHONE_TABLE_NAME " WHERE " SQL_PHONE_COL00 " = (:_v00)");
      query.bindValue(":_v00", vRemovedPhoneId.at(i));
      bSuccess = query.exec();
      if (!bSuccess)
        break;
    }
  }


  if (bSuccess)
  {
    for (int i = 0; i != vRemovedAddressId.size(); ++i)
    {
      query.prepare("DELETE FROM " SQL_ADDRESS_TABLE_NAME " WHERE " SQL_ADDRESS_COL00 " = (:_v00)");
      query.bindValue(":_v00", vRemovedAddressId.at(i));
      bSuccess = query.exec();
      if (!bSuccess)
        break;
    }
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    db.rollback();
    return false;
  }
  else
    bSuccess = db.commit();

  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
}

bool PersonSQL::remove(QSqlDatabase db,
                       int id,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " SQL_PHONE_TABLE_NAME " WHERE " SQL_PHONE_COL01 " = (:_v01)");
  query.bindValue(":_v01", id);
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    query.prepare("DELETE FROM " SQL_ADDRESS_TABLE_NAME " WHERE " SQL_PHONE_COL01 " = (:_v01)");
    query.bindValue(":_v01", id);
    bSuccess = query.exec();
  }
  if (bSuccess)
  {
    query.prepare("DELETE FROM " SQL_PERSON_TABLE_NAME " WHERE " SQL_PERSON_COL00 " = (:_v00)");
    query.bindValue(":_v00", id);
    bSuccess = query.exec();
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    db.rollback();
    return false;
  }
  else
    bSuccess = db.commit();

  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
}
