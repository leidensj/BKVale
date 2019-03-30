#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QHostInfo>
#include <QDate>

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
<<<<<<< HEAD

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " PRODUCT_SQL_TABLE_NAME " SET "
                PRODUCT_SQL_COL01 " = (:_v01),"
                PRODUCT_SQL_COL02 " = (:_v02),"
                PRODUCT_SQL_COL03 " = (:_v03),"
                PRODUCT_SQL_COL04 " = (:_v04),"
                PRODUCT_SQL_COL05 " = (:_v05),"
                PRODUCT_SQL_COL06 " = (:_v06),"
                PRODUCT_SQL_COL07 " = (:_v07)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", product.m_id.get());
  query.bindValue(":_v01", product.m_name);
  query.bindValue(":_v02", product.m_category.m_id.getIdNull());
  query.bindValue(":_v03", product.m_image.m_id.getIdNull());
  query.bindValue(":_v04", product.m_unity);
  query.bindValue(":_v05", product.m_details);
  query.bindValue(":_v06", product.m_bBuy);
  query.bindValue(":_v07", product.m_bSell);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                  " WHERE " PRODUCT_CODE_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", product.m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      for (int i = 0; i != product.m_vCode.size(); ++i)
      {
        query.prepare("INSERT INTO " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME " ("
                      PRODUCT_CODE_ITEMS_SQL_COL01 ","
                      PRODUCT_CODE_ITEMS_SQL_COL02 ")"
                      " VALUES ("
                      "(:_v01),"
                      "(:_v02))");
        query.bindValue(":_v01", product.m_id.get());
        query.bindValue(":_v02", product.m_vCode.at(i).m_code);
        bSuccess = query.exec();
        if (bSuccess)
          product.m_vCode[i].m_id.set(query.lastInsertId().toLongLong());
        else
          break;
      }
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool ProductSQL::remove(Id id,
                        QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " PRODUCT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool CategorySQL::execSelect(QSqlQuery& query,
                             Category& category,
                             QString& error)
{
  error.clear();
  Id id = category.m_id;
  category.clear();

  query.prepare("SELECT "
                CATEGORY_SQL_COL01 ","
                CATEGORY_SQL_COL02
                " FROM " CATEGORY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      category.m_id = id;
      category.m_image.m_id.set(query.value(0).toLongLong());
      category.m_name = query.value(1).toString();
    }
    else
    {
      error = "Categoria não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && category.m_image.m_id.isValid())
    bSuccess = ImageSQL::execSelect(query, category.m_image, error);

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    category.clear();
  }

  return bSuccess;
}

