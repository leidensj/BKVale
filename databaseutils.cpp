#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

#define DATABASE_NOT_OPEN_TXT "O banco de dados não foi aberto."

namespace {
  bool finishTransaction(QSqlDatabase db,
                         bool bExecSelectResult,
                         QString& error)
  {
    if (!bExecSelectResult)
    {
      db.rollback();
      return false;
    }
    else
      bExecSelectResult = db.commit();

    if (!bExecSelectResult)
      error = db.lastError().text();

    return bExecSelectResult;
  }
}
qlonglong NoteSQL::nextNumber(QSqlDatabase db)
{
  qlonglong number = SQL_NOTE_DEFAULT_NUMBER;
  if (!db.isOpen())
    return number;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = query.exec("SELECT MAX(" SQL_NOTE_COL01 ") FROM " SQL_NOTE_TABLE_NAME);
  if (bSuccess)
    number = query.next() ? query.value(0).toLongLong() + 1 : SQL_NOTE_DEFAULT_NUMBER;
  number = number > SQL_NOTE_DEFAULT_NUMBER ? number : SQL_NOTE_DEFAULT_NUMBER;

  if (!bSuccess)
    db.rollback();
  else
    db.commit();

  return number;
}

bool NoteSQL::insert(QSqlDatabase db,
                     const Note& note,
                     const QVector<NoteItem>& vItems,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = query.exec("SELECT MAX(" SQL_NOTE_COL01 ") FROM " SQL_NOTE_TABLE_NAME);
  if (bSuccess)
  {
      qlonglong number = query.next()
                         ? query.value(0).toLongLong() + 1
                         : SQL_NOTE_DEFAULT_NUMBER;
      number = number > SQL_NOTE_DEFAULT_NUMBER ? number : SQL_NOTE_DEFAULT_NUMBER;

      query.prepare("INSERT INTO " SQL_NOTE_TABLE_NAME " ("
                    SQL_NOTE_COL01 ","
                    SQL_NOTE_COL02 ","
                    SQL_NOTE_COL03 ","
                    SQL_NOTE_COL04 ","
                    SQL_NOTE_COL05 ")"
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05))");
      query.bindValue(":_v01", number);
      query.bindValue(":_v02", note.m_date);
      query.bindValue(":_v03", note.m_supplierId);
      query.bindValue(":_v04", note.m_total);
      query.bindValue(":_v05", note.m_bCash);
      bSuccess = query.exec();
  }

  if (bSuccess)
  {
    note.m_id = query.lastInsertId().toLongLong();
    for (int i = 0; i != vItems.size(); ++i)
    {
      query.prepare("INSERT INTO " SQL_NOTE_ITEMS_TABLE_NAME " ("
                    SQL_NOTE_ITEMS_COL01 ","
                    SQL_NOTE_ITEMS_COL02 ","
                    SQL_NOTE_ITEMS_COL03 ","
                    SQL_NOTE_ITEMS_COL04 ","
                    SQL_NOTE_ITEMS_COL05
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05))");
      query.bindValue(":_v01", note.m_id);
      query.bindValue(":_v02", vItems.at(i).m_productId);
      query.bindValue(":_v03", vItems.at(i).m_ammount);
      query.bindValue(":_v04", vItems.at(i).m_price);
      query.bindValue(":_v05", vItems.at(i).m_bIsPackageAmmount);
      bSuccess = query.exec();
      if (bSuccess)
        vItems.at(i).m_id = query.lastInsertId().toLongLong();
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

bool NoteSQL::update(QSqlDatabase db,
                     const Note& note,
                     const QVector<NoteItem>& vItems,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " SQL_NOTE_TABLE_NAME " SET "
                SQL_NOTE_COL02 " = (:_v02),"
                SQL_NOTE_COL03 " = (:_v03),"
                SQL_NOTE_COL04 " = (:_v04),"
                SQL_NOTE_COL05 " = (:_v05) "
                "WHERE " SQL_NOTE_COL00 " = (:_v00)");
  query.bindValue(":_v00", note.m_id);
  query.bindValue(":_v02", note.m_date);
  query.bindValue(":_v03", note.m_supplierId);
  query.bindValue(":_v04", note.m_total);
  query.bindValue(":_v05", note.m_bCash);
  bool bSuccess = query.exec();

  if (bSuccess && !vItems.isEmpty())
  {
    QString str = "DELETE FROM " SQL_NOTE_ITEMS_TABLE_NAME
                  " WHERE " SQL_NOTE_ITEMS_COL01 " = (:_v01)"
                  " AND " SQL_NOTE_ITEMS_COL00 " NOT IN (";
    for (int i = 0; i != vItems.size(); ++i)
      str += "(:_v00" + QString::number(i) + "),";
    str.replace(str.length() - 1, 1, ")");
    query.prepare(str);
    query.bindValue(":_v01", note.m_id);
    for (int i = 0; i != vItems.size(); ++i)
      query.bindValue("_v00" + QString::number(i), vItems.at(i).m_id);
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != vItems.size(); ++i)
    {
      if (vItems.at(i).isValidID())
      {
        query.prepare("UPDATE " SQL_NOTE_ITEMS_TABLE_NAME " SET "
                      SQL_NOTE_ITEMS_COL01 " = (:_v01),"
                      SQL_NOTE_ITEMS_COL02 " = (:_v02),"
                      SQL_NOTE_ITEMS_COL03 " = (:_v03),"
                      SQL_NOTE_ITEMS_COL04 " = (:_v04),"
                      SQL_NOTE_ITEMS_COL05 " = (:_v05)"
                      " WHERE _ID = (:_v00)");
        query.bindValue(":_v00", vItems.at(i).m_id);
        query.bindValue(":_v01", note.m_id);
        query.bindValue(":_v02", vItems.at(i).m_productId);
        query.bindValue(":_v03", vItems.at(i).m_ammount);
        query.bindValue(":_v04", vItems.at(i).m_price);
        query.bindValue(":_v05", vItems.at(i).m_bIsPackageAmmount);
        bSuccess = query.exec();
      }
      else
      {
        query.prepare("INSERT INTO " SQL_NOTE_ITEMS_TABLE_NAME " ("
                      SQL_NOTE_ITEMS_COL01 ","
                      SQL_NOTE_ITEMS_COL02 ","
                      SQL_NOTE_ITEMS_COL03 ","
                      SQL_NOTE_ITEMS_COL04 ","
                      SQL_NOTE_ITEMS_COL05
                      " ) VALUES ("
                      "(:_v01),"
                      "(:_v02),"
                      "(:_v03),"
                      "(:_v04),"
                      "(:_v05))");
        query.bindValue(":_v01", note.m_id);
        query.bindValue(":_v02", vItems.at(i).m_productId);
        query.bindValue(":_v03", vItems.at(i).m_ammount);
        query.bindValue(":_v04", vItems.at(i).m_price);
        query.bindValue(":_v05", vItems.at(i).m_bIsPackageAmmount);
        bSuccess = query.exec();
        if (bSuccess)
          vItems.at(i).m_id = query.lastInsertId().toLongLong();
      }
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

bool NoteSQL::select(QSqlDatabase db,
                     FullNote& fNote,
                     QString& error)
{
  error.clear();
  qlonglong id = fNote.m_note.m_id;
  fNote.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                SQL_NOTE_COL01 ","
                SQL_NOTE_COL02 ","
                SQL_NOTE_COL03 ","
                SQL_NOTE_COL04 ","
                SQL_NOTE_COL05
                " FROM " SQL_NOTE_TABLE_NAME
                " WHERE " SQL_NOTE_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      fNote.m_note.m_id = id;
      fNote.m_number = query.value(0).toLongLong();
      fNote.m_note.m_date = query.value(1).toString();
      fNote.m_note.m_supplierId = query.value(2).toLongLong();
      fNote.m_note.m_total = query.value(3).toDouble();
      fNote.m_note.m_bCash = query.value(4).toBool();
    }
    else
    {
      error = "Vale não encontrado.";
      bSuccess = false;
    }

  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_NOTE_ITEMS_COL00 ","
                  SQL_NOTE_ITEMS_COL02 ","
                  SQL_NOTE_ITEMS_COL03 ","
                  SQL_NOTE_ITEMS_COL04 ","
                  SQL_NOTE_ITEMS_COL05
                  " FROM " SQL_NOTE_ITEMS_TABLE_NAME
                  " WHERE " SQL_NOTE_COL01 " = (:_v01)");
    query.bindValue(":_v01", fNote.m_note.m_id);
    bSuccess = query.exec();
    while (bSuccess && query.next())
    {
      FullNoteItem fNoteItem;
      fNoteItem.m_item.m_id = query.value(0).toLongLong();
      fNoteItem.m_item.m_productId = query.value(1).toLongLong();
      fNoteItem.m_item.m_ammount = query.value(2).toDouble();
      fNoteItem.m_item.m_price = query.value(3).toDouble();
      fNoteItem.m_item.m_bIsPackageAmmount = query.value(4).toBool();
      fNoteItem.m_fProduct.m_product.m_id = fNoteItem.m_item.m_productId;
      bSuccess = ProductSQL::execSelect(query, fNoteItem.m_fProduct, error);
      if (!bSuccess)
        break;
      else
        fNote.m_vfNoteItem.push_back(fNoteItem);
    }
  }

  if (bSuccess)
  {
    fNote.m_fSupplier.m_person.m_id = fNote.m_note.m_supplierId;
    bSuccess = PersonSQL::execSelect(query, fNote.m_fSupplier, error);
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    fNote.clear();
  }
  else
  {
    bSuccess = finishTransaction(db, bSuccess, error);
    if (!bSuccess)
      fNote.clear();
  }
  return bSuccess;
}

