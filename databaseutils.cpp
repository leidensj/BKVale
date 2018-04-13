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
  qlonglong number = NOTE_SQL_DEFAULT_NUMBER;
  if (!db.isOpen())
    return number;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = query.exec("SELECT MAX(" NOTE_SQL_COL01 ") FROM " NOTE_SQL_TABLE_NAME);
  if (bSuccess)
    number = query.next() ? query.value(0).toLongLong() + 1 : NOTE_SQL_DEFAULT_NUMBER;
  number = number > NOTE_SQL_DEFAULT_NUMBER ? number : NOTE_SQL_DEFAULT_NUMBER;

  if (!bSuccess)
    db.rollback();
  else
    db.commit();

  return number;
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

  bool bSuccess = query.exec("SELECT MAX(" NOTE_SQL_COL01 ") FROM " NOTE_SQL_TABLE_NAME);
  if (bSuccess)
  {
      qlonglong number = query.next()
                         ? query.value(0).toLongLong() + 1
                         : NOTE_SQL_DEFAULT_NUMBER;
      number = number > NOTE_SQL_DEFAULT_NUMBER ? number : NOTE_SQL_DEFAULT_NUMBER;

      query.prepare("INSERT INTO " NOTE_SQL_TABLE_NAME " ("
                    NOTE_SQL_COL01 ","
                    NOTE_SQL_COL02 ","
                    NOTE_SQL_COL03 ","
                    NOTE_SQL_COL04 ","
                    NOTE_SQL_COL05
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05))");
      query.bindValue(":_v01", number);
      query.bindValue(":_v02", note.m_date);
      query.bindValue(":_v03", note.m_supplier.m_id);
      query.bindValue(":_v04", note.m_total);
      query.bindValue(":_v05", note.m_bCash);
      bSuccess = query.exec();
  }

  if (bSuccess)
  {
    note.m_id = query.lastInsertId().toLongLong();
    for (int i = 0; i != note.m_vNoteItem.size(); ++i)
    {
      query.prepare("INSERT INTO " NOTE_ITEMS_SQL_TABLE_NAME " ("
                    NOTE_ITEMS_SQL_COL01 ","
                    NOTE_ITEMS_SQL_COL02 ","
                    NOTE_ITEMS_SQL_COL03 ","
                    NOTE_ITEMS_SQL_COL04 ","
                    NOTE_ITEMS_SQL_COL05
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05))");
      query.bindValue(":_v01", note.m_id);
      query.bindValue(":_v02", note.m_vNoteItem.at(i).m_product.m_id);
      query.bindValue(":_v03", note.m_vNoteItem.at(i).m_ammount);
      query.bindValue(":_v04", note.m_vNoteItem.at(i).m_price);
      query.bindValue(":_v05", note.m_vNoteItem.at(i).m_bIsPackageAmmount);
      bSuccess = query.exec();
      if (bSuccess)
        note.m_vNoteItem.at(i).m_id = query.lastInsertId().toLongLong();
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
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " NOTE_SQL_TABLE_NAME " SET "
                NOTE_SQL_COL02 " = (:_v02),"
                NOTE_SQL_COL03 " = (:_v03),"
                NOTE_SQL_COL04 " = (:_v04),"
                NOTE_SQL_COL05 " = (:_v05) "
                "WHERE " NOTE_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", note.m_id);
  query.bindValue(":_v02", note.m_date);
  query.bindValue(":_v03", note.m_supplier.m_id);
  query.bindValue(":_v04", note.m_total);
  query.bindValue(":_v05", note.m_bCash);
  bool bSuccess = query.exec();

  if (bSuccess && !note.m_vNoteItem.isEmpty())
  {
    QString str = "DELETE FROM " NOTE_ITEMS_SQL_TABLE_NAME
                  " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)"
                  " AND " NOTE_ITEMS_SQL_COL00 " NOT IN (";
    for (int i = 0; i != note.m_vNoteItem.size(); ++i)
      str += "(:_v00" + QString::number(i) + "),";
    str.replace(str.length() - 1, 1, ")");
    query.prepare(str);
    query.bindValue(":_v01", note.m_id);
    for (int i = 0; i != note.m_vNoteItem.size(); ++i)
      query.bindValue("_v00" + QString::number(i), note.m_vNoteItem.at(i).m_id);
    bSuccess = query.exec();
  }
  else if (note.m_vNoteItem.isEmpty())
  {
    query.prepare("DELETE FROM " NOTE_ITEMS_SQL_TABLE_NAME " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", note.m_id);
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != note.m_vNoteItem.size(); ++i)
    {
      if (note.m_vNoteItem.at(i).isValidId())
      {
        query.prepare("UPDATE " NOTE_ITEMS_SQL_TABLE_NAME " SET "
                      NOTE_ITEMS_SQL_COL01 " = (:_v01),"
                      NOTE_ITEMS_SQL_COL02 " = (:_v02),"
                      NOTE_ITEMS_SQL_COL03 " = (:_v03),"
                      NOTE_ITEMS_SQL_COL04 " = (:_v04),"
                      NOTE_ITEMS_SQL_COL05 " = (:_v05)"
                      " WHERE " NOTE_ITEMS_SQL_COL00 " = (:_v00)");
        query.bindValue(":_v00", note.m_vNoteItem.at(i).m_id);
        query.bindValue(":_v01", note.m_id);
        query.bindValue(":_v02", note.m_vNoteItem.at(i).m_product.m_id);
        query.bindValue(":_v03", note.m_vNoteItem.at(i).m_ammount);
        query.bindValue(":_v04", note.m_vNoteItem.at(i).m_price);
        query.bindValue(":_v05", note.m_vNoteItem.at(i).m_bIsPackageAmmount);
        bSuccess = query.exec();
      }
      else
      {
        query.prepare("INSERT INTO " NOTE_ITEMS_SQL_TABLE_NAME " ("
                      NOTE_ITEMS_SQL_COL01 ","
                      NOTE_ITEMS_SQL_COL02 ","
                      NOTE_ITEMS_SQL_COL03 ","
                      NOTE_ITEMS_SQL_COL04 ","
                      NOTE_ITEMS_SQL_COL05
                      " ) VALUES ("
                      "(:_v01),"
                      "(:_v02),"
                      "(:_v03),"
                      "(:_v04),"
                      "(:_v05))");
        query.bindValue(":_v01", note.m_id);
        query.bindValue(":_v02", note.m_vNoteItem.at(i).m_product.m_id);
        query.bindValue(":_v03", note.m_vNoteItem.at(i).m_ammount);
        query.bindValue(":_v04", note.m_vNoteItem.at(i).m_price);
        query.bindValue(":_v05", note.m_vNoteItem.at(i).m_bIsPackageAmmount);
        bSuccess = query.exec();
        if (bSuccess)
          note.m_vNoteItem.at(i).m_id = query.lastInsertId().toLongLong();
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
                     Note& note,
                     QString& error)
{
  error.clear();
  qlonglong id = note.m_id;
  note.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                NOTE_SQL_COL01 ","
                NOTE_SQL_COL02 ","
                NOTE_SQL_COL03 ","
                NOTE_SQL_COL04 ","
                NOTE_SQL_COL05
                " FROM " NOTE_SQL_TABLE_NAME
                " WHERE " NOTE_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      note.m_id = id;
      note.m_number = query.value(0).toLongLong();
      note.m_date = query.value(1).toString();
      note.m_supplier.m_id = query.value(2).toLongLong();
      note.m_total = query.value(3).toDouble();
      note.m_bCash = query.value(4).toBool();
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
                  NOTE_ITEMS_SQL_COL00 ","
                  NOTE_ITEMS_SQL_COL02 ","
                  NOTE_ITEMS_SQL_COL03 ","
                  NOTE_ITEMS_SQL_COL04 ","
                  NOTE_ITEMS_SQL_COL05
                  " FROM " NOTE_ITEMS_SQL_TABLE_NAME
                  " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", note.m_id);
    bSuccess = query.exec();
    while (bSuccess && query.next())
    {
      NoteItem noteItem;
      noteItem.m_id = query.value(0).toLongLong();
      noteItem.m_product.m_id = query.value(1).toLongLong();
      noteItem.m_ammount = query.value(2).toDouble();
      noteItem.m_price = query.value(3).toDouble();
      noteItem.m_bIsPackageAmmount = query.value(4).toBool();
      if (noteItem.m_product.isValidId())
        bSuccess = ProductSQL::execSelect(query, noteItem.m_product, error);
      if (!bSuccess)
        break;
      else
        note.m_vNoteItem.push_back(noteItem);
    }
  }

  if (bSuccess && note.m_supplier.isValidId())
    bSuccess = PersonSQL::execSelect(query, note.m_supplier, error);

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    note.clear();
  }
  else
  {
    bSuccess = finishTransaction(db, bSuccess, error);
    if (!bSuccess)
      note.clear();
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
  query.prepare("DELETE FROM " NOTE_SQL_TABLE_NAME
                " WHERE " NOTE_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();

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

double NoteSQL::selectPriceSuggestion(QSqlDatabase db,
                                      qlonglong supplierId,
                                      qlonglong productId)
{
  double val = 0.0;
  QString error;
  if (!BaitaSQL::isOpen(db, error))
    return val;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL04
                " FROM " NOTE_SQL_TABLE_NAME
                " INNER JOIN " NOTE_ITEMS_SQL_TABLE_NAME
                " ON " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL00
                " = " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL01
                " WHERE " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03
                " = (:_v01)"
                " AND " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL02
                " = (:_v02) "
                " ORDER BY " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL00
                " DESC LIMIT 1");
  query.bindValue(":_v01", supplierId);
  query.bindValue(":_v02", productId);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
      val = query.value(0).toDouble();
  }
  error = query.lastError().text();

  finishTransaction(db, bSuccess, error);
  return val;
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

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " IMAGE_SQL_TABLE_NAME " ("
                          IMAGE_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          IMAGE_SQL_COL01 " TEXT NOT NULL UNIQUE,"
                          IMAGE_SQL_COL02 " BLOB)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " CATEGORY_SQL_TABLE_NAME " ("
                          CATEGORY_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          CATEGORY_SQL_COL01 " INTEGER,"
                          CATEGORY_SQL_COL02 " TEXT NOT NULL UNIQUE,"
                          "FOREIGN KEY(" CATEGORY_SQL_COL01 ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" IMAGE_SQL_COL00 ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " REMINDER_SQL_TABLE_NAME " ("
                          REMINDER_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          REMINDER_SQL_COL01 " TEXT,"
                          REMINDER_SQL_COL02 " TEXT,"
                          REMINDER_SQL_COL03 " INT,"
                          REMINDER_SQL_COL04 " INT,"
                          REMINDER_SQL_COL05 " INT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS _CONSUMPTION ("
                          "_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "_DATE INTEGER,"
                          "_PRODUCTID INTEGER,"
                          "_PRICE REAL,"
                          "_AMMOUNT REAL,"
                          "_TOTAL REAL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " USER_SQL_TABLE_NAME " ("
                          USER_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          USER_SQL_COL01 " TEXT NOT NULL UNIQUE,"
                          USER_SQL_COL02 " TEXT NOT NULL,"
                          USER_SQL_COL03 " INT,"
                          USER_SQL_COL04 " INT,"
                          USER_SQL_COL05 " INT,"
                          USER_SQL_COL06 " INT,"
                          USER_SQL_COL07 " INT,"
                          USER_SQL_COL08 " INT,"
                          USER_SQL_COL09 " INT,"
                          USER_SQL_COL10 " INT,"
                          USER_SQL_COL11 " INT,"
                          USER_SQL_COL12 " INT,"
                          USER_SQL_COL13 " INT)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PRODUCT_SQL_TABLE_NAME " ("
                        PRODUCT_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        PRODUCT_SQL_COL01 " TEXT NOT NULL UNIQUE,"
                        PRODUCT_SQL_COL02 " INTEGER,"
                        PRODUCT_SQL_COL03 " INTEGER,"
                        PRODUCT_SQL_COL04 " TEXT NOT NULL,"
                        PRODUCT_SQL_COL05 " TEXT,"
                        PRODUCT_SQL_COL06 " REAL,"
                        PRODUCT_SQL_COL07 " TEXT,"
                        PRODUCT_SQL_COL08 " INT,"
                        PRODUCT_SQL_COL09 " INT,"
                        PRODUCT_SQL_COL10 " INT,"
                        PRODUCT_SQL_COL11 " INT,"
                        PRODUCT_SQL_COL12 " INT,"
                        "FOREIGN KEY(" PRODUCT_SQL_COL02 ") REFERENCES "
                        CATEGORY_SQL_TABLE_NAME "(" CATEGORY_SQL_COL00 ") ON DELETE SET NULL,"
                        "FOREIGN KEY(" PRODUCT_SQL_COL03 ") REFERENCES "
                        IMAGE_SQL_TABLE_NAME "(" IMAGE_SQL_COL00 ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PERSON_SQL_TABLE_NAME " ("
                          PERSON_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          PERSON_SQL_COL01 " INTEGER,"
                          PERSON_SQL_COL02 " TEXT NOT NULL UNIQUE,"
                          PERSON_SQL_COL03 " TEXT,"
                          PERSON_SQL_COL04 " TEXT,"
                          PERSON_SQL_COL05 " TEXT,"
                          PERSON_SQL_COL06 " TEXT,"
                          PERSON_SQL_COL07 " TEXT,"
                          PERSON_SQL_COL08 " TEXT,"
                          PERSON_SQL_COL09 " TEXT,"
                          PERSON_SQL_COL10 " INT,"
                          PERSON_SQL_COL11 " INT,"
                          PERSON_SQL_COL12 " INT,"
                          PERSON_SQL_COL13 " INT,"
                          PERSON_SQL_COL14 " TEXT,"
                          "FOREIGN KEY(" PERSON_SQL_COL01 ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" IMAGE_SQL_COL00 ") ON DELETE SET NULL)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " ADDRESS_SQL_TABLE_NAME " ("
                        ADDRESS_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        ADDRESS_SQL_COL01 " INTEGER,"
                        ADDRESS_SQL_COL02 " TEXT,"
                        ADDRESS_SQL_COL03 " TEXT,"
                        ADDRESS_SQL_COL04 " TEXT,"
                        ADDRESS_SQL_COL05 " INT,"
                        ADDRESS_SQL_COL06 " TEXT,"
                        ADDRESS_SQL_COL07 " INT,"
                        ADDRESS_SQL_COL08 " TEXT,"
                        ADDRESS_SQL_COL09 " TEXT,"
                        "FOREIGN KEY(" ADDRESS_SQL_COL01 ") REFERENCES "
                        PERSON_SQL_TABLE_NAME "(" ADDRESS_SQL_COL00 ") ON DELETE CASCADE)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PHONE_SQL_TABLE_NAME " ("
                        PHONE_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        PHONE_SQL_COL01 " INTEGER,"
                        PHONE_SQL_COL02 " INT DEFAULT " PHONE_DEFAULT_COUNTRY_CODE_VALUE_STR ","
                        PHONE_SQL_COL03 " INT DEFAULT " PHONE_DEFAULT_CODE_VALUE_STR ","
                        PHONE_SQL_COL04 " TEXT,"
                        "FOREIGN KEY(" PHONE_SQL_COL01 ") REFERENCES "
                        PERSON_SQL_TABLE_NAME "(" PERSON_SQL_COL00 ") ON DELETE CASCADE)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " NOTE_SQL_TABLE_NAME " ("
                        NOTE_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                        NOTE_SQL_COL01 " INTEGER UNIQUE NOT NULL,"
                        NOTE_SQL_COL02 " TEXT NOT NULL,"
                        NOTE_SQL_COL03 " INTEGER,"
                        NOTE_SQL_COL04 " REAL,"
                        NOTE_SQL_COL05 " INT,"
                        "FOREIGN KEY(" NOTE_SQL_COL03 ") REFERENCES "
                        PERSON_SQL_TABLE_NAME "(" PERSON_SQL_COL00 ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " NOTE_ITEMS_SQL_TABLE_NAME " ("
                          NOTE_ITEMS_SQL_COL00 " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          NOTE_ITEMS_SQL_COL01 " INTEGER NOT NULL,"
                          NOTE_ITEMS_SQL_COL02 " INTEGER,"
                          NOTE_ITEMS_SQL_COL03 " REAL,"
                          NOTE_ITEMS_SQL_COL04 " REAL,"
                          NOTE_ITEMS_SQL_COL05 " INT,"
                          "FOREIGN KEY(" NOTE_ITEMS_SQL_COL01 ") REFERENCES "
                          NOTE_SQL_TABLE_NAME "(" NOTE_SQL_COL00 ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" NOTE_ITEMS_SQL_COL02 ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" PRODUCT_SQL_COL00 ") ON DELETE SET NULL)");

  if (bSuccess)
  {
    query.exec("SELECT * FROM " USER_SQL_TABLE_NAME " LIMIT 1");
    if (!query.next())
    {
      query.prepare("INSERT INTO " USER_SQL_TABLE_NAME " ("
                    USER_SQL_COL01 ","
                    USER_SQL_COL02 ","
                    USER_SQL_COL03 ","
                    USER_SQL_COL04 ","
                    USER_SQL_COL05 ","
                    USER_SQL_COL06 ","
                    USER_SQL_COL07 ","
                    USER_SQL_COL08 ","
                    USER_SQL_COL09 ","
                    USER_SQL_COL10 ","
                    USER_SQL_COL11 ","
                    USER_SQL_COL12 ","
                    USER_SQL_COL13 ")"
                    " VALUES ('"
                    USER_SQL_DEFAULT_NAME "',"
                    "(:_password),"
                    "1,1,1,1,1,1,1,1,1,1,1)");
      query.bindValue(":_password", User::st_strEncryptedPassword(USER_SQL_DEFAULT_PASSWORD));
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
                            Product& product,
                            QString& error)
{
  error.clear();
  qlonglong id = product.m_id;
  product.clear();

  query.prepare("SELECT "
                PRODUCT_SQL_COL01 ","
                PRODUCT_SQL_COL02 ","
                PRODUCT_SQL_COL03 ","
                PRODUCT_SQL_COL04 ","
                PRODUCT_SQL_COL05 ","
                PRODUCT_SQL_COL06 ","
                PRODUCT_SQL_COL07 ","
                PRODUCT_SQL_COL08 ","
                PRODUCT_SQL_COL09 ","
                PRODUCT_SQL_COL10 ","
                PRODUCT_SQL_COL11 ","
                PRODUCT_SQL_COL12
                " FROM " PRODUCT_SQL_TABLE_NAME
                " WHERE " PRODUCT_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      product.m_id = id;
      product.m_name = query.value(0).toString();
      product.m_category.m_id = query.value(1).toLongLong();
      product.m_image.m_id = query.value(2).toLongLong();
      product.m_unity = query.value(3).toString();
      product.m_packageUnity = query.value(4).toString();
      product.m_packageAmmount = query.value(5).toDouble();
      product.m_details = query.value(6).toString();
      product.m_bAvailableAtNotes = query.value(7).toBool();
      product.m_bAvailableAtShop = query.value(8).toBool();
      product.m_bAvailableAtConsumption = query.value(9).toBool();
      product.m_bAvailableToBuy = query.value(10).toBool();
      product.m_bAvailableToSell = query.value(11).toBool();
    }
    else
    {
      error = "Produto não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess && product.m_image.isValidId())
    bSuccess = ImageSQL::execSelect(query, product.m_image, error);

  if (bSuccess && product.m_category.isValidId())
    bSuccess = CategorySQL::execSelect(query, product.m_category, error);

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    product.clear();
  }

  return bSuccess;
}

bool ProductSQL::select(QSqlDatabase db,
                        Product& product,
                        QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, product, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    product.clear();
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
  query.prepare(
        QString("INSERT INTO " PRODUCT_SQL_TABLE_NAME " ("
                PRODUCT_SQL_COL01 ","
                PRODUCT_SQL_COL02 ","
                PRODUCT_SQL_COL03 ","
                PRODUCT_SQL_COL04 ","
                PRODUCT_SQL_COL05 ","
                PRODUCT_SQL_COL06 ","
                PRODUCT_SQL_COL07 ","
                PRODUCT_SQL_COL08 ","
                PRODUCT_SQL_COL09 ","
                PRODUCT_SQL_COL10 ","
                PRODUCT_SQL_COL11 ","
                PRODUCT_SQL_COL12 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),") +
        (product.m_image.isValidId()
         ? "(:_v03)," : "NULL,") +
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07),"
                "(:_v08),"
                "(:_v09),"
                "(:_v10),"
                "(:_v11),"
                "(:_v12))");
  query.bindValue(":_v01", product.m_name);
  query.bindValue(":_v02", product.m_category.m_id);
  if (product.m_image.isValidId())
    query.bindValue(":_v03", product.m_image.m_id);
  query.bindValue(":_v04", product.m_unity);
  query.bindValue(":_v05", product.m_packageUnity);
  query.bindValue(":_v06", product.m_packageAmmount);
  query.bindValue(":_v07", product.m_details);
  query.bindValue(":_v08", product.m_bAvailableAtNotes);
  query.bindValue(":_v09", product.m_bAvailableAtShop);
  query.bindValue(":_v10", product.m_bAvailableAtConsumption);
  query.bindValue(":_v11", product.m_bAvailableToBuy);
  query.bindValue(":_v12", product.m_bAvailableToSell);

  bool bSuccess = query.exec();
  if (bSuccess)
    product.m_id = query.lastInsertId().toLongLong();

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
  query.prepare(
        QString("UPDATE " PRODUCT_SQL_TABLE_NAME " SET "
                PRODUCT_SQL_COL01 " = (:_v01),"
                PRODUCT_SQL_COL02 " = (:_v02),"
                PRODUCT_SQL_COL03 " = ") +
        (product.m_image.isValidId() ? "(:_v03)," : "NULL,") +
                PRODUCT_SQL_COL04 " = (:_v04),"
                PRODUCT_SQL_COL05 " = (:_v05),"
                PRODUCT_SQL_COL06 " = (:_v06),"
                PRODUCT_SQL_COL07 " = (:_v07),"
                PRODUCT_SQL_COL08 " = (:_v08),"
                PRODUCT_SQL_COL09 " = (:_v09),"
                PRODUCT_SQL_COL10 " = (:_v10),"
                PRODUCT_SQL_COL11 " = (:_v11),"
                PRODUCT_SQL_COL12 " = (:_v12)"
                " WHERE " PRODUCT_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", product.m_id);
  query.bindValue(":_v01", product.m_name);
  query.bindValue(":_v02", product.m_category.m_id);
  if (product.m_image.isValidId())
    query.bindValue(":_v03", product.m_image.m_id);
  query.bindValue(":_v04", product.m_unity);
  query.bindValue(":_v05", product.m_packageUnity);
  query.bindValue(":_v06", product.m_packageAmmount);
  query.bindValue(":_v07", product.m_details);
  query.bindValue(":_v08", product.m_bAvailableAtNotes);
  query.bindValue(":_v09", product.m_bAvailableAtShop);
  query.bindValue(":_v10", product.m_bAvailableAtConsumption);
  query.bindValue(":_v11", product.m_bAvailableToBuy);
  query.bindValue(":_v12", product.m_bAvailableToSell);
  bool bSuccess = query.exec();

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
  query.prepare("DELETE FROM " PRODUCT_SQL_TABLE_NAME
                " WHERE " PRODUCT_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool CategorySQL::execSelect(QSqlQuery& query,
                             Category& category,
                             QString& error)
{
  error.clear();
  qlonglong id = category.m_id;
  category.clear();

  query.prepare("SELECT "
                CATEGORY_SQL_COL01 ","
                CATEGORY_SQL_COL02
                " FROM " CATEGORY_SQL_TABLE_NAME
                " WHERE " CATEGORY_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      category.m_id = id;
      category.m_image.m_id = query.value(0).toLongLong();
      category.m_name = query.value(1).toString();
    }
    else
    {
      error = "Categoria não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && category.m_image.isValidId())
    bSuccess = ImageSQL::execSelect(query, category.m_image, error);

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    category.clear();
  }

  return bSuccess;
}

bool CategorySQL::select(QSqlDatabase db,
                         Category& category,
                         QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, category, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    category.clear();
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
  query.prepare(
        QString("INSERT INTO " CATEGORY_SQL_TABLE_NAME " ("
                CATEGORY_SQL_COL01 ","
                CATEGORY_SQL_COL02 ")"
                " VALUES (") +
                (category.m_image.isValidId()
                 ? "(:_v01)," : "NULL,") +
                "(:_v02))");
  if (category.m_image.isValidId())
    query.bindValue(":_v01", category.m_image.m_id);
  query.bindValue(":_v02", category.m_name);
  bool bSuccess = query.exec();
  if (bSuccess)
    category.m_id = query.lastInsertId().toLongLong();

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

  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " CATEGORY_SQL_TABLE_NAME " SET "
                CATEGORY_SQL_COL02 " = (:_v02)"
                " WHERE " CATEGORY_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", category.m_id);
  query.bindValue(":_v02", category.m_name);
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (category.m_image.isValidId())
    {
      query.prepare("UPDATE " CATEGORY_SQL_TABLE_NAME " SET "
                    CATEGORY_SQL_COL01 " = (:_v01)"
                    " WHERE " CATEGORY_SQL_COL00 " = (:_v00)");
      query.bindValue(":_v01", category.m_image.m_id);
    }
    else
    {
      query.prepare("UPDATE " CATEGORY_SQL_TABLE_NAME " SET "
                    CATEGORY_SQL_COL01 " = NULL"
                    " WHERE " CATEGORY_SQL_COL00 " = (:_v00)");
    }
    query.bindValue(":_v00", category.m_id);
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
  query.prepare("DELETE FROM " CATEGORY_SQL_TABLE_NAME
                " WHERE " CATEGORY_SQL_COL00 " = (:_v00)");
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
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02
                " FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " IMAGE_SQL_COL00 " = (:_v00)");
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
  query.prepare("INSERT INTO " IMAGE_SQL_TABLE_NAME " ("
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02 ")"
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
  query.prepare("UPDATE " IMAGE_SQL_TABLE_NAME " SET "
                IMAGE_SQL_COL01 " = (:_v01),"
                IMAGE_SQL_COL02 " = (:_v02)"
                " WHERE " IMAGE_SQL_COL00 " = (:_v00)");
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
  query.prepare("DELETE FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " IMAGE_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool ReminderSQL::execSelect(QSqlQuery& query,
                             Reminder& reminder,
                             QString& error)
{
  qlonglong id = reminder.m_id;
  reminder.clear();
  query.prepare("SELECT "
                REMINDER_SQL_COL01 ","
                REMINDER_SQL_COL02 ","
                REMINDER_SQL_COL03 ","
                REMINDER_SQL_COL04 ","
                REMINDER_SQL_COL05
                " FROM " REMINDER_SQL_TABLE_NAME
                " WHERE " REMINDER_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
  {
    if (query.next())
    {
      reminder.m_id = id;
      reminder.m_title = query.value(0).toString();
      reminder.m_message = query.value(1).toString();
      reminder.m_bFavorite = query.value(2).toBool();
      reminder.m_capitalization = (Reminder::Capitalization)query.value(3).toInt();
      reminder.m_size = (Reminder::Size)query.value(4).toInt();
      return true;
    }
    else
    {
      error = "Lembrete não encontrado.";
      return false;
    }
  }
  else
  {
    error = query.lastError().text();
    return false;
  }
}

bool ReminderSQL::select(QSqlDatabase db,
                         Reminder& reminder,
                         QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, reminder, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    reminder.clear();
  return bSuccess;
}

bool ReminderSQL::insert(QSqlDatabase db,
                         const Reminder& reminder,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("INSERT INTO " REMINDER_SQL_TABLE_NAME " ("
                REMINDER_SQL_COL01 ","
                REMINDER_SQL_COL02 ","
                REMINDER_SQL_COL03 ","
                REMINDER_SQL_COL04 ","
                REMINDER_SQL_COL05
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05))");
  query.bindValue(":_v01", reminder.m_title);
  query.bindValue(":_v02", reminder.m_message);
  query.bindValue(":_v03", reminder.m_bFavorite);
  query.bindValue(":_v04", (int)reminder.m_capitalization);
  query.bindValue(":_v05", (int)reminder.m_size);

  if (query.exec())
  {
    reminder.m_id = query.lastInsertId().toLongLong();
    return true;
  }

  error = query.lastError().text();
  return false;
}

bool ReminderSQL::update(QSqlDatabase db,
                         const Reminder& reminder,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("UPDATE " REMINDER_SQL_TABLE_NAME " SET "
                REMINDER_SQL_COL01 " = (:_v01),"
                REMINDER_SQL_COL02 " = (:_v02),"
                REMINDER_SQL_COL03 " = (:_v03),"
                REMINDER_SQL_COL04 " = (:_v04),"
                REMINDER_SQL_COL05 " = (:_v02)"
                " WHERE " REMINDER_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", reminder.m_id);
  query.bindValue(":_v01", reminder.m_title);
  query.bindValue(":_v02", reminder.m_message);
  query.bindValue(":_v03", reminder.m_bFavorite);
  query.bindValue(":_v04", (int)reminder.m_capitalization);
  query.bindValue(":_v05", (int)reminder.m_size);

  if (query.exec())
    return true;

  error = query.lastError().text();
  return false;
}

bool ReminderSQL::remove(QSqlDatabase db,
                         qlonglong id,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("DELETE FROM " REMINDER_SQL_TABLE_NAME
                " WHERE " REMINDER_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);

  if (query.exec())
    return true;

  error = query.lastError().text();
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
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QSqlQuery query(db);
  query.prepare("INSERT INTO " USER_SQL_TABLE_NAME " ("
                USER_SQL_COL01 ","
                USER_SQL_COL02 ","
                USER_SQL_COL03 ","
                USER_SQL_COL04 ","
                USER_SQL_COL05 ","
                USER_SQL_COL06 ","
                USER_SQL_COL07 ","
                USER_SQL_COL08 ","
                USER_SQL_COL09 ","
                USER_SQL_COL10 ","
                USER_SQL_COL11 ","
                USER_SQL_COL12 ","
                USER_SQL_COL13 ")"
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
  query.bindValue(":_v01", user.m_strUser);
  query.bindValue(":_v02", user.strEncryptedPassword());
  query.bindValue(":_v03", user.m_bAccessNote);
  query.bindValue(":_v04", user.m_bAccessReminder);
  query.bindValue(":_v05", user.m_bAccessCalculator);
  query.bindValue(":_v06", user.m_bAccessShop);
  query.bindValue(":_v07", user.m_bAccessConsumption);
  query.bindValue(":_v08", user.m_bAccessUser);
  query.bindValue(":_v09", user.m_bAccessProduct);
  query.bindValue(":_v10", user.m_bAccessSettings);
  query.bindValue(":_v11", user.m_bAccessPerson);
  query.bindValue(":_v12", user.m_bAccessCategory);
  query.bindValue(":_v13", user.m_bAccessImage);

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
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL01 " = (:_v01),");
  if (!user.m_password.isEmpty())
    strQuery += USER_SQL_COL02 " = (:_v02),";
  strQuery += USER_SQL_COL03" = (:_v03),"
              USER_SQL_COL04" = (:_v04),"
              USER_SQL_COL05" = (:_v05),"
              USER_SQL_COL06" = (:_v06),"
              USER_SQL_COL07" = (:_v07),"
              USER_SQL_COL08" = (:_v08),"
              USER_SQL_COL09" = (:_v09),"
              USER_SQL_COL10" = (:_v10),"
              USER_SQL_COL11" = (:_v11),"
              USER_SQL_COL12" = (:_v12),"
              USER_SQL_COL13" = (:_v13)"
              " WHERE " USER_SQL_COL00 " = (:_v00)";


  QSqlQuery query(db);
  query.prepare(strQuery);
  query.bindValue(":_v00", user.m_id);
  query.bindValue(":_v01", user.m_strUser);
  if (!user.m_password.isEmpty())
    query.bindValue(":_v02", user.strEncryptedPassword());
  query.bindValue(":_v03", user.m_bAccessNote);
  query.bindValue(":_v04", user.m_bAccessReminder);
  query.bindValue(":_v05", user.m_bAccessCalculator);
  query.bindValue(":_v06", user.m_bAccessShop);
  query.bindValue(":_v07", user.m_bAccessConsumption);
  query.bindValue(":_v08", user.m_bAccessUser);
  query.bindValue(":_v09", user.m_bAccessProduct);
  query.bindValue(":_v10", user.m_bAccessSettings);
  query.bindValue(":_v11", user.m_bAccessPerson);
  query.bindValue(":_v12", user.m_bAccessCategory);
  query.bindValue(":_v13", user.m_bAccessImage);

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
                USER_SQL_COL01 ","
                USER_SQL_COL02 ","
                USER_SQL_COL03 ","
                USER_SQL_COL04 ","
                USER_SQL_COL05 ","
                USER_SQL_COL06 ","
                USER_SQL_COL07 ","
                USER_SQL_COL08 ","
                USER_SQL_COL09 ","
                USER_SQL_COL10 ","
                USER_SQL_COL11 ","
                USER_SQL_COL12 ","
                USER_SQL_COL13
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " ADDRESS_SQL_COL00 " = (:_v00)");
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
      user.m_bAccessPerson = query.value(10).toBool();
      user.m_bAccessCategory = query.value(11).toBool();
      user.m_bAccessImage = query.value(12).toBool();
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
  query.prepare("DELETE FROM " USER_SQL_TABLE_NAME
                " WHERE " USER_SQL_COL00 " = (:_v00)");
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
                USER_SQL_COL00 ","
                USER_SQL_COL01 ","
                USER_SQL_COL02 ","
                USER_SQL_COL03 ","
                USER_SQL_COL04 ","
                USER_SQL_COL05 ","
                USER_SQL_COL06 ","
                USER_SQL_COL07 ","
                USER_SQL_COL08 ","
                USER_SQL_COL09 ","
                USER_SQL_COL10 ","
                USER_SQL_COL11 ","
                USER_SQL_COL12 ","
                USER_SQL_COL13
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " USER_SQL_COL01 " = (:_v01) AND "
                USER_SQL_COL02 " = (:_v02) LIMIT 1");
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
      m_user.m_bAccessPerson = query.value(11).toBool();
      m_user.m_bAccessCategory = query.value(12).toBool();
      m_user.m_bAccessImage = query.value(13).toBool();
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
                           Person& person,
                           QString& error)
{
  error.clear();
  qlonglong id = person.m_id;
  person.clear();

  query.prepare("SELECT "
                PERSON_SQL_COL01 ","
                PERSON_SQL_COL02 ","
                PERSON_SQL_COL03 ","
                PERSON_SQL_COL04 ","
                PERSON_SQL_COL05 ","
                PERSON_SQL_COL06 ","
                PERSON_SQL_COL07 ","
                PERSON_SQL_COL08 ","
                PERSON_SQL_COL09 ","
                PERSON_SQL_COL10 ","
                PERSON_SQL_COL11 ","
                PERSON_SQL_COL12 ","
                PERSON_SQL_COL13 ","
                PERSON_SQL_COL14
                " FROM " PERSON_SQL_TABLE_NAME
                " WHERE " PERSON_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      person.m_id = id;
      person.m_image.m_id = query.value(0).toLongLong();
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
      person.m_employeePinCode = query.value(13).toString();
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
                  ADDRESS_SQL_COL00 ","
                  ADDRESS_SQL_COL02 ","
                  ADDRESS_SQL_COL03 ","
                  ADDRESS_SQL_COL04 ","
                  ADDRESS_SQL_COL05 ","
                  ADDRESS_SQL_COL06 ","
                  ADDRESS_SQL_COL07 ","
                  ADDRESS_SQL_COL08 ","
                  ADDRESS_SQL_COL09
                  " FROM " ADDRESS_SQL_TABLE_NAME
                  " WHERE " ADDRESS_SQL_COL01 " = (:_v01)");
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
      person.m_vAddress.push_back(address);
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  PHONE_SQL_COL00 ","
                  PHONE_SQL_COL01 ","
                  PHONE_SQL_COL02 ","
                  PHONE_SQL_COL03 ","
                  PHONE_SQL_COL04
                  " FROM " PHONE_SQL_TABLE_NAME
                  " WHERE " PHONE_SQL_COL01 " = (:_v01)");
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
      person.m_vPhone.push_back(phone);
    }
  }

  if (bSuccess && person.m_image.isValidId())
    bSuccess = ImageSQL::execSelect(query, person.m_image, error);

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    person.clear();
  }

  return bSuccess;
}

bool PersonSQL::execByPinCodeSelect(QSqlQuery& query,
                                    const QString& pincode,
                                    Person& person,
                                    QString& error)
{
  person.clear();
  error.clear();

  query.prepare("SELECT "
                PERSON_SQL_COL00
                " FROM " PERSON_SQL_TABLE_NAME
                " WHERE " PERSON_SQL_COL14 " = (:_v14) AND "
                PERSON_SQL_COL13 " <> 0 AND "
                PERSON_SQL_COL13 " IS NOT NULL");
  query.bindValue(":_v14", pincode);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      person.m_id = query.value(0).toLongLong();
      bSuccess = execSelect(query, person, error);
    }
    else
    {
      error = "Código PIN informado não encontrado.";
      bSuccess = false;
    }
  }

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    person.clear();
  }

  return bSuccess;
}

bool PersonSQL::select(QSqlDatabase db,
                       Person& person,
                       QString& error)
{
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, person, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    person.clear();
  return bSuccess;
}

bool PersonSQL::insert(QSqlDatabase db,
                       const Person& person,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " PERSON_SQL_TABLE_NAME " ("
                PERSON_SQL_COL02 ","
                PERSON_SQL_COL03 ","
                PERSON_SQL_COL04 ","
                PERSON_SQL_COL05 ","
                PERSON_SQL_COL06 ","
                PERSON_SQL_COL07 ","
                PERSON_SQL_COL08 ","
                PERSON_SQL_COL09 ","
                PERSON_SQL_COL10 ","
                PERSON_SQL_COL11 ","
                PERSON_SQL_COL12 ","
                PERSON_SQL_COL13 ","
                PERSON_SQL_COL14  ")"
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
    if (person.m_image.isValidId())
    {
      query.prepare("UPDATE " PERSON_SQL_TABLE_NAME " SET "
                    PERSON_SQL_COL01 " = (:_v01)"
                    " WHERE " PERSON_SQL_COL00 " = (:_v00)");
      query.bindValue(":_v00", person.m_id);
      query.bindValue(":_v01", person.m_image.m_id);
      bSuccess = query.exec();
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != person.m_vPhone.size(); ++i)
    {
      query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                    PHONE_SQL_COL01 ","
                    PHONE_SQL_COL02 ","
                    PHONE_SQL_COL03 ","
                    PHONE_SQL_COL04 ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04))");
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", person.m_vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", person.m_vPhone.at(i).m_code);
      query.bindValue(":_v04", person.m_vPhone.at(i).m_number);
      bSuccess = query.exec();
      if (bSuccess)
        person.m_vPhone.at(i).m_id = query.lastInsertId().toLongLong();
      else
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != person.m_vAddress.size(); ++i)
    {
      query.prepare("INSERT INTO " ADDRESS_SQL_TABLE_NAME " ("
                    ADDRESS_SQL_COL01 ","
                    ADDRESS_SQL_COL02 ","
                    ADDRESS_SQL_COL03 ","
                    ADDRESS_SQL_COL04 ","
                    ADDRESS_SQL_COL05 ","
                    ADDRESS_SQL_COL06 ","
                    ADDRESS_SQL_COL07 ","
                    ADDRESS_SQL_COL08 ","
                    ADDRESS_SQL_COL09 ")"
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
      query.bindValue(":_v02", person.m_vAddress.at(i).m_cep);
      query.bindValue(":_v03", person.m_vAddress.at(i).m_neighborhood);
      query.bindValue(":_v04", person.m_vAddress.at(i).m_street);
      query.bindValue(":_v05", person.m_vAddress.at(i).m_number);
      query.bindValue(":_v06", person.m_vAddress.at(i).m_city);
      query.bindValue(":_v07", (int)person.m_vAddress.at(i).m_state);
      query.bindValue(":_v08", person.m_vAddress.at(i).m_complement);
      query.bindValue(":_v09", person.m_vAddress.at(i).m_reference);
      bSuccess = query.exec();
      if (bSuccess)
        person.m_vAddress.at(i).m_id = query.lastInsertId().toLongLong();
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
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);

  query.prepare("UPDATE " PERSON_SQL_TABLE_NAME " SET "
                PERSON_SQL_COL02 " = (:_v02),"
                PERSON_SQL_COL03 " = (:_v03),"
                PERSON_SQL_COL04 " = (:_v04),"
                PERSON_SQL_COL05 " = (:_v05),"
                PERSON_SQL_COL06 " = (:_v06),"
                PERSON_SQL_COL07 " = (:_v07),"
                PERSON_SQL_COL08 " = (:_v08),"
                PERSON_SQL_COL09 " = (:_v09),"
                PERSON_SQL_COL10 " = (:_v10),"
                PERSON_SQL_COL11 " = (:_v11),"
                PERSON_SQL_COL12 " = (:_v12),"
                PERSON_SQL_COL13 " = (:_v13),"
                PERSON_SQL_COL14 " = (:_v14)"
                " WHERE " PERSON_SQL_COL00 " = (:_v00)");
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

  if (bSuccess && person.m_image.isValidId())
  {
    query.prepare("UPDATE " PERSON_SQL_TABLE_NAME " SET "
                  PERSON_SQL_COL01 " = (:_v01)"
                                   " WHERE " PERSON_SQL_COL00 " = (:_v00)");
    query.bindValue(":_v00", person.m_id);
    query.bindValue(":_v01", person.m_image.m_id);
    bSuccess = query.exec();
  }

  if (bSuccess && !person.m_vAddress.isEmpty())
  {
    QString str = "DELETE FROM " ADDRESS_SQL_TABLE_NAME
                  " WHERE " ADDRESS_SQL_COL01 " = (:_v01)"
                  " AND " ADDRESS_SQL_COL00 " NOT IN (";
    for (int i = 0; i != person.m_vAddress.size(); ++i)
      str += "(:_v00" + QString::number(i) + "),";
    str.replace(str.length() - 1, 1, ")");
    query.prepare(str);
    query.bindValue(":_v01", person.m_id);
    for (int i = 0; i != person.m_vAddress.size(); ++i)
      query.bindValue("_v00" + QString::number(i), person.m_vAddress.at(i).m_id);
    bSuccess = query.exec();
  }
  else if (!person.m_vAddress.isEmpty())
  {
    query.prepare("DELETE FROM " ADDRESS_SQL_TABLE_NAME " WHERE " ADDRESS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", person.m_id);
    bSuccess = query.exec();
  }

  if (bSuccess && !person.m_vPhone.isEmpty())
  {
    QString str = "DELETE FROM " PHONE_SQL_TABLE_NAME
                  " WHERE " PHONE_SQL_COL01 " = (:_v01)"
                  " AND " PHONE_SQL_COL00 " NOT IN (";
    for (int i = 0; i != person.m_vPhone.size(); ++i)
      str += "(:_v00" + QString::number(i) + "),";
    str.replace(str.length() - 1, 1, ")");
    query.prepare(str);
    query.bindValue(":_v01", person.m_id);
    for (int i = 0; i != person.m_vPhone.size(); ++i)
      query.bindValue("_v00" + QString::number(i), person.m_vPhone.at(i).m_id);
    bSuccess = query.exec();
  }
  else if (!person.m_vPhone.isEmpty())
  {
    query.prepare("DELETE FROM " PHONE_SQL_TABLE_NAME " WHERE " PHONE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", person.m_id);
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != person.m_vPhone.size(); ++i)
    {
      query.prepare("UPDATE " PHONE_SQL_TABLE_NAME " SET "
                    PHONE_SQL_COL01 " = (:_v01),"
                    PHONE_SQL_COL02 " = (:_v02),"
                    PHONE_SQL_COL03 " = (:_v03),"
                    PHONE_SQL_COL04 " = (:_v04) "
                    " WHERE " PHONE_SQL_COL00 " = (:_v00)");
      query.bindValue(":_v00", person.m_vPhone.at(i).m_id);
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", person.m_vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", person.m_vPhone.at(i).m_code);
      query.bindValue(":_v04", person.m_vPhone.at(i).m_number);
      bSuccess = query.exec();
      if (!bSuccess)
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != person.m_vAddress.size(); ++i)
    {
      query.prepare("UPDATE " ADDRESS_SQL_TABLE_NAME " SET "
                    ADDRESS_SQL_COL01 " = (:_v01),"
                    ADDRESS_SQL_COL02 " = (:_v02),"
                    ADDRESS_SQL_COL03 " = (:_v03),"
                    ADDRESS_SQL_COL04 " = (:_v04),"
                    ADDRESS_SQL_COL05 " = (:_v05),"
                    ADDRESS_SQL_COL06 " = (:_v06),"
                    ADDRESS_SQL_COL07 " = (:_v07),"
                    ADDRESS_SQL_COL08 " = (:_v08),"
                    ADDRESS_SQL_COL09 " = (:_v09) "
                    "WHERE " ADDRESS_SQL_COL00 " = (:_v00)");
      query.bindValue(":_v00", person.m_vAddress.at(i).m_id);
      query.bindValue(":_v01", person.m_id);
      query.bindValue(":_v02", person.m_vAddress.at(i).m_cep);
      query.bindValue(":_v03", person.m_vAddress.at(i).m_neighborhood);
      query.bindValue(":_v04", person.m_vAddress.at(i).m_street);
      query.bindValue(":_v05", person.m_vAddress.at(i).m_number);
      query.bindValue(":_v06", person.m_vAddress.at(i).m_city);
      query.bindValue(":_v07", (int)person.m_vAddress.at(i).m_state);
      query.bindValue(":_v08", person.m_vAddress.at(i).m_complement);
      query.bindValue(":_v09", person.m_vAddress.at(i).m_reference);
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
  query.prepare("DELETE FROM " PERSON_SQL_TABLE_NAME " WHERE " PERSON_SQL_COL00 " = (:_v00)");
  query.bindValue(":_v00", id);
  bool bSuccess = query.exec();
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

bool PersonSQL::isValidPinCode(QSqlDatabase db,
                               const QString& pincode,
                               Person& person,
                               QString& error)
{
  if (!BaitaSQL::isOpen(db, error))
    return false;

  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execByPinCodeSelect(query, pincode, person, error);
  bSuccess = finishTransaction(db, bSuccess, error);
  if (!bSuccess)
    person.clear();
  return bSuccess;
}
