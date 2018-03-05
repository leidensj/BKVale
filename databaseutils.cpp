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

  query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NUMBER INTEGER NOT NULL,"
             "_DATE INTEGER NOT NULL,"
             "_SUPPLIER TEXT NOT NULL,"
             "_TOTAL REAL,"
             "_CASH INT)");

  query.exec("CREATE TABLE IF NOT EXISTS _PROMISSORYNOTESITEMS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NOTEID INTEGER NOT NULL,"
             "_AMMOUNT REAL,"
             "_PRICE REAL,"
             "_UNITY TEXT,"
             "_DESCRIPTION TEXT)");

  query.exec("CREATE TABLE IF NOT EXISTS _PRODUCTS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NAME TEXT NOT NULL UNIQUE,"
             "FOREIGN KEY(_CATEGORYID) REFERENCES _CATEGORIES(_ID),"
             "FOREIGN KEY(_IMAGEID) REFERENCES _IMAGES(_ID),"
             "_UNITY TEXT NOT NULL,"
             "_PACKAGE_UNITY TEXT,"
             "_PACKAGE_AMMOUNT REAL,"
             "_DETAILS TEXT,"
             "_CODE TEXT,"
             "_AVAILABLE_AT_NOTES,"
             "_AVAILABLE_AT_SHOP,"
             "_AVAILABLE_AT_CONSUMPTION,"
             "_AVAILABLE_TO_BUY,"
             "_AVAILABLE_TO_SELL)");

  query.exec("CREATE TABLE IF NOT EXISTS _CATEGORIES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NAME TEXT NOT NULL UNIQUE,"
             "FOREIGN KEY(_IMAGEID) REFERENCES _IMAGES(_ID))");

  query.exec("CREATE TABLE IF NOT EXISTS _IMAGES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "_NAME TEXT NOT NULL UNIQUE,"
             "_IMAGE BLOB)");

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
             "_PRODUCTID INTEGER,"
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
             "_ACCESS_PRODUCT INT,"
             "_ACCESS_SETTINGS INT)");

  query.exec("CREATE TABLE IF NOT EXISTS _PERSONS ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "FOREIGN KEY(_IMAGEID) REFERENCES _IMAGES(_ID),"
             "_NAME TEXT NOT NULL UNIQUE,"
             "_ALIAS TEXT,"
             "_EMAIL TEXT,"
             "_CPF_CNPJ TEXT,"
             "_RG_IE TEXT,"
             "_DETAILS TEXT,"
             "_BIRTH_DATE INTEGER,"
             "_CREATION_DATE INTEGER,"
             "_IS_COMPANY INT,"
             "_IS_CUSTOMER INT,"
             "_IS_SUPPLIER INT,"
             "_IS_EMPLOYEE INT)");

  query.exec("CREATE TABLE IF NOT EXISTS _ADDRESSES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "FOREIGN KEY(_PERSONID) REFERENCES _PERSONS(_ID),"
             "_CEP TEXT,"
             "_NEIGHBORHOOD TEXT,"
             "_STREET TEXT,"
             "_NUMBER INT,"
             "_CITY TEXT,"
             "_STATE INT,"
             "_COMPLEMENT TEXT,"
             "_REFERENCE TEXT)");

  query.exec("CREATE TABLE IF NOT EXISTS _PHONES ("
             "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
             "FOREIGN KEY(_PERSONID) REFERENCES _PERSONS(_ID),"
             "_COUNTRY_CODE INT DEFAULT " DEFAULT_PHONE_COUNTRY_CODE_VALUE_STR ","
             "_CODE INT DEFAULT " DEFAULT_PHONE_CODE_VALUE_STR ","
             "_NUMBER TEXT)");

  query.exec("SELECT * FROM _USERS LIMIT 1");
  if (!query.next())
  {
    query.prepare("INSERT INTO _USERS ("
                  "_USER,"
                  "_PASSWORD,"
                  "_ACCESS_NOTE,"
                  "_ACCESS_REMINDER,"
                  "_ACCESS_CALCULATOR,"
                  "_ACCESS_SHOP,"
                  "_ACCESS_CONSUMPTION,"
                  "_ACCESS_USER,"
                  "_ACCESS_PRODUCT,"
                  "_ACCESS_SETTINGS) "
                  "VALUES ("
                  "'ADMIN',"
                  "(:_password),"
                  "1,1,1,1,1,1,1,1)");
    query.bindValue(":_password", User::st_strEncryptedPassword("admin"));
    query.exec();
  }

  bool bSuccess = db.commit();
  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
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
                "_NAME,"
                "_CATEGORYID,"
                "_IMAGEID,"
                "_UNITY,"
                "_PACKAGE_UNITY,"
                "_PACKAGE_AMMOUNT,"
                "_DETAILS,"
                "_CODE,"
                "_AVAILABLE_AT_NOTES,"
                "_AVAILABLE_AT_SHOP,"
                "_AVAILABLE_AT_CONSUMPTION,"
                "_AVAILABLE_TO_BUY,"
                "_AVAILABLE_TO_SELL "
                "FROM _PRODUCTS "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  if (query.exec())
  {
    if (query.next())
    {
      product.m_id = id;
      product.m_name = query.value(query.record().indexOf("_NAME")).toString();
      product.m_categoryId = query.value(query.record().indexOf("_CATEGORYID")).toInt();
      product.m_imageId = query.value(query.record().indexOf("_IMAGEID")).toInt();
      product.m_unity = query.value(query.record().indexOf("_UNITY")).toString();
      product.m_packageUnity = query.value(query.record().indexOf("_PACKAGE_UNITY")).toString();
      product.m_packageAmmount = query.value(query.record().indexOf("_PACKAGE_AMMOUNT")).toDouble();
      product.m_details = query.value(query.record().indexOf("_DETAILS")).toString();
      product.m_code = query.value(query.record().indexOf("_CODE")).toString();
      product.m_bAvailableAtNotes = query.value(query.record().indexOf("_AVAILABLE_AT_NOTES")).toBool();
      product.m_bAvailableAtShop = query.value(query.record().indexOf("_AVAILABLE_AT_SHOP")).toBool();
      product.m_bAvailableAtConsumption = query.value(query.record().indexOf("_AVAILABLE_AT_CONSUMPTION")).toBool();
      product.m_bAvailableToBuy = query.value(query.record().indexOf("_AVAILABLE_TO_BUY")).toBool();
      product.m_bAvailableToSell = query.value(query.record().indexOf("_AVAILABLE_TO_SELL")).toBool();
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
  query.prepare("INSERT INTO _PRODUCTS ("
                "_NAME,"
                "_CATEGORYID,"
                "_IMAGEID,"
                "_UNITY,"
                "_PACKAGE_UNITY,"
                "_PACKAGE_AMMOUNT,"
                "_DETAILS,"
                "_CODE,"
                "_AVAILABLE_AT_NOTES,"
                "_AVAILABLE_AT_SHOP,"
                "_AVAILABLE_AT_CONSUMPTION,"
                "_AVAILABLE_TO_BUY,"
                "_AVAILABLE_TO_SELL) "
                "VALUES ("
                "(:_name),"
                "(:_categoryid),"
                "(:_imageid),"
                "(:_unity),"
                "(:_packageunity),"
                "(:_packageammount),"
                "(:_details),"
                "(:_code),"
                "(:_availableatnotes),"
                "(:_availableatshop),"
                "(:_availableatconsumption),"
                "(:_availabletobuy),"
                "(:_availabletosell))");
  query.bindValue(":_name", product.m_name);
  query.bindValue(":_categoryid", product.m_categoryId);
  query.bindValue(":_imageid", product.m_imageId);
  query.bindValue(":_unity", product.m_unity);
  query.bindValue(":_packageunity", product.m_packageUnity);
  query.bindValue(":_packageammount", product.m_packageAmmount);
  query.bindValue(":_details", product.m_details);
  query.bindValue(":_code", product.m_code);
  query.bindValue(":_availableatnotes", product.m_bAvailableAtNotes);
  query.bindValue(":_availableatshop", product.m_bAvailableAtShop);
  query.bindValue(":_availableatconsumption", product.m_bAvailableAtConsumption);
  query.bindValue(":_availabletobuy", product.m_bAvailableToBuy);
  query.bindValue(":_availabletosell", product.m_bAvailableToSell);

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
  query.prepare("UPDATE _PRODUCTS SET "
                "_NAME = (:_name),"
                "_CATEGORYID = (:_categoryid),"
                "_IMAGEID = (:_imageid),"
                "_UNITY = (:_unity),"
                "_PACKAGE_UNITY = (:_packageunity),"
                "_PACKAGE_AMMOUNT = (:_packageammount),"
                "_DETAILS = (:_details),"
                "_CODE = (:_code),"
                "_AVAILABLE_AT_NOTES = (:_availableatnotes),"
                "_AVAILABLE_AT_SHOP = (:_availableatshop),"
                "_AVAILABLE_AT_CONSUMPTION = (:_availableatconsumption),"
                "_AVAILABLE_TO_BUY = (:_availabletobuy),"
                "_AVAILABLE_TO_SELL = (:_availabletosell) "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", product.m_id);
  query.bindValue(":_name", product.m_name);
  query.bindValue(":_categoryid", product.m_categoryId);
  query.bindValue(":_imageid", product.m_imageId);
  query.bindValue(":_unity", product.m_unity);
  query.bindValue(":_packageunity", product.m_packageUnity);
  query.bindValue(":_packageammount", product.m_packageAmmount);
  query.bindValue(":_details", product.m_details);
  query.bindValue(":_code", product.m_code);
  query.bindValue(":_availableatnotes", product.m_bAvailableAtNotes);
  query.bindValue(":_availableatshop", product.m_bAvailableAtShop);
  query.bindValue(":_availableatconsumption", product.m_bAvailableAtConsumption);
  query.bindValue(":_availabletobuy", product.m_bAvailableToBuy);
  query.bindValue(":_availabletosell", product.m_bAvailableToSell);

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
  query.prepare("DELETE FROM _PRODUCTS "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);

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
                "_NAME,"
                "_IMAGEID "
                "FROM _CATEGORIES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);

  if (query.exec())
  {
    if (query.next())
    {
      category.m_id = id;
      category.m_name = query.value(query.record().indexOf("_NAME")).toString();
      category.m_imageId = query.value(query.record().indexOf("_IMAGEID")).toInt();
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
  query.prepare("INSERT INTO _CATEGORIES ("
                "_NAME,"
                "_IMAGEID) "
                "VALUES ("
                "(:_name),"
                "(:_imageid))");
  query.bindValue(":_name", category.m_name);
  query.bindValue(":_imageid", category.m_imageId);

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
  query.prepare("UPDATE _CATEGORIES SET "
                "_NAME = (:_name),"
                "_IMAGEID = (:_imageid) "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", category.m_id);
  query.bindValue(":_name", category.m_name);
  query.bindValue(":_imageid", category.m_imageId);

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
  query.prepare("DELETE FROM _CATEGORIES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);

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
                "_NAME,"
                "_IMAGE "
                "FROM _IMAGES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);

  if (query.exec())
  {
    if (query.next())
    {
      image.m_id = id;
      image.m_name = query.value(query.record().indexOf("_NAME")).toString();
      image.m_image = query.value(query.record().indexOf("_IMAGE")).toByteArray();
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
  query.prepare("INSERT INTO _IMAGES ("
                "_NAME,"
                "_IMAGE) "
                "VALUES ("
                "(:_name),"
                "(:_image))");
  query.bindValue(":_name", image.m_name);
  query.bindValue(":_image", image.m_image);

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
  query.prepare("UPDATE _IMAGES SET "
                "_NAME = (:_name),"
                "_IMAGE = (:_image) "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", image.m_id);
  query.bindValue(":_name", image.m_name);
  query.bindValue(":_image", image.m_image);

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
  query.prepare("DELETE FROM _IMAGES "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);

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
  query.prepare("INSERT INTO _USERS ("
                "_USER,"
                "_PASSWORD,"
                "_ACCESS_NOTE,"
                "_ACCESS_REMINDER,"
                "_ACCESS_CALCULATOR,"
                "_ACCESS_SHOP,"
                "_ACCESS_CONSUMPTION,"
                "_ACCESS_USER,"
                "_ACCESS_PRODUCT,"
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
                "(:_accessProduct),"
                "(:_accessSettings))");
  query.bindValue(":_user", user.m_strUser);
  query.bindValue(":_password", user.st_strEncryptedPassword(strPassword));
  query.bindValue(":_accessNote", user.m_bAccessNote);
  query.bindValue(":_accessReminder", user.m_bAccessReminder);
  query.bindValue(":_accessCalculator", user.m_bAccessCalculator);
  query.bindValue(":_accessShop", user.m_bAccessShop);
  query.bindValue(":_accessConsumption", user.m_bAccessConsumption);
  query.bindValue(":_accessUser", user.m_bAccessUser);
  query.bindValue(":_accessProduct", user.m_bAccessProduct);
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
              "_ACCESS_PRODUCT = (:_accessProduct),"
              "_ACCESS_SETTINGS = (:_accessSettings) "
              "WHERE _ID = (:_id)";


  QSqlQuery query(db);
  query.prepare(strQuery);
  query.bindValue(":_id", user.m_id);
  query.bindValue(":_user", user.m_strUser);
  if (!strPassword.isEmpty())
    query.bindValue(":_password", User::st_strEncryptedPassword(strPassword));
  query.bindValue(":_accessNote", user.m_bAccessNote);
  query.bindValue(":_accessReminder", user.m_bAccessReminder);
  query.bindValue(":_accessCalculator", user.m_bAccessCalculator);
  query.bindValue(":_accessShop", user.m_bAccessShop);
  query.bindValue(":_accessConsumption", user.m_bAccessConsumption);
  query.bindValue(":_accessUser", user.m_bAccessUser);
  query.bindValue(":_accessProduct", user.m_bAccessProduct);
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
                "_ACCESS_PRODUCT,"
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
      user.m_bAccessReminder = query.value(query.record().indexOf("_ACCESS_REMINDER")).toBool();
      user.m_bAccessCalculator = query.value(query.record().indexOf("_ACCESS_CALCULATOR")).toBool();
      user.m_bAccessShop = query.value(query.record().indexOf("_ACCESS_SHOP")).toBool();
      user.m_bAccessConsumption = query.value(query.record().indexOf("_ACCESS_CONSUMPTION")).toBool();
      user.m_bAccessUser = query.value(query.record().indexOf("_ACCESS_USER")).toBool();
      user.m_bAccessProduct = query.value(query.record().indexOf("_ACCESS_PRODUCT")).toBool();
      user.m_bAccessSettings = query.value(query.record().indexOf("_ACCESS_SETTINGS")).toBool();
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
  query.prepare("SELECT * FROM _USERS "
                "WHERE _USER = (:_user) AND "
                "_PASSWORD = (:_password) LIMIT 1");
  query.bindValue(":_user", strUser);
  query.bindValue(":_password", User::st_strEncryptedPassword(strPassword));

  if (query.exec())
  {
    bool bFound = false;
    if (query.next())
    {
      m_user.m_id = query.value(query.record().indexOf("_ID")).toInt();
      m_user.m_strUser = query.value(query.record().indexOf("_USER")).toString();
      m_user.m_bAccessNote = query.value(query.record().indexOf("_ACCESS_NOTE")).toBool();
      m_user.m_bAccessReminder = query.value(query.record().indexOf("_ACCESS_REMINDER")).toBool();
      m_user.m_bAccessCalculator = query.value(query.record().indexOf("_ACCESS_CALCULATOR")).toBool();
      m_user.m_bAccessShop = query.value(query.record().indexOf("_ACCESS_SHOP")).toBool();
      m_user.m_bAccessConsumption = query.value(query.record().indexOf("_ACCESS_CONSUMPTION")).toBool();
      m_user.m_bAccessUser = query.value(query.record().indexOf("_ACCESS_USER")).toBool();
      m_user.m_bAccessProduct = query.value(query.record().indexOf("_ACCESS_PRODUCT")).toBool();
      m_user.m_bAccessSettings = query.value(query.record().indexOf("_ACCESS_SETTINGS")).toBool();
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
  query.prepare("SELECT * "
                "FROM _PERSONS "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", id);

  query.exec();
  {
    if (query.next())
    {
      person.m_id = id;
      person.m_imageId = query.value(query.record().indexOf("_IMAGEID")).toLongLong();
      person.m_name = query.value(query.record().indexOf("_NAME")).toString();
      person.m_alias = query.value(query.record().indexOf("_ALIAS")).toString();
      person.m_email = query.value(query.record().indexOf("_EMAIL")).toString();
      person.m_CPF_CNPJ = query.value(query.record().indexOf("_CPF_CNPJ")).toString();
      person.m_RG_IE = query.value(query.record().indexOf("_RG_IE")).toString();
      person.m_details = query.value(query.record().indexOf("_DETAILS")).toString();
      person.m_birthDate = query.value(query.record().indexOf("_BIRTH_DATE")).toLongLong();
      person.m_creationDate = query.value(query.record().indexOf("_CREATION_DATE")).toLongLong();
      person.m_bCompany = query.value(query.record().indexOf("_IS_COMPANY")).toBool();
      person.m_bCustomer = query.value(query.record().indexOf("_IS_CUSTOMER")).toBool();
      person.m_bSupplier = query.value(query.record().indexOf("_IS_SUPPLIER")).toBool();
      person.m_bEmployee = query.value(query.record().indexOf("_IS_EMPLOYEE")).toBool();
      return true;
    }
  }

  query.prepare("SELECT * "
                "FROM _ADDRESSES "
                "WHERE _PERSONID = (:_personid)");
  query.bindValue(":_personid", id);
  query.exec();
  while (query.next())
  {
    Address address;
    address.m_id = query.value(query.record().indexOf("_ID")).toLongLong();
    address.m_cep = query.value(query.record().indexOf("_CEP")).toString();
    address.m_neighborhood = query.value(query.record().indexOf("_NEIGHBORHOOD")).toString();
    address.m_street = query.value(query.record().indexOf("_STREET")).toString();
    address.m_number = query.value(query.record().indexOf("_NUMBER")).toInt();
    address.m_city = query.value(query.record().indexOf("_CITY")).toString();
    address.m_state = (Address::EBRState)query.value(query.record().indexOf("_STATE")).toInt();
    address.m_complement = query.value(query.record().indexOf("_COMPLEMENT")).toString();
    address.m_reference = query.value(query.record().indexOf("_REFERENCE")).toString();
    vAddress.push_back(address);
  }

  query.prepare("SELECT * "
                "FROM _PHONES "
                "WHERE _PERSONID = (:_personid)");
  query.bindValue(":_personid", id);
  query.exec();
  while (query.next())
  {
    Phone phone;
    phone.m_id = query.value(query.record().indexOf("_ID")).toLongLong();
    phone.m_countryCode = query.value(query.record().indexOf("_COUNTRY_CODE")).toInt();
    phone.m_code = query.value(query.record().indexOf("_CODE")).toInt();
    phone.m_number = query.value(query.record().indexOf("_NUMBER")).toString();
    vPhone.push_back(phone);
  }

  bool bSuccess = db.commit();
  if (!bSuccess)
    error = query.lastError().text();
  bSuccess = person.m_id == id;
  if (!bSuccess)
    error = "Pessoa não encontrada.";

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
  query.prepare("INSERT INTO _PERSONS ("
                "_IMAGEID,"
                "_NAME,"
                "_ALIAS,"
                "_EMAIL,"
                "_CPF_CNPJ,"
                "_RG_IE,"
                "_BIRTH_DATE,"
                "_CREATION_DATE,"
                "_IS_COMPANY,"
                "_IS_CUSTOMER,"
                "_IS_SUPPLIER,"
                "_IS_EMPLOYEE)"
                "VALUES ("
                "(:_imageid),"
                "(:_name),"
                "(:_alias),"
                "(:_email),"
                "(:_cpfcnpj),"
                "(:_rgie),"
                "(:_birthdate),"
                "(:_creationdate),"
                "(:_iscompany),"
                "(:_iscustomer),"
                "(:_issupplier),"
                "(:_isemployee))");
  query.bindValue(":_imageid", person.m_imageId);
  query.bindValue(":_name", person.m_name);
  query.bindValue(":_alias", person.m_alias);
  query.bindValue(":_email", person.m_email);
  query.bindValue(":_cpfcnpj", person.m_CPF_CNPJ);
  query.bindValue(":_rgie", person.m_RG_IE);
  query.bindValue(":_birthdate", person.m_birthDate);
  query.bindValue(":_creationdate", person.m_creationDate);
  query.bindValue(":_iscompany", person.m_bCompany);
  query.bindValue(":_iscustomer", person.m_bCustomer);
  query.bindValue(":_issupplier", person.m_bSupplier);
  query.bindValue(":_isemployee", person.m_bEmployee);

  if (query.exec())
  {
    person.m_id = query.lastInsertId().toInt();
    for (int i = 0; i != vPhone.size(); ++i)
    {
      query.prepare("INSER INTO _PHONES ("
                    "_PERSONID,"
                    "_COUNTRY_CODE,"
                    "_CODE,"
                    "_NUMBER)"
                    "VALUES ("
                    "(:_personid),"
                    "(:_countrycode),"
                    "(:_code),"
                    "(:_number))");
      query.bindValue(":_personid", person.m_id);
      query.bindValue(":_countrycode", vPhone.at(i).m_countryCode);
      query.bindValue(":_code", vPhone.at(i).m_code);
      query.bindValue(":_number", vPhone.at(i).m_number);
      if (query.exec())
        vPhone.at(i).m_id = query.lastInsertId().toInt();
    }

    for (int i = 0; i != vAddress.size(); ++i)
    {
      query.prepare("INSER INTO _ADDRESSES ("
                    "_PERSONID,"
                    "_CEP,"
                    "_NEIGHBORHOOD,"
                    "_STREET,"
                    "_NUMBER,"
                    "_CITY,"
                    "_STATE,"
                    "_COMPLEMENT,"
                    "_REFERENCE)"
                    "VALUES ("
                    "(:_personid),"
                    "(:_cep),"
                    "(:_neighborhood),"
                    "(:_street),"
                    "(:_number),"
                    "(:_city),"
                    "(:_state),"
                    "(:_complement),"
                    "(:_reference))");
      query.bindValue(":_personid", person.m_id);
      query.bindValue(":_cep", vAddress.at(i).m_cep);
      query.bindValue(":_neighborhood", vAddress.at(i).m_neighborhood);
      query.bindValue(":_street", vAddress.at(i).m_street);
      query.bindValue(":_number", vAddress.at(i).m_number);
      query.bindValue(":_city", vAddress.at(i).m_city);
      query.bindValue(":_state", (int)vAddress.at(i).m_state);
      query.bindValue(":_complement", vAddress.at(i).m_complement);
      query.bindValue(":_reference", vAddress.at(i).m_reference);
      if (query.exec())
        vAddress.at(i).m_id = query.lastInsertId().toInt();
    }
  }

  bool bSuccess = db.commit();
  if (bSuccess)
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

  query.prepare("UPDATE _PERSONS SET "
                "_IMAGEID = (:_imageid),"
                "_NAME = (:_name),"
                "_ALIAS = (:_alias),"
                "_EMAIL = (:_email),"
                "_CPF_CNPJ = (:_cpfcnpj),"
                "_RG_IE = (:_rgie),"
                "_BIRTH_DATE = (:_birthdate),"
                "_CREATION_DATE = (:_creationdate),"
                "_IS_COMPANY = (:_iscompany),"
                "_IS_CUSTOMER = (:_iscustomer),"
                "_IS_SUPPLIER = (:_issupplier),"
                "_IS_EMPLOYEE = (:_isemployee) "
                "WHERE _ID = (:_id)");
  query.bindValue(":_id", person.m_id);
  query.bindValue(":_imageid", person.m_imageId);
  query.bindValue(":_name", person.m_name);
  query.bindValue(":_alias", person.m_alias);
  query.bindValue(":_email", person.m_email);
  query.bindValue(":_cpfcnpj", person.m_CPF_CNPJ);
  query.bindValue(":_rgie", person.m_RG_IE);
  query.bindValue(":_birthdate", person.m_birthDate);
  query.bindValue(":_creationdate", person.m_creationDate);
  query.bindValue(":_iscompany", person.m_bCompany);
  query.bindValue(":_iscustomer", person.m_bCustomer);
  query.bindValue(":_issupplier", person.m_bSupplier);
  query.bindValue(":_isemployee", person.m_bEmployee);
  query.exec();

  for (int i = 0; i != vPhone.size(); ++i)
  {
    query.prepare("UPDATE _PHONES SET "
                  "_PERSONID = (:_personid),"
                  "_COUNTRY_CODE = (:_countrycode),"
                  "_CODE = (:_code),"
                  "_NUMBER = (:_number) "
                  "WHERE _ID = (:_id)");
    query.bindValue(":_id", vPhone.at(i).m_id);
    query.bindValue(":_personid", person.m_id);
    query.bindValue(":_countrycode", vPhone.at(i).m_countryCode);
    query.bindValue(":_code", vPhone.at(i).m_code);
    query.bindValue(":_number", vPhone.at(i).m_number);
    query.exec();
  }

  for (int i = 0; i != vAddress.size(); ++i)
  {
    query.prepare("UPDATE _ADDRESSES SET "
                  "_PERSONID = (:_personid),"
                  "_CEP = (:_cep),"
                  "_NEIGHBORHOOD = (:_neighborhood),"
                  "_STREET = (:_street),"
                  "_NUMBER = (:_number),"
                  "_CITY = (:_city),"
                  "_STATE = (:_state),"
                  "_COMPLEMENT = (:_complement),"
                  "_REFERENCE = (:_reference) "
                  "WHERE _ID = (:_id)");
    query.bindValue(":_id", vAddress.at(i).m_id);
    query.bindValue(":_personid", person.m_id);
    query.bindValue(":_cep", vAddress.at(i).m_cep);
    query.bindValue(":_neighborhood", vAddress.at(i).m_neighborhood);
    query.bindValue(":_street", vAddress.at(i).m_street);
    query.bindValue(":_number", vAddress.at(i).m_number);
    query.bindValue(":_city", vAddress.at(i).m_city);
    query.bindValue(":_state", (int)vAddress.at(i).m_state);
    query.bindValue(":_complement", vAddress.at(i).m_complement);
    query.bindValue(":_reference", vAddress.at(i).m_reference);
    query.exec();
  }

  for (int i = 0; i != vRemovedPhoneId.size(); ++i)
  {
    query.prepare("DELETE FROM _PHONES WHERE _ID = (:_id)");
    query.bindValue(":_id", vRemovedPhoneId.at(i));
    query.exec();
  }

  for (int i = 0; i != vRemovedAddressId.size(); ++i)
  {
    query.prepare("DELETE FROM _ADDRESSES WHERE _ID = (:_id)");
    query.bindValue(":_id", vRemovedAddressId.at(i));
    query.exec();
  }

  bool bSuccess = db.commit();
  if (bSuccess)
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
  query.prepare("DELETE FROM _PHONES WHERE _PERSONID = (:_personid)");
  query.bindValue(":_personid", id);
  query.exec();
  query.prepare("DELETE FROM _ADDRESSES WHERE _PERSONID = (:_personid)");
  query.bindValue(":_personid", id);
  query.exec();
  query.prepare("DELETE FROM _PERSONS WHERE _ID = (:_id)");
  query.bindValue(":_id", id);
  query.exec();

  bool bSuccess = db.commit();
  if (bSuccess)
    error = db.lastError().text();
  return bSuccess;
}