bool NoteSQL::remove(QSqlDatabase db,
                     qlonglong id,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " SQL_NOTE_ITEMS_TABLE_NAME
                " WHERE " SQL_NOTE_ITEMS_COL01 " = (:_v01)");
  query.bindValue(":_v01", id);
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    query.prepare("DELETE FROM " SQL_NOTE_TABLE_NAME
                  " WHERE " SQL_NOTE_COL00 " = (:_v00)");
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

  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_NOTE_TABLE_NAME " ("
                        SQL_NOTE_COL00 " _ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                        SQL_NOTE_COL01 " INTEGER UNIQUE NOT NULL,"
                        SQL_NOTE_COL02 " TEXT NOT NULL,"
                        SQL_NOTE_COL03 " INTEGER NOT NULL,"
                        SQL_NOTE_COL04 " REAL,"
                        SQL_NOTE_COL05 " INT,"
                        "FOREIGN KEY(" SQL_NOTE_COL03 ") REFERENCES "
                        SQL_PERSON_TABLE_NAME "(" SQL_PERSON_COL00 "))");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SQL_NOTE_ITEMS_TABLE_NAME " ("
                          SQL_NOTE_ITEMS_COL00 "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          SQL_NOTE_ITEMS_COL01 " INTEGER NOT NULL,"
                          SQL_NOTE_ITEMS_COL02 " INTEGER NOT NULL,"
                          SQL_NOTE_ITEMS_COL03 " REAL,"
                          SQL_NOTE_ITEMS_COL04 " REAL,"
                          SQL_NOTE_ITEMS_COL04 " INT,"
                          "FOREIGN KEY(" SQL_NOTE_ITEMS_COL01 ") REFERENCES "
                          SQL_NOTE_TABLE_NAME "(" SQL_NOTE_COL00 "),"
                          "FOREIGN KEY(" SQL_NOTE_ITEMS_COL02 ") REFERENCES "
                          SQL_PRODUCT_TABLE_NAME "(" SQL_PRODUCT_COL00 "))");

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
                          "FOREIGN KEY(" SQL_CATEGORY_COL01 ") REFERENCES "
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
                        SQL_PRODUCT_COL02 " INTEGER NOT NULL,"
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
                          SQL_PERSON_COL14 " TEXT,"
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

