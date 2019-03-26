#include "databaseutils.h"
#include "settings.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>
#include <QHostInfo>

#define DATABASE_NOT_OPEN_TXT "O banco de dados não foi aberto."

namespace {
  bool finishTransaction(QSqlDatabase db,
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
}

bool BaitaSQL::isOpen(QString& error)
{
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  error.clear();
  if (!db.isOpen())
     error = "Banco de dados não foi aberto.";
  return db.isOpen();
}

bool BaitaSQL::open(const QString& hostName,
                    int port,
                    QString& error)
{
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  error.clear();
  if (db.isOpen())
    db.close();
  db.setPort(port);
  db.setHostName(hostName);
  db.setDatabaseName("BaitaAssistente");
  db.setUserName("BaitaAssistente");
  db.setPassword("jfljfl");
  bool bSuccess = db.open();
  if (!bSuccess)
    error = db.lastError().text();
  return bSuccess;
}

void BaitaSQL::close()
{
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.close();
}

bool BaitaSQL::init(const QString& hostName,
                    int port,
                    QString& error)
{
  QSqlDatabase db = QSqlDatabase::database(POSTGRE_CONNECTION_NAME);
  bool bSuccess = db.isValid();
  if (bSuccess)
  {
    if (db.isOpen())
      db.close();
    bSuccess = BaitaSQL::open(hostName, port, error);
    if (bSuccess)
    {
      bSuccess = BaitaSQL::createTables(error);
      if (!bSuccess)
        db.close();
    }
  }
  else
  {
    error = "Driver not loaded.";
  }
  return bSuccess;
}

QDate BaitaSQL::getDate(bool dfltMax)
{
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);
  if (query.exec("SELECT current_date") && query.next())
    return query.value(0).toDate();
  return QDate::fromJulianDay(dfltMax ? Q_INT64_C(784354017364) : Q_INT64_C(-784350574879));
}