bool CategorySQL::select(Category& category,
                         QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, category, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool CategorySQL::insert(const Category& o,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare(
        QString("INSERT INTO " CATEGORY_SQL_TABLE_NAME " ("
                CATEGORY_SQL_COL01 ","
                CATEGORY_SQL_COL02 ")"
                " VALUES ("
                "%1,"
                "(:_v02))").arg(o.m_image.m_id.isValid() ? "(:_v01)" : "NULL"));

  if (o.m_image.m_id.isValid())
    query.bindValue(":_v01", o.m_image.m_id.get());
  query.bindValue(":_v02", o.m_name);

  bool bSuccess = query.exec();
  if (bSuccess)
    o.m_id.set(query.lastInsertId().toLongLong());

  return finishTransaction(db, query, bSuccess, error);
}

bool CategorySQL::update(const Category& o,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare(
        QString("UPDATE " CATEGORY_SQL_TABLE_NAME " SET "
                CATEGORY_SQL_COL01 " = %1,"
                CATEGORY_SQL_COL02 " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)").arg(o.m_image.m_id.isValid() ? "(:_v01)" : "NULL"));
  query.bindValue(":_v00", o.m_id.get());
  query.bindValue(":_v02", o.m_name);

  if (o.m_image.m_id.isValid())
    query.bindValue(":_v01", o.m_image.m_id.get());

  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool CategorySQL::remove(Id id,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " CATEGORY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();

  return finishTransaction(db, query, bSuccess, error);
}

bool ImageSQL::execSelect(QSqlQuery& query,
                          Image& image,
                          QString& error)
{
  Id id = image.m_id;
  image.clear();
  query.prepare("SELECT "
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02
                " FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

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

bool ImageSQL::select(Image& image,
                      QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, image, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool ImageSQL::insert(const Image& image,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " IMAGE_SQL_TABLE_NAME " ("
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", image.m_name);
  query.bindValue(":_v02", image.m_image);

  bool bSuccess = query.exec();
  if (bSuccess)
    image.m_id.set(query.lastInsertId().toLongLong());

  return finishTransaction(db, query, bSuccess, error);
}

bool ImageSQL::update(const Image& image,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " IMAGE_SQL_TABLE_NAME " SET "
                IMAGE_SQL_COL01 " = (:_v01),"
                IMAGE_SQL_COL02 " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", image.m_id.get());
  query.bindValue(":_v01", image.m_name);
  query.bindValue(":_v02", image.m_image);

  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool ImageSQL::remove(Id id,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool ReminderSQL::execSelect(QSqlQuery& query,
                             Reminder& reminder,
                             QString& error)
{
  Id id = reminder.m_id;
  reminder.clear();
  query.prepare("SELECT "
                REMINDER_SQL_COL01 ","
                REMINDER_SQL_COL02 ","
                REMINDER_SQL_COL03 ","
                REMINDER_SQL_COL04 ","
                REMINDER_SQL_COL05 ","
                REMINDER_SQL_COL06 ","
                REMINDER_SQL_COL07
                " FROM " REMINDER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

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
      reminder.m_bBarcodeHRI = query.value(5).toBool();
      reminder.m_barcode = query.value(6).toString();
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

bool ReminderSQL::select(Reminder& reminder,
                         QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, reminder, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool ReminderSQL::insert(const Reminder& reminder,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " REMINDER_SQL_TABLE_NAME " ("
                REMINDER_SQL_COL01 ","
                REMINDER_SQL_COL02 ","
                REMINDER_SQL_COL03 ","
                REMINDER_SQL_COL04 ","
                REMINDER_SQL_COL05 ","
                REMINDER_SQL_COL06 ","
                REMINDER_SQL_COL07
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");
  query.bindValue(":_v01", reminder.m_title);
  query.bindValue(":_v02", reminder.m_message);
  query.bindValue(":_v03", reminder.m_bFavorite);
  query.bindValue(":_v04", (int)reminder.m_capitalization);
  query.bindValue(":_v05", (int)reminder.m_size);
  query.bindValue(":_v06", reminder.m_bBarcodeHRI);
  query.bindValue(":_v07", reminder.m_barcode);

  bool bSuccess = query.exec();
  if (bSuccess)
    reminder.m_id.set(query.lastInsertId().toLongLong());
  return finishTransaction(db, query, bSuccess, error);
}

bool ReminderSQL::update(const Reminder& reminder,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " REMINDER_SQL_TABLE_NAME " SET "
                REMINDER_SQL_COL01 " = (:_v01),"
                REMINDER_SQL_COL02 " = (:_v02),"
                REMINDER_SQL_COL03 " = (:_v03),"
                REMINDER_SQL_COL04 " = (:_v04),"
                REMINDER_SQL_COL05 " = (:_v05),"
                REMINDER_SQL_COL06 " = (:_v06),"
                REMINDER_SQL_COL07 " = (:_v07)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", reminder.m_id.get());
  query.bindValue(":_v01", reminder.m_title);
  query.bindValue(":_v02", reminder.m_message);
  query.bindValue(":_v03", reminder.m_bFavorite);
  query.bindValue(":_v04", (int)reminder.m_capitalization);
  query.bindValue(":_v05", (int)reminder.m_size);
  query.bindValue(":_v06", reminder.m_bBarcodeHRI);
  query.bindValue(":_v07", reminder.m_barcode);

  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool ReminderSQL::remove(Id id,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " REMINDER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
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

bool PersonSQL::execSelect(QSqlQuery& query,
                           Person& person,
                           QString& error)
{
  error.clear();
  Id id = person.m_id;
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
                PERSON_SQL_COL11
                " FROM " PERSON_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      person.m_id = id;
      person.m_image.m_id.set(query.value(0).toLongLong());
      person.m_name = query.value(1).toString();
      person.m_alias = query.value(2).toString();
      person.m_email = query.value(3).toString();
      person.m_CPF_CNPJ = query.value(4).toString();
      person.m_RG_IE = query.value(5).toString();
      person.m_details = query.value(6).toString();
      person.m_dtBirth = query.value(7).toDate();
      person.m_dtCreation = query.value(8).toDate();
      person.m_bCompany = query.value(9).toBool();
      person.m_bBirth = query.value(10).toBool();
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  EMPLOYEE_SQL_COL02 ","
                  EMPLOYEE_SQL_COL03 ","
                  EMPLOYEE_SQL_COL04
                  " FROM " EMPLOYEE_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", id.get());
    bSuccess = query.exec();
    if (bSuccess && query.next())
    {
      person.m_employee.m_bIsEmployee = true;
      person.m_employee.m_pincode = query.value(0).toString();
      person.m_employee.m_bNoteEdit = query.value(1).toBool();
      person.m_employee.m_bNoteRemove = query.value(2).toBool();
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT * "
                  " FROM " SUPPLIER_SQL_TABLE_NAME
                  " WHERE " SUPPLIER_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", id.get());
    bSuccess = query.exec();
    if (bSuccess && query.next())
    {
      person.m_supplier.m_bIsSupplier = true;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " ADDRESS_SQL_TABLE_NAME
                  " WHERE " ADDRESS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", id.get());
    bSuccess = query.exec();
    QVector<Id> ids;
    while (bSuccess && query.next())
    {
      Id id;
      id.set(query.value(0).toLongLong());
      ids.push_back(id);
    }

    for (int i = 0; i != ids.size(); ++i)
    {
      QString error2;
      Address address;
      address.m_id.set(ids.at(i).get());
      AddressSQL::execSelect(query, address, error2);
      person.m_vAddress.push_back(address);
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " PHONE_SQL_TABLE_NAME
                  " WHERE " PHONE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", id.get());
    bSuccess = query.exec();
    QVector<Id> ids;
    while (bSuccess && query.next())
    {
      Id id;
      id.set(query.value(0).toLongLong());
      ids.push_back(id);
    }

    for (int i = 0; i != ids.size(); ++i)
    {
      QString error2;
      Phone phone;
      phone.m_id.set(ids.at(i).get());
      PhoneSQL::execSelect(query, phone, error2);
      person.m_vPhone.push_back(phone);
    }
  }

  if (bSuccess && person.m_image.m_id.isValid())
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
                EMPLOYEE_SQL_COL01
                " FROM " EMPLOYEE_SQL_TABLE_NAME
                " WHERE " EMPLOYEE_SQL_COL02 " = (:_v02) LIMIT 1");
  query.bindValue(":_v02", pincode);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      person.m_id.set(query.value(0).toLongLong());
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

bool PersonSQL::select(Person& person,
                       QString& error)
{
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, person, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool PersonSQL::insert(const Person& person,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
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
                PERSON_SQL_COL11 ")"
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
                "(:_v11))");
  query.bindValue(":_v02", person.m_name);
  query.bindValue(":_v03", person.m_alias);
  query.bindValue(":_v04", person.m_email);
  query.bindValue(":_v05", person.m_CPF_CNPJ);
  query.bindValue(":_v06", person.m_RG_IE);
  query.bindValue(":_v07", person.m_details);
  query.bindValue(":_v08", person.m_dtBirth);
  query.bindValue(":_v09", person.m_dtCreation);
  query.bindValue(":_v10", person.m_bCompany);
  query.bindValue(":_v10", person.m_bBirth);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    person.m_id.set(query.lastInsertId().toLongLong());

    if (person.m_employee.m_bIsEmployee)
    {
      query.prepare("INSERT INTO " EMPLOYEE_SQL_TABLE_NAME " ("
                    EMPLOYEE_SQL_COL01 ","
                    EMPLOYEE_SQL_COL02 ","
                    EMPLOYEE_SQL_COL03 ","
                    EMPLOYEE_SQL_COL04
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04))");
      query.bindValue(":_v01", person.m_id.get());
      query.bindValue(":_v02", person.m_employee.getPincodeNull());
      query.bindValue(":_v03", person.m_employee.m_bNoteEdit);
      query.bindValue(":_v04", person.m_employee.m_bNoteRemove);
      bSuccess = query.exec();
    }

    if (bSuccess && person.m_supplier.m_bIsSupplier)
    {
      query.prepare("INSERT INTO " SUPPLIER_SQL_TABLE_NAME " ("
                    SUPPLIER_SQL_COL01 ")"
                    " VALUES ("
                    "(:_v01))");
      query.bindValue(":_v01", person.m_id.get());
      bSuccess = query.exec();
    }

    if (bSuccess && person.m_image.m_id.isValid())
    {
      query.prepare("UPDATE " PERSON_SQL_TABLE_NAME " SET "
                    PERSON_SQL_COL01 " = (:_v01)"
                    " WHERE " SQL_COLID " = (:_v00)");
      query.bindValue(":_v00", person.m_id.get());
      query.bindValue(":_v01", person.m_image.m_id.get());
      bSuccess = query.exec();
    }

    if (bSuccess)
    {
      for (int i = 0; i != person.m_vPhone.size(); ++i)
      {
        query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                      PHONE_SQL_COL01 ","
                      PHONE_SQL_COL02 ","
                      PHONE_SQL_COL03 ","
                      PHONE_SQL_COL04 ","
                      PHONE_SQL_COL05 ")"
                      " VALUES ("
                      "(:_v01),"
                      "(:_v02),"
                      "(:_v03),"
                      "(:_v04),"
                      "(:_v05))");
        query.bindValue(":_v01", person.m_id.get());
        query.bindValue(":_v02", person.m_vPhone.at(i).m_countryCode);
        query.bindValue(":_v03", person.m_vPhone.at(i).m_code);
        query.bindValue(":_v04", person.m_vPhone.at(i).m_number);
        query.bindValue(":_v05", person.m_vPhone.at(i).m_name);
        bSuccess = query.exec();
        if (bSuccess)
          person.m_vPhone.at(i).m_id.set(query.lastInsertId().toLongLong());
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
        query.bindValue(":_v01", person.m_id.get());
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
          person.m_vAddress.at(i).m_id.set(query.lastInsertId().toLongLong());
        else
          break;
      }
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool PersonSQL::update(const Person& person,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
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
                PERSON_SQL_COL11 " = (:_v11)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", person.m_id.get());
  query.bindValue(":_v02", person.m_name);
  query.bindValue(":_v03", person.m_alias);
  query.bindValue(":_v04", person.m_email);
  query.bindValue(":_v05", person.m_CPF_CNPJ);
  query.bindValue(":_v06", person.m_RG_IE);
  query.bindValue(":_v07", person.m_details);
  query.bindValue(":_v08", person.m_dtBirth);
  query.bindValue(":_v09", person.m_dtCreation);
  query.bindValue(":_v10", person.m_bCompany);
  query.bindValue(":_v11", person.m_bBirth);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " EMPLOYEE_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", person.m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " SUPPLIER_SQL_TABLE_NAME
                  " WHERE " SUPPLIER_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", person.m_id.get());
    bSuccess = query.exec();
  }

  if (person.m_employee.m_bIsEmployee)
  {
    query.prepare("INSERT INTO " EMPLOYEE_SQL_TABLE_NAME " ("
                  EMPLOYEE_SQL_COL01 ","
                  EMPLOYEE_SQL_COL02 ","
                  EMPLOYEE_SQL_COL03 ","
                  EMPLOYEE_SQL_COL04
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04))");
    query.bindValue(":_v01", person.m_id.get());
    query.bindValue(":_v02", person.m_employee.getPincodeNull());
    query.bindValue(":_v03", person.m_employee.m_bNoteEdit);
    query.bindValue(":_v04", person.m_employee.m_bNoteRemove);
    bSuccess = query.exec();
  }

  if (bSuccess && person.m_supplier.m_bIsSupplier)
  {
    query.prepare("INSERT INTO " SUPPLIER_SQL_TABLE_NAME " ("
                  SUPPLIER_SQL_COL01 ")"
                  " VALUES ("
                  "(:_v01))");
    query.bindValue(":_v01", person.m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess && person.m_image.m_id.isValid())
  {
    query.prepare("UPDATE " PERSON_SQL_TABLE_NAME " SET "
                  PERSON_SQL_COL01 " = (:_v01)"
                                   " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", person.m_id.get());
    query.bindValue(":_v01", person.m_image.m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " ADDRESS_SQL_TABLE_NAME " WHERE " ADDRESS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", person.m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " PHONE_SQL_TABLE_NAME " WHERE " PHONE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", person.m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != person.m_vPhone.size(); ++i)
    {
      query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                    PHONE_SQL_COL01 ","
                    PHONE_SQL_COL02 ","
                    PHONE_SQL_COL03 ","
                    PHONE_SQL_COL04 ","
                    PHONE_SQL_COL05 ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05))");
      query.bindValue(":_v01", person.m_id.get());
      query.bindValue(":_v02", person.m_vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", person.m_vPhone.at(i).m_code);
      query.bindValue(":_v04", person.m_vPhone.at(i).m_number);
      query.bindValue(":_v05", person.m_vPhone.at(i).m_name);
      bSuccess = query.exec();
      if (bSuccess)
        person.m_vPhone.at(i).m_id.set(query.lastInsertId().toLongLong());
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
      query.bindValue(":_v01", person.m_id.get());
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
        person.m_vAddress.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool PersonSQL::remove(Id id,
                       QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " PERSON_SQL_TABLE_NAME " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool PersonSQL::isValidPinCode(const QString& pincode,
                               Person& person,
                               QString& error)
{
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execByPinCodeSelect(query, pincode, person, error);
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

bool ReservationSQL::execSelect(QSqlQuery& query,
                                Reservation& res,
                                QString& error)
{
  error.clear();
  Id id = res.m_id;
  res.clear();

  query.prepare("SELECT "
                RESERVATION_SQL_COL01 ","
                RESERVATION_SQL_COL02 ","
                RESERVATION_SQL_COL03 ","
                RESERVATION_SQL_COL04 ","
                RESERVATION_SQL_COL05 ","
                RESERVATION_SQL_COL06 ","
                RESERVATION_SQL_COL07
                " FROM " RESERVATION_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      res.m_id = id;
      res.m_number = query.value(0).toLongLong();
      res.m_name = query.value(1).toString();
      res.m_location = query.value(2).toString();
      res.m_dateTime = query.value(3).toString();
      res.m_ammount = query.value(4).toInt();
      res.m_observation = query.value(5).toString();
      res.m_phone = query.value(6).toString();
    }
    else
    {
      error = "Reserva não encontrada.";
      bSuccess = false;
    }
  }

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    res.clear();
  }

  return bSuccess;
}

bool ReservationSQL::select(Reservation& res,
                            QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, res, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool ReservationSQL::insert(const Reservation& res,
                            QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.exec("SELECT MAX(" RESERVATION_SQL_COL01 ") FROM " RESERVATION_SQL_TABLE_NAME);
  res.m_number = query.next() ? query.value(0).toLongLong() + 1 : 1;
  query.prepare("INSERT INTO " RESERVATION_SQL_TABLE_NAME " ("
                RESERVATION_SQL_COL01 ","
                RESERVATION_SQL_COL02 ","
                RESERVATION_SQL_COL03 ","
                RESERVATION_SQL_COL04 ","
                RESERVATION_SQL_COL05 ","
                RESERVATION_SQL_COL06 ","
                RESERVATION_SQL_COL07 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");
  query.bindValue(":_v01", res.m_number);
  query.bindValue(":_v02", res.m_name);
  query.bindValue(":_v03", res.m_location);
  query.bindValue(":_v04", res.m_dateTime);
  query.bindValue(":_v05", res.m_ammount);
  query.bindValue(":_v06", res.m_observation);
  query.bindValue(":_v07", res.m_phone);

  bool bSuccess = query.exec();
  if (bSuccess)
    res.m_id.set(query.lastInsertId().toLongLong());

  return finishTransaction(db, query, bSuccess, error);
}

bool ReservationSQL::update(const Reservation& res,
                            QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " RESERVATION_SQL_TABLE_NAME " SET "
                RESERVATION_SQL_COL01 " = (:_v01),"
                RESERVATION_SQL_COL02 " = (:_v02),"
                RESERVATION_SQL_COL03 " = (:_v03),"
                RESERVATION_SQL_COL04 " = (:_v04),"
                RESERVATION_SQL_COL05 " = (:_v05),"
                RESERVATION_SQL_COL06 " = (:_v06),"
                RESERVATION_SQL_COL07 " = (:_v07)"
                " WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", res.m_id.get());
  query.bindValue(":_v01", res.m_number);
  query.bindValue(":_v02", res.m_name);
  query.bindValue(":_v03", res.m_location);
  query.bindValue(":_v04", res.m_dateTime);
  query.bindValue(":_v05", res.m_ammount);
  query.bindValue(":_v06", res.m_observation);
  query.bindValue(":_v07", res.m_phone);
  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool ReservationSQL::remove(Id id,
                            QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " RESERVATION_SQL_TABLE_NAME
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

bool DiscountSQL::insert(const Discount& o, QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  Settings settings;
  settings.load();

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = false;

  do
  {
    o.m_code = Discount::getRandomCode();
    query.prepare("SELECT " SQL_COLID " FROM "
                  DISCOUNT_SQL_TABLE_NAME " WHERE "
                  DISCOUNT_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", o.m_code);
    bSuccess = query.exec();
  } while (bSuccess && query.next());

  if (bSuccess)
  {
    query.prepare("INSERT INTO " DISCOUNT_SQL_TABLE_NAME " ("
                  DISCOUNT_SQL_COL01 ","
                  DISCOUNT_SQL_COL02 ","
                  DISCOUNT_SQL_COL03 ","
                  DISCOUNT_SQL_COL04 ","
                  DISCOUNT_SQL_COL05 ","
                  DISCOUNT_SQL_COL06 ","
                  DISCOUNT_SQL_COL07 ","
                  DISCOUNT_SQL_COL08
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04),"
                  "(:_v05),"
                  "(:_v06),"
                  "(:_v07),"
                  "(:_v08))");
      query.bindValue(":_v01", o.m_code);
      query.bindValue(":_v02", o.m_bExpires);
      query.bindValue(":_v03", o.m_dtExp);
      query.bindValue(":_v04", (int)o.m_type);
      query.bindValue(":_v05", o.m_value);
      query.bindValue(":_v06", o.m_percentage);
      query.bindValue(":_v07", o.m_bRedeemed);
      query.bindValue(":_v08", o.m_description);
      bSuccess = query.exec();
  }

  if (bSuccess)
  {
    o.m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != o.m_items.size(); ++i)
    {
      query.prepare("INSERT INTO " DISCOUNT_ITEMS_SQL_TABLE_NAME " ("
                    DISCOUNT_ITEMS_SQL_COL01 ","
                    DISCOUNT_ITEMS_SQL_COL02 ","
                    DISCOUNT_ITEMS_SQL_COL03
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", o.m_id.get());
      query.bindValue(":_v02", o.m_items.at(i).m_product.m_id.get());
      query.bindValue(":_v03", o.m_items.at(i).m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        o.m_items.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool DiscountSQL::update(const Discount& o, QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " DISCOUNT_SQL_TABLE_NAME " SET "
                DISCOUNT_SQL_COL01 " = (:_v01),"
                DISCOUNT_SQL_COL02 " = (:_v02),"
                DISCOUNT_SQL_COL03 " = (:_v03),"
                DISCOUNT_SQL_COL04 " = (:_v04),"
                DISCOUNT_SQL_COL05 " = (:_v05),"
                DISCOUNT_SQL_COL06 " = (:_v06),"
                DISCOUNT_SQL_COL07 " = (:_v07),"
                DISCOUNT_SQL_COL08 " = (:_v08) "
                "WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", o.m_id.get());
  query.bindValue(":_v01", o.m_code);
  query.bindValue(":_v02", o.m_bExpires);
  query.bindValue(":_v03", o.m_dtExp);
  query.bindValue(":_v04", (int)o.m_type);
  query.bindValue(":_v05", o.m_value);
  query.bindValue(":_v06", o.m_percentage);
  query.bindValue(":_v07", o.m_bRedeemed);
  query.bindValue(":_v08", o.m_description);
  bool bSuccess = query.exec();

  query.prepare("DELETE FROM " DISCOUNT_ITEMS_SQL_TABLE_NAME " WHERE " DISCOUNT_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", o.m_id.get());
  bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != o.m_items.size(); ++i)
    {
      query.prepare("INSERT INTO " DISCOUNT_ITEMS_SQL_TABLE_NAME " ("
                    DISCOUNT_ITEMS_SQL_COL01 ","
                    DISCOUNT_ITEMS_SQL_COL02 ","
                    DISCOUNT_ITEMS_SQL_COL03
                    " ) VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", o.m_id.get());
      query.bindValue(":_v02", o.m_items.at(i).m_product.m_id.get());
      query.bindValue(":_v03", o.m_items.at(i).m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        o.m_items.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool DiscountSQL::execSelect(QSqlQuery& query,
                             Discount& o,
                             QString& error)
{
  error.clear();
  Id id = o.m_id;
  o.clear();

  query.prepare("SELECT "
                DISCOUNT_SQL_COL01 ","
                DISCOUNT_SQL_COL02 ","
                DISCOUNT_SQL_COL03 ","
                DISCOUNT_SQL_COL04 ","
                DISCOUNT_SQL_COL05 ","
                DISCOUNT_SQL_COL06 ","
                DISCOUNT_SQL_COL07 ","
                DISCOUNT_SQL_COL08
                " FROM " DISCOUNT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      o.m_id = id;
      o.m_code = query.value(0).toString();
      o.m_bExpires = query.value(1).toBool();
      o.m_dtExp = query.value(2).toDate();
      o.m_type = (Discount::Type)query.value(3).toInt();
      o.m_value = query.value(4).toDouble();
      o.m_percentage = query.value(5).toDouble();
      o.m_bRedeemed = query.value(6).toBool();
      o.m_description = query.value(7).toString();
    }
    else
    {
      error = "Desconto não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  DISCOUNT_ITEMS_SQL_COL02 ","
                  DISCOUNT_ITEMS_SQL_COL03
                  " FROM " DISCOUNT_ITEMS_SQL_TABLE_NAME
                  " WHERE " DISCOUNT_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", o.m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (bSuccess && query.next())
      {
        DiscountItem oi;
        oi.m_id.set(query.value(0).toLongLong());
        oi.m_product.m_id.set(query.value(1).toLongLong());
        oi.m_ammount = query.value(2).toDouble();
        o.m_items.push_back(oi);
      }
    }
  }

  if (bSuccess)
  {
    QString error2;
    for (int i = 0; i != o.m_items.size(); ++i)
    {
      if (o.m_items.at(i).m_product.m_id.isValid())
        ProductSQL::execSelect(query, o.m_items[i].m_product, error2);
    }
  }

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    o.clear();
  }

  return bSuccess;
}

bool DiscountSQL::select(Discount& o, QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  bool bSuccess = execSelect(query, o, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool DiscountSQL::remove(Id id,
                         QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " DISCOUNT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();
  return finishTransaction(db, query, bSuccess, error);
}

bool DiscountSQL::redeem(const QString& code,
                         Discount& o,
                         bool& redeemed,
                         QString& error)
{
  redeemed = false;
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                SQL_COLID
                " FROM " DISCOUNT_SQL_TABLE_NAME
                " WHERE " DISCOUNT_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", code);
  bool bSuccess = query.exec();

  if (bSuccess && query.next())
  {
    o.m_id.set(query.value(0).toLongLong());
    bSuccess = execSelect(query, o, error);

    if (bSuccess)
    {
      redeemed = o.m_bRedeemed;
      query.prepare("UPDATE " DISCOUNT_SQL_TABLE_NAME " SET "
                    DISCOUNT_SQL_COL07 " = TRUE "
                    "WHERE " SQL_COLID " = (:_v00)");
      query.bindValue(":_v00", o.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
        o.m_bRedeemed = true;
    }
  }
  else
  {
    bSuccess = false;
    error = "Código informado não encontrado.";
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool StoreSQL::execSelect(QSqlQuery& query,
                          Store& o,
                          QString& error)
{
  error.clear();
  Id id = o.m_id;
  o.clear();

  query.prepare("SELECT "
                STORE_SQL_COL01 ","
                STORE_SQL_COL02 ","
                STORE_SQL_COL03 ","
                STORE_SQL_COL04
                " FROM " STORE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      o.m_id = id;
      o.m_person.m_id = query.value(0).toLongLong();
      o.m_address.m_id = query.value(1).toLongLong();
      o.m_phone.m_id = query.value(2).toLongLong();
      o.m_name = query.value(3).toString();

      query.prepare("SELECT "
                    SQL_COLID ","
                    STORE_EMPLOYEES_SQL_COL02
                    " FROM " STORE_EMPLOYEES_SQL_TABLE_NAME
                    " WHERE " STORE_EMPLOYEES_SQL_COL01 " = (:_v01)");
      query.bindValue(":_v01", o.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        while (query.next())
        {
          StoreEmployee _o;
          _o.m_id.set(query.value(0).toLongLong());
          _o.m_employee.m_id.set(query.value(1).toLongLong());
          o.m_vEmployee.push_back(_o);
        }
      }
    }
    else
    {
      error = "Loja não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && o.m_person.m_id.isValid())
    bSuccess = PersonSQL::execSelect(query, o.m_person, error);

  if (bSuccess)
  {
    for (int i = 0; i != o.m_vEmployee.size(); ++i)
    {
      if (o.m_vEmployee.at(i).m_id.isValid())
      {
        QString error2;
        if (PersonSQL::execSelect(query, o.m_vEmployee[i].m_employee, error2))
        {
          query.prepare("SELECT "
                        STORE_EMPLOYEE_HOURS_SQL_COL02 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL03 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL04
                        " FROM " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME
                        " WHERE " STORE_EMPLOYEE_HOURS_SQL_COL01 " = (:_v01)");
          query.bindValue(":_v01", o.m_vEmployee.at(i).m_id.get());
          bSuccess = query.exec();
          while (bSuccess && query.next())
          {
            TimeInterval h;
            h.m_day = query.value(0).toInt();
            h.m_tmBegin = query.value(1).toTime();
            h.m_tmEnd = query.value(2).toTime();
            o.m_vEmployee[i].m_hours.push_back(h);
          }
        }
      }
      if (!bSuccess)
        break;
    }
  }

  // TODO Separar Address e Phone e suas funções

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    o.clear();
  }

  return bSuccess;
}

bool StoreSQL::select(Store& o,
                      QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, o, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool StoreSQL::insert(const Store& o,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("INSERT INTO " STORE_SQL_TABLE_NAME " ("
                STORE_SQL_COL01 ","
                STORE_SQL_COL02 ","
                STORE_SQL_COL03 ","
                STORE_SQL_COL04 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04))");

  query.bindValue(":_v01", o.m_person.m_id.getIdNull());
  query.bindValue(":_v02", o.m_address.m_id.getIdNull());
  query.bindValue(":_v03", o.m_phone.m_id.getIdNull());
  query.bindValue(":_v04", o.m_name);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    o.m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != o.m_vEmployee.size(); ++i)
    {
      query.prepare("INSERT INTO " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                    STORE_EMPLOYEES_SQL_COL01 ","
                    STORE_EMPLOYEES_SQL_COL02
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02))");
      query.bindValue(":_v01", o.m_id.getIdNull());
      query.bindValue(":_v02", o.m_vEmployee.at(i).m_employee.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        o.m_vEmployee.at(i).m_id.set(query.lastInsertId().toLongLong());
        for (int j = 0; j != o.m_vEmployee.at(i).m_hours.size(); ++j)
        {
          query.prepare("INSERT INTO " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                        STORE_EMPLOYEE_HOURS_SQL_COL01 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL02 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL03 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL04
                        ") VALUES ("
                        "(:_v01),"
                        "(:_v02),"
                        "(:_v03),"
                        "(:_v04))");
          query.bindValue(":_v01", o.m_vEmployee.at(i).m_id.getIdNull());
          query.bindValue(":_v02", o.m_vEmployee.at(i).m_hours.at(j).m_day);
          query.bindValue(":_v03", o.m_vEmployee.at(i).m_hours.at(j).m_tmBegin);
          query.bindValue(":_v04", o.m_vEmployee.at(i).m_hours.at(j).m_tmEnd);
          bSuccess = query.exec();
          if (!bSuccess)
            break;
        }
      }

      if (!bSuccess)
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool StoreSQL::update(const Store& o,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("UPDATE " STORE_SQL_TABLE_NAME " SET "
                STORE_SQL_COL01 " = (:_v01),"
                STORE_SQL_COL02 " = (:_v02),"
                STORE_SQL_COL03 " = (:_v03),"
                STORE_SQL_COL04 " = (:_v04)"
                " WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", o.m_id.get());
  query.bindValue(":_v01", o.m_person.m_id.getIdNull());
  query.bindValue(":_v02", o.m_address.m_id.getIdNull());
  query.bindValue(":_v03", o.m_phone.m_id.getIdNull());
  query.bindValue(":_v04", o.m_name);

  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " STORE_EMPLOYEES_SQL_TABLE_NAME
                  " WHERE " STORE_EMPLOYEES_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", o.m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != o.m_vEmployee.size(); ++i)
    {
      query.prepare("INSERT INTO " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                    STORE_EMPLOYEES_SQL_COL01 ","
                    STORE_EMPLOYEES_SQL_COL02
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02))");
      query.bindValue(":_v01", o.m_id.getIdNull());
      query.bindValue(":_v02", o.m_vEmployee.at(i).m_employee.m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
      {
        o.m_vEmployee.at(i).m_id.set(query.lastInsertId().toLongLong());
        for (int j = 0; j != o.m_vEmployee.at(i).m_hours.size(); ++j)
        {
          query.prepare("INSERT INTO " STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                        STORE_EMPLOYEE_HOURS_SQL_COL01 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL02 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL03 ","
                        STORE_EMPLOYEE_HOURS_SQL_COL04
                        ") VALUES ("
                        "(:_v01),"
                        "(:_v02),"
                        "(:_v03),"
                        "(:_v04))");
          query.bindValue(":_v01", o.m_vEmployee.at(i).m_id.getIdNull());
          query.bindValue(":_v02", o.m_vEmployee.at(i).m_hours.at(j).m_day);
          query.bindValue(":_v03", o.m_vEmployee.at(i).m_hours.at(j).m_tmBegin);
          query.bindValue(":_v04", o.m_vEmployee.at(i).m_hours.at(j).m_tmEnd);
          bSuccess = query.exec();
          if (!bSuccess)
            break;
        }
      }

      if (!bSuccess)
        break;
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}

bool StoreSQL::remove(Id id,
                      QString& error)
{
  error.clear();

  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  query.prepare("DELETE FROM " STORE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());
  bool bSuccess = query.exec();

  return finishTransaction(db, query, bSuccess, error);
}

bool PhoneSQL::execSelect(QSqlQuery& query,
                          Phone& o,
                          QString& error)
{
  error.clear();
  Id id = o.m_id;
  o.clear();

  query.prepare("SELECT "
                PHONE_SQL_COL01 ","
                PHONE_SQL_COL02 ","
                PHONE_SQL_COL03 ","
                PHONE_SQL_COL04 ","
                PHONE_SQL_COL05
                " FROM " PHONE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      o.m_id = id;
      query.value(0).toLongLong(); //personId não usamos
      o.m_countryCode = query.value(1).toInt();
      o.m_code = query.value(2).toInt();
      o.m_number = query.value(3).toString();
      o.m_name = query.value(4).toString();
    }
    else
    {
      error = "Telefone não encontrado.";
      bSuccess = false;
    }
  }

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    o.clear();
  }

  return bSuccess;
}

bool PhoneSQL::select(Phone& o,
                      QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, o, error);
  return finishTransaction(db, query, bSuccess, error);
}

bool AddressSQL::execSelect(QSqlQuery& query,
                            Address& o,
                            QString& error)
{
  error.clear();
  Id id = o.m_id;
  o.clear();

  query.prepare("SELECT "
                ADDRESS_SQL_COL01 ","
                ADDRESS_SQL_COL02 ","
                ADDRESS_SQL_COL03 ","
                ADDRESS_SQL_COL04 ","
                ADDRESS_SQL_COL05 ","
                ADDRESS_SQL_COL06 ","
                ADDRESS_SQL_COL07 ","
                ADDRESS_SQL_COL08 ","
                ADDRESS_SQL_COL09
                " FROM " ADDRESS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      o.m_id = id;
      query.value(0).toLongLong(); // PersonId não usamoe
      o.m_cep = query.value(1).toString();
      o.m_neighborhood = query.value(2).toString();
      o.m_street = query.value(3).toString();
      o.m_number = query.value(4).toInt();
      o.m_city = query.value(5).toString();
      o.m_state = (Address::EBRState)query.value(6).toInt();
      o.m_complement = query.value(7).toString();
      o.m_reference = query.value(8).toString();
    }
    else
    {
      error = "Endereço não encontrado.";
      bSuccess = false;
    }
  }

  if (!bSuccess)
  {
    if (error.isEmpty())
      error = query.lastError().text();
    o.clear();
  }

  return bSuccess;
}

bool AddressSQL::select(Address& o,
                        QString& error)
{
  error.clear();
  if (!BaitaSQL::isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = execSelect(query, o, error);
  return finishTransaction(db, query, bSuccess, error);
=======
  query.prepare("DELETE FROM " ACTIVE_USERS_SQL_TABLE_NAME
                " WHERE " ACTIVE_USERS_SQL_COL01 " = pg_backend_pid()");
  return query.exec();
>>>>>>> refs/remotes/origin/master
}