bool ProductSQL::execSelect(QSqlQuery& query,
                            FullProduct& fProduct,
                            QString& error)
{
  error.clear();
  qlonglong id = fProduct.m_product.m_id;
  fProduct.clear();

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
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      fProduct.m_product.m_id = id;
      fProduct.m_product.m_name = query.value(0).toString();
      fProduct.m_product.m_categoryId = query.value(1).toLongLong();
      fProduct.m_product.m_imageId = query.value(2).toLongLong();
      fProduct.m_product.m_unity = query.value(3).toString();
      fProduct.m_product.m_packageUnity = query.value(4).toString();
      fProduct.m_product.m_packageAmmount = query.value(5).toDouble();
      fProduct.m_product.m_details = query.value(6).toString();
      fProduct.m_product.m_code = query.value(7).toString();
      fProduct.m_product.m_bAvailableAtNotes = query.value(8).toBool();
      fProduct.m_product.m_bAvailableAtShop = query.value(9).toBool();
      fProduct.m_product.m_bAvailableAtConsumption = query.value(10).toBool();
      fProduct.m_product.m_bAvailableToBuy = query.value(11).toBool();
      fProduct.m_product.m_bAvailableToSell = query.value(12).toBool();
    }
    else
    {
      error = "Produto não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    fProduct.m_image.m_id = fProduct.m_product.m_imageId;
    bSuccess = ImageSQL::execSelect(query, fProduct.m_image, error);
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    fProduct.clear();
  }

  return bSuccess;
}