bool BaitaSQL::createTables(QString& error)
{
  error.clear();

  if (!isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = true;

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " IMAGE_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          IMAGE_SQL_COL01 " TEXT NOT NULL UNIQUE,"
                          IMAGE_SQL_COL02 " BYTEA)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " CATEGORY_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          CATEGORY_SQL_COL01 " INTEGER,"
                          CATEGORY_SQL_COL02 " TEXT NOT NULL UNIQUE,"
                          "FOREIGN KEY(" CATEGORY_SQL_COL01 ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " REMINDER_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          REMINDER_SQL_COL01 " TEXT,"
                          REMINDER_SQL_COL02 " TEXT,"
                          REMINDER_SQL_COL03 " BOOLEAN,"
                          REMINDER_SQL_COL04 " INTEGER,"
                          REMINDER_SQL_COL05 " INTEGER,"
                          REMINDER_SQL_COL06 " BOOLEAN,"
                          REMINDER_SQL_COL07 " TEXT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " USER_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          USER_SQL_COL01 " TEXT NOT NULL UNIQUE,"
                          USER_SQL_COL02 " TEXT NOT NULL,"
                          USER_SQL_COL03 " BOOLEAN,"
                          USER_SQL_COL04 " BOOLEAN,"
                          USER_SQL_COL05 " BOOLEAN,"
                          USER_SQL_COL06 " BOOLEAN,"
                          USER_SQL_COL07 " BOOLEAN,"
                          USER_SQL_COL08 " BOOLEAN,"
                          USER_SQL_COL09 " BOOLEAN,"
                          USER_SQL_COL10 " BOOLEAN,"
                          USER_SQL_COL11 " BOOLEAN,"
                          USER_SQL_COL12 " BOOLEAN,"
                          USER_SQL_COL13 " BOOLEAN,"
                          USER_SQL_COL14 " BOOLEAN,"
                          USER_SQL_COL15 " BOOLEAN,"
                          USER_SQL_COL16 " BOOLEAN)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PRODUCT_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        PRODUCT_SQL_COL01 " TEXT NOT NULL UNIQUE,"
                        PRODUCT_SQL_COL02 " INTEGER,"
                        PRODUCT_SQL_COL03 " INTEGER,"
                        PRODUCT_SQL_COL04 " TEXT NOT NULL,"
                        PRODUCT_SQL_COL05 " TEXT,"
                        PRODUCT_SQL_COL06 " BOOLEAN,"
                        PRODUCT_SQL_COL07 " BOOLEAN,"
                        "FOREIGN KEY(" PRODUCT_SQL_COL02 ") REFERENCES "
                        CATEGORY_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                        "FOREIGN KEY(" PRODUCT_SQL_COL03 ") REFERENCES "
                        IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PERSON_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PERSON_SQL_COL01 " INTEGER,"
                          PERSON_SQL_COL02 " TEXT NOT NULL UNIQUE,"
                          PERSON_SQL_COL03 " TEXT,"
                          PERSON_SQL_COL04 " TEXT,"
                          PERSON_SQL_COL05 " TEXT,"
                          PERSON_SQL_COL06 " TEXT,"
                          PERSON_SQL_COL07 " TEXT,"
                          PERSON_SQL_COL08 " DATE,"
                          PERSON_SQL_COL09 " DATE,"
                          PERSON_SQL_COL10 " BOOLEAN,"
                          PERSON_SQL_COL11 " BOOLEAN,"
                          "FOREIGN KEY(" PERSON_SQL_COL01 ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " ADDRESS_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        ADDRESS_SQL_COL01 " INTEGER,"
                        ADDRESS_SQL_COL02 " TEXT,"
                        ADDRESS_SQL_COL03 " TEXT,"
                        ADDRESS_SQL_COL04 " TEXT,"
                        ADDRESS_SQL_COL05 " INTEGER,"
                        ADDRESS_SQL_COL06 " TEXT,"
                        ADDRESS_SQL_COL07 " INTEGER,"
                        ADDRESS_SQL_COL08 " TEXT,"
                        ADDRESS_SQL_COL09 " TEXT,"
                        "FOREIGN KEY(" ADDRESS_SQL_COL01 ") REFERENCES "
                        PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PHONE_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        PHONE_SQL_COL01 " INTEGER,"
                        PHONE_SQL_COL02 " INTEGER DEFAULT " PHONE_DEFAULT_COUNTRY_CODE_VALUE_STR ","
                        PHONE_SQL_COL03 " INTEGER DEFAULT " PHONE_DEFAULT_CODE_VALUE_STR ","
                        PHONE_SQL_COL04 " TEXT,"
                        PHONE_SQL_COL05 " TEXT,"
                        "FOREIGN KEY(" PHONE_SQL_COL01 ") REFERENCES "
                        PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " STORE_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          STORE_SQL_COL01 " INTEGER,"
                          STORE_SQL_COL02 " INTEGER,"
                          STORE_SQL_COL03 " INTEGER,"
                          STORE_SQL_COL04 " TEXT,"
                          "FOREIGN KEY(" STORE_SQL_COL01 ") REFERENCES "
                          PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                          "FOREIGN KEY(" STORE_SQL_COL02 ") REFERENCES "
                          ADDRESS_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                          "FOREIGN KEY(" STORE_SQL_COL03 ") REFERENCES "
                          PHONE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          STORE_EMPLOYEES_SQL_COL01 " INTEGER NOT NULL,"
                          STORE_EMPLOYEES_SQL_COL02 " INTEGER NOT NULL,"
                          "FOREIGN KEY(" STORE_EMPLOYEES_SQL_COL01 ") REFERENCES "
                          STORE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" STORE_EMPLOYEES_SQL_COL02 ") REFERENCES "
                          EMPLOYEE_SQL_TABLE_NAME "(" EMPLOYEE_SQL_COL01 ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                          STORE_EMPLOYEE_HOURS_SQL_COL01 " INTEGER NOT NULL,"
                          STORE_EMPLOYEE_HOURS_SQL_COL02 " INTEGER,"
                          STORE_EMPLOYEE_HOURS_SQL_COL03 " TIME,"
                          STORE_EMPLOYEE_HOURS_SQL_COL04 " TIME,"
                          "FOREIGN KEY(" STORE_EMPLOYEE_HOURS_SQL_COL01 ") REFERENCES "
                          STORE_EMPLOYEES_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " EMPLOYEE_SQL_TABLE_NAME " ("
                          EMPLOYEE_SQL_COL01 " INTEGER PRIMARY KEY,"
                          EMPLOYEE_SQL_COL02 " TEXT UNIQUE,"
                          EMPLOYEE_SQL_COL03 " BOOLEAN,"
                          EMPLOYEE_SQL_COL04 " BOOLEAN,"
                          "FOREIGN KEY(" EMPLOYEE_SQL_COL01 ") REFERENCES "
                          PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SUPPLIER_SQL_TABLE_NAME " ("
                          SUPPLIER_SQL_COL01 " INTEGER PRIMARY KEY,"
                          "FOREIGN KEY(" SUPPLIER_SQL_COL01 ") REFERENCES "
                          PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " NOTE_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        NOTE_SQL_COL01 " INTEGER UNIQUE NOT NULL,"
                        NOTE_SQL_COL02 " DATE NOT NULL,"
                        NOTE_SQL_COL03 " INTEGER,"
                        NOTE_SQL_COL04 " BOOLEAN,"
                        NOTE_SQL_COL05 " TEXT,"
                        NOTE_SQL_COL06 " REAL,"
                        "FOREIGN KEY(" NOTE_SQL_COL03 ") REFERENCES "
                        PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " NOTE_ITEMS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          NOTE_ITEMS_SQL_COL01 " INTEGER NOT NULL,"
                          NOTE_ITEMS_SQL_COL02 " INTEGER,"
                          NOTE_ITEMS_SQL_COL03 " REAL,"
                          NOTE_ITEMS_SQL_COL04 " REAL,"
                          NOTE_ITEMS_SQL_COL05 " BOOLEAN,"
                          NOTE_ITEMS_SQL_COL06 " TEXT,"
                          NOTE_ITEMS_SQL_COL07 " REAL,"
                          "FOREIGN KEY(" NOTE_ITEMS_SQL_COL01 ") REFERENCES "
                          NOTE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" NOTE_ITEMS_SQL_COL02 ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SHOPPING_LIST_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          SHOPPING_LIST_SQL_COL01 " INTEGER,"
                          SHOPPING_LIST_SQL_COL02 " INTEGER,"
                          SHOPPING_LIST_SQL_COL03 " TEXT NOT NULL,"
                          SHOPPING_LIST_SQL_COL04 " TEXT,"
                          SHOPPING_LIST_SQL_COL05 " TEXT,"
                          SHOPPING_LIST_SQL_COL06 " TEXT,"
                          SHOPPING_LIST_SQL_COL07 " INTEGER,"
                          "FOREIGN KEY(" SHOPPING_LIST_SQL_COL01 ") REFERENCES "
                          PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                          "FOREIGN KEY(" SHOPPING_LIST_SQL_COL02 ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          SHOPPING_LIST_ITEMS_SQL_COL01 " INTEGER NOT NULL,"
                          SHOPPING_LIST_ITEMS_SQL_COL02 " INTEGER NOT NULL,"
                          SHOPPING_LIST_ITEMS_SQL_COL03 " REAL,"
                          SHOPPING_LIST_ITEMS_SQL_COL04 " REAL,"
                          SHOPPING_LIST_ITEMS_SQL_COL05 " BOOLEAN,"
                          SHOPPING_LIST_ITEMS_SQL_COL06 " TEXT,"
                          SHOPPING_LIST_ITEMS_SQL_COL07 " REAL,"
                          SHOPPING_LIST_ITEMS_SQL_COL08 " BOOLEAN,"
                          SHOPPING_LIST_ITEMS_SQL_COL09 " BOOLEAN,"
                          SHOPPING_LIST_ITEMS_SQL_COL10 " INTEGER,"
                          "FOREIGN KEY(" SHOPPING_LIST_ITEMS_SQL_COL01 ") REFERENCES "
                          SHOPPING_LIST_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" SHOPPING_LIST_ITEMS_SQL_COL02 ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" SHOPPING_LIST_ITEMS_SQL_COL10 ") REFERENCES "
                          PERSON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " RESERVATION_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          RESERVATION_SQL_COL01 " INTEGER,"
                          RESERVATION_SQL_COL02 " TEXT,"
                          RESERVATION_SQL_COL03 " TEXT,"
                          RESERVATION_SQL_COL04 " DATE,"
                          RESERVATION_SQL_COL05 " INTEGER,"
                          RESERVATION_SQL_COL06 " TEXT,"
                          RESERVATION_SQL_COL07 " TEXT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PRODUCT_CODE_ITEMS_SQL_COL01 " INTEGER NOT NULL,"
                          PRODUCT_CODE_ITEMS_SQL_COL02 " TEXT UNIQUE NOT NULL CHECK ("
                          PRODUCT_CODE_ITEMS_SQL_COL02 " <> ''),"
                          "FOREIGN KEY(" PRODUCT_CODE_ITEMS_SQL_COL01 ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " ACTIVE_USERS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          ACTIVE_USERS_SQL_COL01 " INTEGER,"
                          ACTIVE_USERS_SQL_COL02 " TEXT,"
                          ACTIVE_USERS_SQL_COL03 " TEXT,"
                          ACTIVE_USERS_SQL_COL04 " TIMESTAMP)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " DISCOUNT_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          DISCOUNT_SQL_COL01 " TEXT UNIQUE NOT NULL CHECK ("
                          DISCOUNT_SQL_COL01 " <> ''),"
                          DISCOUNT_SQL_COL02 " BOOLEAN,"
                          DISCOUNT_SQL_COL03 " DATE,"
                          DISCOUNT_SQL_COL04 " INTEGER,"
                          DISCOUNT_SQL_COL05 " REAL,"
                          DISCOUNT_SQL_COL06 " REAL CHECK ("
                          DISCOUNT_SQL_COL06 " BETWEEN 0 AND 100),"
                          DISCOUNT_SQL_COL07 " BOOLEAN,"
                          DISCOUNT_SQL_COL08 " TEXT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " DISCOUNT_ITEMS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          DISCOUNT_ITEMS_SQL_COL01 " INTEGER NOT NULL,"
                          DISCOUNT_ITEMS_SQL_COL02 " INTEGER NOT NULL,"
                          DISCOUNT_ITEMS_SQL_COL03 " REAL,"
                          "FOREIGN KEY(" DISCOUNT_ITEMS_SQL_COL01 ") REFERENCES "
                          DISCOUNT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" DISCOUNT_ITEMS_SQL_COL02 ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
  {
    query.exec("SELECT * FROM " USER_SQL_TABLE_NAME " LIMIT 1");
    if (!query.next())
    {
      QString str = "INSERT INTO " USER_SQL_TABLE_NAME " ("
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
                    USER_SQL_COL13 ","
                    USER_SQL_COL14 ","
                    USER_SQL_COL15 ","
                    USER_SQL_COL16 ")"
                    " VALUES ('"
                    USER_SQL_DEFAULT_NAME "',"
                    ":_password,"
                    "TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);";
      query.prepare(str);
      query.bindValue(":_password", User::st_strEncryptedPassword(USER_SQL_DEFAULT_PASSWORD));
      bSuccess = query.exec();
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool UserSQL::insert(const User& user,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
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
                USER_SQL_COL13 ","
                USER_SQL_COL14 ","
                USER_SQL_COL15 ","
                USER_SQL_COL16 ")"
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
                "(:_v13),"
                "(:_v14),"
                "(:_v15),"
                "(:_v16))");
  query.bindValue(":_v01", user.m_strUser);
  query.bindValue(":_v02", user.strEncryptedPassword());
  query.bindValue(":_v03", user.m_bAccessNote);
  query.bindValue(":_v04", user.m_bAccessReminder);
  query.bindValue(":_v05", user.m_bAccessCalculator);
  query.bindValue(":_v06", user.m_bAccessShop);
  query.bindValue(":_v07", user.m_bAccessUser);
  query.bindValue(":_v08", user.m_bAccessProduct);
  query.bindValue(":_v09", user.m_bAccessSettings);
  query.bindValue(":_v10", user.m_bAccessPerson);
  query.bindValue(":_v11", user.m_bAccessCategory);
  query.bindValue(":_v12", user.m_bAccessImage);
  query.bindValue(":_v13", user.m_bAccessReservation);
  query.bindValue(":_v14", user.m_bAccessShoppingList);
  query.bindValue(":_v15", user.m_bAccessEmployee);
  query.bindValue(":_v16", user.m_bAccessSupplier);

  bool bSuccess = query.exec();
  if (bSuccess)
    user.m_id.set(query.lastInsertId().toLongLong());

  return finishTransaction(db, query, bSuccess, error);
}

bool UserSQL::update(const User& user,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
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
              USER_SQL_COL13" = (:_v13),"
              USER_SQL_COL14" = (:_v14),"
              USER_SQL_COL15" = (:_v15),"
              USER_SQL_COL16" = (:_v16)"
              " WHERE " SQL_COLID " = (:_v00)";


  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
   db.transaction();
  QSqlQuery query(db);
  query.prepare(strQuery);
  query.bindValue(":_v00", user.m_id.get());
  query.bindValue(":_v01", user.m_strUser);
  if (!user.m_password.isEmpty())
    query.bindValue(":_v02", user.strEncryptedPassword());
  query.bindValue(":_v03", user.m_bAccessNote);
  query.bindValue(":_v04", user.m_bAccessReminder);
  query.bindValue(":_v05", user.m_bAccessCalculator);
  query.bindValue(":_v06", user.m_bAccessShop);
  query.bindValue(":_v07", user.m_bAccessUser);
  query.bindValue(":_v08", user.m_bAccessProduct);
  query.bindValue(":_v09", user.m_bAccessSettings);
  query.bindValue(":_v10", user.m_bAccessPerson);
  query.bindValue(":_v11", user.m_bAccessCategory);
  query.bindValue(":_v12", user.m_bAccessImage);
  query.bindValue(":_v13", user.m_bAccessReservation);
  query.bindValue(":_v14", user.m_bAccessShoppingList);
  query.bindValue(":_v15", user.m_bAccessEmployee);
  query.bindValue(":_v16", user.m_bAccessSupplier);

  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool UserSQL::select(User& user,
                     QString& error)
{
  error.clear();
  Id id = user.m_id;
  user.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
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
                USER_SQL_COL13 ","
                USER_SQL_COL14 ","
                USER_SQL_COL15 ","
                USER_SQL_COL16
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
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
      user.m_bAccessUser = query.value(6).toBool();
      user.m_bAccessProduct = query.value(7).toBool();
      user.m_bAccessSettings = query.value(8).toBool();
      user.m_bAccessPerson = query.value(9).toBool();
      user.m_bAccessCategory = query.value(10).toBool();
      user.m_bAccessImage = query.value(11).toBool();
      user.m_bAccessReservation = query.value(12).toBool();
      user.m_bAccessShoppingList = query.value(13).toBool();
      user.m_bAccessEmployee = query.value(14).toBool();
      user.m_bAccessSupplier = query.value(15).toBool();
    }
    else
    {
      bSuccess = false;
      error = "Usuário não encontrado.";
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool UserSQL::remove(Id id,
                     QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

UserLoginSQL::UserLoginSQL()
{

}

bool UserLoginSQL::login(const QString& strUser,
                         const QString& strPassword,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                SQL_COLID ","
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
                USER_SQL_COL13 ","
                USER_SQL_COL14 ","
                USER_SQL_COL15 ","
                USER_SQL_COL16
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " USER_SQL_COL01 " = (:_v01) AND "
                USER_SQL_COL02 " = (:_v02) LIMIT 1");
  query.bindValue(":_v01", strUser);
  query.bindValue(":_v02", User::st_strEncryptedPassword(strPassword));

  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_user.m_id.set(query.value(0).toLongLong());
      m_user.m_strUser = query.value(1).toString();
      query.value(2).toString(); // password nao precisamos
      m_user.m_bAccessNote = query.value(3).toBool();
      m_user.m_bAccessReminder = query.value(4).toBool();
      m_user.m_bAccessCalculator = query.value(5).toBool();
      m_user.m_bAccessShop = query.value(6).toBool();
      m_user.m_bAccessUser = query.value(7).toBool();
      m_user.m_bAccessProduct = query.value(8).toBool();
      m_user.m_bAccessSettings = query.value(9).toBool();
      m_user.m_bAccessPerson = query.value(10).toBool();
      m_user.m_bAccessCategory = query.value(11).toBool();
      m_user.m_bAccessImage = query.value(12).toBool();
      m_user.m_bAccessReservation = query.value(13).toBool();
      m_user.m_bAccessShoppingList = query.value(14).toBool();
      m_user.m_bAccessEmployee = query.value(15).toBool();
      m_user.m_bAccessSupplier = query.value(16).toBool();

      bSuccess = ActiveUserSQL::execRemove(query, error);
      if (bSuccess)
      {
        bSuccess = ActiveUserSQL::execRefresh(query, error);
        if (bSuccess)
        {
          query.prepare("SELECT " ACTIVE_USERS_SQL_COL02 ","
                        ACTIVE_USERS_SQL_COL03 " FROM "
                        ACTIVE_USERS_SQL_TABLE_NAME " WHERE "
                        ACTIVE_USERS_SQL_COL02 " = (:v02) LIMIT 1");
          query.bindValue(":v02", strUser);
          bSuccess = query.exec();
          if (bSuccess)
          {
            if (query.next())
            {
              bSuccess = false;
              error = "Usuário " +
                      strUser  +
                      " já logado na máquina " +
                      query.value(1).toString();
            }
            else
            {
              query.prepare("SELECT pg_backend_pid()");
              bSuccess = query.exec();
              if (bSuccess)
              {
                qlonglong pid = 0;
                if (query.next())
                  pid = query.value(0).toLongLong();
                QString strQuery = "INSERT INTO " ACTIVE_USERS_SQL_TABLE_NAME " ("
                                ACTIVE_USERS_SQL_COL01 ","
                                ACTIVE_USERS_SQL_COL02 ","
                                ACTIVE_USERS_SQL_COL03 ","
                                ACTIVE_USERS_SQL_COL04 ")"
                                " VALUES ("
                                "(:_v01),"
                                "(:_v02),"
                                "(:_v03),"
                                "current_timestamp)";
                query.prepare(strQuery);
                query.bindValue(":_v01", pid);
                query.bindValue(":_v02", strUser);
                query.bindValue(":_v03", QHostInfo::localHostName().toUpper());
                bSuccess = query.exec();
              }
            }
          }
        }
      }
    }
    else
    {
      bSuccess = false;
      error = "Usuário ou senha inválidos.";
    }
  }

  if (!bSuccess)
    m_user.clear();

  return finishTransaction(db, query, bSuccess, error);
}


bool ShoppingListSQL::insert(const ShoppingList& shoppingList,
                             QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " SHOPPING_LIST_SQL_TABLE_NAME " ("
                SHOPPING_LIST_SQL_COL01 ","
                SHOPPING_LIST_SQL_COL02 ","
                SHOPPING_LIST_SQL_COL03 ","
                SHOPPING_LIST_SQL_COL04 ","
                SHOPPING_LIST_SQL_COL05 ","
                SHOPPING_LIST_SQL_COL06 ","
                SHOPPING_LIST_SQL_COL07
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");


  query.bindValue(":_v01", shoppingList.m_supplier.m_id.getIdNull());
  query.bindValue(":_v02", shoppingList.m_image.m_id.getIdNull());
  query.bindValue(":_v03", shoppingList.m_title);
  query.bindValue(":_v04", shoppingList.m_description);
  query.bindValue(":_v05", shoppingList.getWeekDays());
  query.bindValue(":_v06", shoppingList.getMonthDays());
  query.bindValue(":_v07", shoppingList.m_nLines);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    shoppingList.m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != shoppingList.m_vItem.size(); ++i)
    {
      query.prepare("INSERT INTO " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " ("
                    SHOPPING_LIST_ITEMS_SQL_COL01 ","
                    SHOPPING_LIST_ITEMS_SQL_COL02 ","
                    SHOPPING_LIST_ITEMS_SQL_COL03 ","
                    SHOPPING_LIST_ITEMS_SQL_COL04 ","
                    SHOPPING_LIST_ITEMS_SQL_COL05 ","
                    SHOPPING_LIST_ITEMS_SQL_COL06 ","
                    SHOPPING_LIST_ITEMS_SQL_COL07 ","
                    SHOPPING_LIST_ITEMS_SQL_COL08 ","
                    SHOPPING_LIST_ITEMS_SQL_COL09 ","
                    SHOPPING_LIST_ITEMS_SQL_COL10
                    ") VALUES ("
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

      query.bindValue(":_v01", shoppingList.m_id.get());
      query.bindValue(":_v02", shoppingList.m_vItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", shoppingList.m_vItem.at(i).m_ammount);
      query.bindValue(":_v04", shoppingList.m_vItem.at(i).m_price);
      query.bindValue(":_v05", shoppingList.m_vItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", shoppingList.m_vItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", shoppingList.m_vItem.at(i).m_package.m_ammount);
      query.bindValue(":_v08", shoppingList.m_vItem.at(i).m_bAmmount);
      query.bindValue(":_v09", shoppingList.m_vItem.at(i).m_bPrice);
      query.bindValue(":_v10", shoppingList.m_vItem.at(i).m_supplier.m_id.getIdNull());
      bSuccess = query.exec();
      if (bSuccess)
        shoppingList.m_vItem.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool ShoppingListSQL::update(const ShoppingList& shoppingList,
                             QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " SHOPPING_LIST_SQL_TABLE_NAME " SET "
                SHOPPING_LIST_SQL_COL01 " = (:_v01),"
                SHOPPING_LIST_SQL_COL02 " = (:_v02),"
                SHOPPING_LIST_SQL_COL03 " = (:_v03),"
                SHOPPING_LIST_SQL_COL04 " = (:_v04),"
                SHOPPING_LIST_SQL_COL05 " = (:_v05),"
                SHOPPING_LIST_SQL_COL06 " = (:_v06),"
                SHOPPING_LIST_SQL_COL07 " = (:_v07) "
                "WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", shoppingList.m_id.get());
  query.bindValue(":_v01", shoppingList.m_supplier.m_id.getIdNull());
  query.bindValue(":_v02", shoppingList.m_image.m_id.getIdNull());
  query.bindValue(":_v03", shoppingList.m_title);
  query.bindValue(":_v04", shoppingList.m_description);
  query.bindValue(":_v05", shoppingList.getWeekDays());
  query.bindValue(":_v06", shoppingList.getMonthDays());
  query.bindValue(":_v07", shoppingList.m_nLines);
  bool bSuccess = query.exec();

  query.prepare("DELETE FROM " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " WHERE " SHOPPING_LIST_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", shoppingList.m_id.get());
  bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != shoppingList.m_vItem.size(); ++i)
    {
      query.prepare("INSERT INTO " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " ("
                    SHOPPING_LIST_ITEMS_SQL_COL01 ","
                    SHOPPING_LIST_ITEMS_SQL_COL02 ","
                    SHOPPING_LIST_ITEMS_SQL_COL03 ","
                    SHOPPING_LIST_ITEMS_SQL_COL04 ","
                    SHOPPING_LIST_ITEMS_SQL_COL05 ","
                    SHOPPING_LIST_ITEMS_SQL_COL06 ","
                    SHOPPING_LIST_ITEMS_SQL_COL07 ","
                    SHOPPING_LIST_ITEMS_SQL_COL08 ","
                    SHOPPING_LIST_ITEMS_SQL_COL09 ","
                    SHOPPING_LIST_ITEMS_SQL_COL10
                    ") VALUES ("
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
      query.bindValue(":_v01", shoppingList.m_id.get());
      query.bindValue(":_v02", shoppingList.m_vItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", shoppingList.m_vItem.at(i).m_ammount);
      query.bindValue(":_v04", shoppingList.m_vItem.at(i).m_price);
      query.bindValue(":_v05", shoppingList.m_vItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", shoppingList.m_vItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", shoppingList.m_vItem.at(i).m_package.m_ammount);
      query.bindValue(":_v08", shoppingList.m_vItem.at(i).m_bAmmount);
      query.bindValue(":_v09", shoppingList.m_vItem.at(i).m_bPrice);
      query.bindValue(":_v10", shoppingList.m_vItem.at(i).m_supplier.m_id.getIdNull());
      bSuccess = query.exec();
      if (bSuccess)
        shoppingList.m_vItem.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool ShoppingListSQL::select(ShoppingList& shoppingList,
                             QString& error)
{
  error.clear();
  Id id = shoppingList.m_id;
  shoppingList.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("SELECT "
                SHOPPING_LIST_SQL_COL01 ","
                SHOPPING_LIST_SQL_COL02 ","
                SHOPPING_LIST_SQL_COL03 ","
                SHOPPING_LIST_SQL_COL04 ","
                SHOPPING_LIST_SQL_COL05 ","
                SHOPPING_LIST_SQL_COL06 ","
                SHOPPING_LIST_SQL_COL07
                " FROM " SHOPPING_LIST_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      shoppingList.m_id = id;
      shoppingList.m_supplier.m_id.set(query.value(0).toLongLong());
      shoppingList.m_image.m_id.set(query.value(1).toLongLong());
      shoppingList.m_title = query.value(2).toString();
      shoppingList.m_description = query.value(3).toString();
      shoppingList.setWeekDays(query.value(4).toString());
      shoppingList.setMonthDays(query.value(5).toString());
      shoppingList.m_nLines = query.value(6).toInt();
    }
    else
    {
      error = "Lista não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  SHOPPING_LIST_ITEMS_SQL_COL02 ","
                  SHOPPING_LIST_ITEMS_SQL_COL03 ","
                  SHOPPING_LIST_ITEMS_SQL_COL04 ","
                  SHOPPING_LIST_ITEMS_SQL_COL05 ","
                  SHOPPING_LIST_ITEMS_SQL_COL06 ","
                  SHOPPING_LIST_ITEMS_SQL_COL07 ","
                  SHOPPING_LIST_ITEMS_SQL_COL08 ","
                  SHOPPING_LIST_ITEMS_SQL_COL09 ","
                  SHOPPING_LIST_ITEMS_SQL_COL10
                  " FROM " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME
                  " WHERE " SHOPPING_LIST_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", shoppingList.m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (bSuccess && query.next())
      {
        ShoppingListItem item;
        item.m_id.set(query.value(0).toLongLong());
        item.m_product.m_id.set(query.value(1).toLongLong());
        item.m_ammount = query.value(2).toDouble();
        item.m_price = query.value(3).toDouble();
        item.m_package.m_bIsPackage = query.value(4).toBool();
        item.m_package.m_unity = query.value(5).toString();
        item.m_package.m_ammount = query.value(6).toDouble();
        item.m_bAmmount= query.value(7).toDouble();
        item.m_bPrice = query.value(8).toDouble();
        item.m_supplier.m_id.set(query.value(9).toDouble());
        shoppingList.m_vItem.push_back(item);
      }
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != shoppingList.m_vItem.size(); ++i)
    {
      if (shoppingList.m_vItem.at(i).m_supplier.m_id.isValid())
        bSuccess = PersonSQL::execSelect(query, shoppingList.m_vItem[i].m_supplier, error);
      if (!bSuccess)
        break;
      if (shoppingList.m_vItem.at(i).m_product.m_id.isValid())
        bSuccess = ProductSQL::execSelect(query, shoppingList.m_vItem[i].m_product, error);
      if (!bSuccess)
        break;
    }
  }

  if (bSuccess && shoppingList.m_supplier.m_id.isValid())
    bSuccess = PersonSQL::execSelect(query, shoppingList.m_supplier, error);

  if (bSuccess && shoppingList.m_image.m_id.isValid())
    bSuccess = ImageSQL::execSelect(query, shoppingList.m_image, error);

  return finishTransaction(db, query, bSuccess, error);
}

bool ShoppingListSQL::remove(Id id,
                             QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " SHOPPING_LIST_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool ActiveUserSQL::refresh(QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execRefresh(query, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool ActiveUserSQL::execRefresh(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("DELETE FROM " ACTIVE_USERS_SQL_TABLE_NAME
                " WHERE " ACTIVE_USERS_SQL_COL01 " NOT IN "
                "(SELECT pid FROM pg_stat_activity)");
  return query.exec();
}

bool ActiveUserSQL::remove(QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execRemove(query, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool ActiveUserSQL::execRemove(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("DELETE FROM " ACTIVE_USERS_SQL_TABLE_NAME
                " WHERE " ACTIVE_USERS_SQL_COL01 " = pg_backend_pid()");
  return query.exec();
}