bool ProductSQL::select(QSqlDatabase db,
                        FullProduct& fProduct,
                        QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, fProduct, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    fProduct.clear();
  return bSuccess;
}

bool ProductSQL::insert(QSqlDatabase db,
                        const Product& product,
                        QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " SQL_PRODUCT_TABLE_NAME " ("
                SQL_PRODUCT_COL01 ","
                SQL_PRODUCT_COL02 ","
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

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    product.m_id = query.lastInsertId().toLongLong();
    if (Image::st_isValidId(product.m_imageId))
    {
      query.prepare("UPDATE " SQL_PRODUCT_TABLE_NAME " SET "
                    SQL_PRODUCT_COL03 " = (:_v03)"
                    " WHERE " SQL_PRODUCT_COL00 " = (:_v00)");
      query.bindValue(":_v03", product.m_imageId);
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
    bSuccess = db.commit();

  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
}

bool ProductSQL::update(QSqlDatabase db,
                        const Product& product,
                        QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " SQL_PRODUCT_TABLE_NAME " SET "
                SQL_PRODUCT_COL01 " = (:_v01),"
                SQL_PRODUCT_COL02 " = (:_v02),"
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
  bool bSuccess = query.exec();
  if (bSuccess && Image::st_isValidId(product.m_imageId))
  {
    query.prepare("UPDATE " SQL_PRODUCT_TABLE_NAME " SET "
                  SQL_PRODUCT_COL03 " = (:_v03)"
                  " WHERE " SQL_PRODUCT_COL00 " = (:_v00)");
    query.bindValue(":_v00", product.m_id);
    query.bindValue(":_v03", product.m_imageId);
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

bool ProductSQL::remove(QSqlDatabase db,
                        qlonglong id,
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

bool CategorySQL::execSelect(QSqlQuery& query,
                             FullCategory& fCategory,
                             QString& error)
{
  error.clear();
  qlonglong id = fCategory.m_category.m_id;
  fCategory.clear();

  query.prepare("SELECT "
                SQL_CATEGORY_COL01 ","
                SQL_CATEGORY_COL02
                " FROM " SQL_CATEGORY_TABLE_NAME
                " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      fCategory.m_category.m_id = id;
      fCategory.m_category.m_imageId = query.value(0).toLongLong();
      fCategory.m_category.m_name = query.value(1).toString();
    }
    else
    {
      error = "Categoria não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    fCategory.m_image.m_id = fCategory.m_category.m_imageId;
    bSuccess = ImageSQL::execSelect(query, fCategory.m_image, error);
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    fCategory.clear();
  }

  return bSuccess;
}

bool CategorySQL::select(QSqlDatabase db,
                         FullCategory& fCategory,
                         QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, fCategory, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    fCategory.clear();
  return bSuccess;
}

bool CategorySQL::insert(QSqlDatabase db,
                         const Category& category,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " SQL_CATEGORY_TABLE_NAME " ("
                SQL_CATEGORY_COL02 ")"
                " VALUES ("
                "(:_v02))");
  query.bindValue(":_v02", category.m_name);
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    category.m_id = query.lastInsertId().toLongLong();
    if (Image::st_isValidId(category.m_imageId))
    {
      query.prepare("UPDATE " SQL_CATEGORY_TABLE_NAME " SET "
                    SQL_CATEGORY_COL01 " = (:_v01)"
                    " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
      query.bindValue(":_v00", category.m_id);
      query.bindValue(":_v01", category.m_imageId);
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
    bSuccess = db.commit();

  if (!bSuccess)
    error = db.lastError().text();

  return bSuccess;
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
                SQL_CATEGORY_COL02 " = (:_v02)"
                " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
  query.bindValue(":_v00", category.m_id);
  query.bindValue(":_v02", category.m_name);
  bool bSuccess = query.exec();
  if (bSuccess && Image::st_isValidId(category.m_imageId))
  {
    query.prepare("UPDATE " SQL_CATEGORY_TABLE_NAME " SET "
                  SQL_CATEGORY_COL01 " = (:_v01)"
                  " WHERE " SQL_CATEGORY_COL00 " = (:_v00)");
    query.bindValue(":_v00", category.m_id);
    query.bindValue(":_v01", category.m_imageId);
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

bool CategorySQL::remove(QSqlDatabase db,
                         qlonglong id,
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

bool ImageSQL::execSelect(QSqlQuery& query,
                          Image& image,
                          QString& error)
{
  qlonglong id = image.m_id;
  image.clear();
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

bool ImageSQL::select(QSqlDatabase db,
                      Image& image,
                      QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, image, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    image.clear();
  return bSuccess;
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
                SQL_IMAGE_COL01 ","
                SQL_IMAGE_COL02 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", image.m_name);
  query.bindValue(":_v02", image.m_image);

  if (query.exec())
  {
    image.m_id = query.lastInsertId().toLongLong();
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
                      qlonglong id,
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
                              qlonglong id,
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
                             qlonglong id)
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
    FullProduct product;
    product.m_product.m_id = vConsumption.at(i).m_itemID;
    ProductSQL::select(db, product, error);
    vProduct.push_back(product.m_product);
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
    user.m_id = query.lastInsertId().toLongLong();
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
  qlonglong id = user.m_id;
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
                     qlonglong id,
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
      m_user.m_id = query.value(0).toLongLong();
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

bool PersonSQL::execSelect(QSqlQuery& query,
                           FullPerson& fPerson,
                           QString& error)
{
  error.clear();
  qlonglong id = fPerson.m_person.m_id;
  fPerson.clear();

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
                SQL_PERSON_COL14
                " FROM " SQL_PERSON_TABLE_NAME
                " WHERE " SQL_PERSON_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      fPerson.m_person.m_id = id;
      fPerson.m_person.m_imageId = query.value(0).toLongLong();
      fPerson.m_person.m_name = query.value(1).toString();
      fPerson.m_person.m_alias = query.value(2).toString();
      fPerson.m_person.m_email = query.value(3).toString();
      fPerson.m_person.m_CPF_CNPJ = query.value(4).toString();
      fPerson.m_person.m_RG_IE = query.value(5).toString();
      fPerson.m_person.m_details = query.value(6).toString();
      fPerson.m_person.m_birthDate = query.value(7).toString();
      fPerson.m_person.m_creationDate = query.value(8).toString();
      fPerson.m_person.m_bCompany = query.value(9).toBool();
      fPerson.m_person.m_bCustomer = query.value(10).toBool();
      fPerson.m_person.m_bSupplier = query.value(11).toBool();
      fPerson.m_person.m_bEmployee = query.value(12).toBool();
      fPerson.m_person.m_bEmployee = query.value(13).toBool();
    }
    else
    {
      error = "Pessoa não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_ADDRESS_COL00 ","
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
      address.m_cep = query.value(1).toString();
      address.m_neighborhood = query.value(2).toString();
      address.m_street = query.value(3).toString();
      address.m_number = query.value(4).toInt();
      address.m_city = query.value(5).toString();
      address.m_state = (Address::EBRState)query.value(6).toInt();
      address.m_complement = query.value(7).toString();
      address.m_reference = query.value(8).toString();
      fPerson.m_vAddress.push_back(address);
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
      fPerson.m_vPhone.push_back(phone);
    }
  }

  if (bSuccess)
  {
    fPerson.m_image.m_id = fPerson.m_person.m_imageId;
    bSuccess = ImageSQL::execSelect(query, fPerson.m_image, error);
  }

  if (!bSuccess)
  {
    error = query.lastError().text();
    fPerson.clear();
  }

  return bSuccess;
}

bool PersonSQL::select(QSqlDatabase db,
                       FullPerson& fPerson,
                       QString& error)
{
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, fPerson, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    fPerson.clear();
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
                SQL_PERSON_COL13 ","
                SQL_PERSON_COL14  ")"
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
                "(:_v13),"
                "(:_v14))");
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
  query.bindValue(":_v14", person.m_employeePinCode);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    person.m_id = query.lastInsertId().toLongLong();
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
      query.prepare("INSERT INTO " SQL_PHONE_TABLE_NAME " ("
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
        vPhone.at(i).m_id = query.lastInsertId().toLongLong();
      else
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != vAddress.size(); ++i)
    {
      query.prepare("INSERT INTO " SQL_ADDRESS_TABLE_NAME " ("
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
        vAddress.at(i).m_id = query.lastInsertId().toLongLong();
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
                       const QVector<qlonglong>& vRemovedPhoneId,
                       const QVector<qlonglong>& vRemovedAddressId,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);

  query.prepare("UPDATE " SQL_PERSON_TABLE_NAME " SET "
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
                SQL_PERSON_COL13 " = (:_v13),"
                SQL_PERSON_COL14 " = (:_v14)"
                " WHERE " SQL_PERSON_COL00 " = (:_v00)");
  query.bindValue(":_v00", person.m_id);
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
  query.bindValue(":_v14", person.m_employeePinCode);
  bool bSuccess = query.exec();

  if (bSuccess && Image::st_isValidId(person.m_imageId))
  {
    query.prepare("UPDATE " SQL_PERSON_TABLE_NAME " SET "
                  SQL_PERSON_COL01 " = (:_v01)"
                                   " WHERE " SQL_PERSON_COL00 " = (:_v00)");
    query.bindValue(":_v00", person.m_id);
    query.bindValue(":_v01", person.m_imageId);
    bSuccess = query.exec();
  }

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
                       qlonglong id,
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
