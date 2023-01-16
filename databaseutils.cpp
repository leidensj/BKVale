#include "databaseutils.h"
#include "defines.h"
#include "items/user.h"
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

bool BaitaSQL::init(const QString& hostName, int port, QString& error)
{
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
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
                          IMAGE_SQL_COL_NAM " TEXT NOT NULL UNIQUE,"
                          IMAGE_SQL_COL_IMA " BYTEA)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " CATEGORY_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          CATEGORY_SQL_COL_IID " INTEGER,"
                          CATEGORY_SQL_COL_NAM " TEXT NOT NULL UNIQUE,"
                          "FOREIGN KEY(" CATEGORY_SQL_COL_IID ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " REMINDER_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          REMINDER_SQL_COL_TIT " TEXT,"
                          REMINDER_SQL_COL_MES " TEXT,"
                          REMINDER_SQL_COL_FAV " BOOLEAN,"
                          REMINDER_SQL_COL_CAP " INTEGER,"
                          REMINDER_SQL_COL_SIZ " INTEGER,"
                          REMINDER_SQL_COL_HRI " BOOLEAN,"
                          REMINDER_SQL_COL_BAR " TEXT,"
                          REMINDER_SQL_COL_DAT " DATE,"
                          REMINDER_SQL_COL_HDA " BOOLEAN,"
                          REMINDER_SQL_COL_TIM " TIME,"
                          REMINDER_SQL_COL_HTI " BOOLEAN,"
                          REMINDER_SQL_COL_SUB " TEXT)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " USER_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          USER_SQL_COL_USE " TEXT NOT NULL UNIQUE CHECK ("
                          USER_SQL_COL_USE " <> ''),"
                          USER_SQL_COL_PAS " TEXT NOT NULL,"
                          USER_SQL_COL_APU " BOOLEAN,"
                          USER_SQL_COL_ARE " BOOLEAN,"
                          USER_SQL_COL_ACA " BOOLEAN,"
                          USER_SQL_COL_ASH " BOOLEAN,"
                          USER_SQL_COL_AUS " BOOLEAN,"
                          USER_SQL_COL_APR " BOOLEAN,"
                          USER_SQL_COL_ASE " BOOLEAN,"
                          USER_SQL_COL_ACT " BOOLEAN,"
                          USER_SQL_COL_AIM " BOOLEAN,"
                          USER_SQL_COL_ASL " BOOLEAN,"
                          USER_SQL_COL_AEM " BOOLEAN,"
                          USER_SQL_COL_ASU " BOOLEAN,"
                          USER_SQL_COL_AST " BOOLEAN,"
                          USER_SQL_COL_ATI " BOOLEAN,"
                          USER_SQL_COL_ACO " BOOLEAN,"
                          USER_SQL_COL_ACR " BOOLEAN,"
                          USER_SQL_COL_ART " BOOLEAN)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PRODUCT_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        PRODUCT_SQL_COL_NAM " TEXT NOT NULL UNIQUE,"
                        PRODUCT_SQL_COL_CID " INTEGER,"
                        PRODUCT_SQL_COL_IID " INTEGER,"
                        PRODUCT_SQL_COL_UNI " TEXT NOT NULL,"
                        PRODUCT_SQL_COL_DET " TEXT,"
                        PRODUCT_SQL_COL_ATB " BOOLEAN,"
                        PRODUCT_SQL_COL_ATS " BOOLEAN,"
                        "FOREIGN KEY(" PRODUCT_SQL_COL_CID ") REFERENCES "
                        CATEGORY_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                        "FOREIGN KEY(" PRODUCT_SQL_COL_IID ") REFERENCES "
                        IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " FORM_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          FORM_SQL_COL_IID " INTEGER,"
                          FORM_SQL_COL_NAM " TEXT NOT NULL,"
                          FORM_SQL_COL_ALI " TEXT,"
                          FORM_SQL_COL_EMA " TEXT,"
                          FORM_SQL_COL_CPF " TEXT,"
                          FORM_SQL_COL_RGI " TEXT,"
                          FORM_SQL_COL_DET " TEXT,"
                          FORM_SQL_COL_BIR " DATE,"
                          FORM_SQL_COL_CRE " DATE,"
                          FORM_SQL_COL_ISC " BOOLEAN,"
                          FORM_SQL_COL_HBI " BOOLEAN,"
                          FORM_SQL_COL_SEX " BOOLEAN,"
                          "FOREIGN KEY(" FORM_SQL_COL_IID ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " ADDRESS_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        ADDRESS_SQL_COL_FID " INTEGER,"
                        ADDRESS_SQL_COL_CEP " TEXT,"
                        ADDRESS_SQL_COL_NEI " TEXT,"
                        ADDRESS_SQL_COL_STR " TEXT,"
                        ADDRESS_SQL_COL_NUM " INTEGER,"
                        ADDRESS_SQL_COL_CIT " TEXT,"
                        ADDRESS_SQL_COL_STA " INTEGER,"
                        ADDRESS_SQL_COL_COM " TEXT,"
                        ADDRESS_SQL_COL_REF " TEXT,"
                        "FOREIGN KEY(" ADDRESS_SQL_COL_FID ") REFERENCES "
                        FORM_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PHONE_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PHONE_SQL_COL_FID " INTEGER,"
                          PHONE_SQL_COL_COU " INTEGER DEFAULT " PHONE_DEFAULT_COUNTRY_CODE_VALUE_STR ","
                          PHONE_SQL_COL_COD " INTEGER DEFAULT " PHONE_DEFAULT_CODE_VALUE_STR ","
                          PHONE_SQL_COL_NUM " TEXT,"
                          PHONE_SQL_COL_NAM " TEXT,"
                          "FOREIGN KEY(" PHONE_SQL_COL_FID ") REFERENCES "
                          FORM_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " STORE_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          STORE_SQL_COL_FID " INTEGER,"
                          STORE_SQL_COL_DES " TEXT,"
                          "FOREIGN KEY(" STORE_SQL_COL_FID ") REFERENCES "
                          FORM_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " EMPLOYEE_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          EMPLOYEE_SQL_COL_FID " INTEGER NOT NULL UNIQUE,"
                          EMPLOYEE_SQL_COL_PIN " TEXT UNIQUE,"
                          EMPLOYEE_SQL_COL_NED " BOOLEAN,"
                          EMPLOYEE_SQL_COL_NRE " BOOLEAN,"
                          EMPLOYEE_SQL_COL_RED " BOOLEAN,"
                          EMPLOYEE_SQL_COL_RRE " BOOLEAN,"
                          EMPLOYEE_SQL_COL_CED " BOOLEAN,"
                          EMPLOYEE_SQL_COL_CRE " BOOLEAN,"
                          "FOREIGN KEY(" EMPLOYEE_SQL_COL_FID ") REFERENCES "
                          FORM_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " EMPLOYEE_HOURS_SQL_TABLE_NAME " ("
                          EMPLOYEE_HOURS_SQL_COL_EID " INTEGER NOT NULL,"
                          EMPLOYEE_HOURS_SQL_COL_DAY " INTEGER,"
                          EMPLOYEE_HOURS_SQL_COL_BEG " TIME,"
                          EMPLOYEE_HOURS_SQL_COL_END " TIME,"
                          "FOREIGN KEY(" EMPLOYEE_HOURS_SQL_COL_EID ") REFERENCES "
                          EMPLOYEE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SUPPLIER_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          SUPPLIER_SQL_COL_FID " INTEGER NOT NULL UNIQUE,"
                          "FOREIGN KEY(" SUPPLIER_SQL_COL_FID ") REFERENCES "
                          FORM_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          STORE_EMPLOYEES_SQL_COL_EID " INTEGER NOT NULL UNIQUE,"
                          STORE_EMPLOYEES_SQL_COL_SID " INTEGER NOT NULL,"
                          "FOREIGN KEY(" STORE_EMPLOYEES_SQL_COL_EID ") REFERENCES "
                          EMPLOYEE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" STORE_EMPLOYEES_SQL_COL_SID ") REFERENCES "
                          STORE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
  bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PURCHASE_SQL_TABLE_NAME " ("
                        SQL_COLID " SERIAL PRIMARY KEY,"
                        PURCHASE_SQL_COL_NUM " INTEGER UNIQUE NOT NULL,"
                        PURCHASE_SQL_COL_DAT " DATE NOT NULL,"
                        PURCHASE_SQL_COL_SID " INTEGER,"
                        PURCHASE_SQL_COL_OBS " TEXT,"
                        PURCHASE_SQL_COL_DIS " REAL,"
                        PURCHASE_SQL_COL_EID " INTEGER,"
                        PURCHASE_SQL_COL_PAY " INTEGER NOT NULL,"
                        PURCHASE_SQL_COL_TID " INTEGER NOT NULL,"
                        "FOREIGN KEY(" PURCHASE_SQL_COL_SID ") REFERENCES "
                        SUPPLIER_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                        "FOREIGN KEY(" PURCHASE_SQL_COL_EID ") REFERENCES "
                        EMPLOYEE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                        "FOREIGN KEY(" PURCHASE_SQL_COL_TID ") REFERENCES "
                        STORE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PURCHASE_ELEMENTS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PURCHASE_ELEMENTS_SQL_COL_NID " INTEGER NOT NULL,"
                          PURCHASE_ELEMENTS_SQL_COL_PID " INTEGER,"
                          PURCHASE_ELEMENTS_SQL_COL_AMM " REAL,"
                          PURCHASE_ELEMENTS_SQL_COL_PRI " REAL,"
                          PURCHASE_ELEMENTS_SQL_COL_ISP " BOOLEAN,"
                          PURCHASE_ELEMENTS_SQL_COL_PUN " TEXT,"
                          PURCHASE_ELEMENTS_SQL_COL_PAM " REAL,"
                          "FOREIGN KEY(" PURCHASE_ELEMENTS_SQL_COL_NID ") REFERENCES "
                          PURCHASE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" PURCHASE_ELEMENTS_SQL_COL_PID ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PAYMENT_ELEMENTS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PAYMENT_ELEMENTS_SQL_COL_NID " INTEGER NOT NULL,"
                          PAYMENT_ELEMENTS_SQL_COL_DAT " DATE,"
                          PAYMENT_ELEMENTS_SQL_COL_VAL " REAL,"
                          "FOREIGN KEY(" PAYMENT_ELEMENTS_SQL_COL_NID ") REFERENCES "
                          PURCHASE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SHOPPING_LIST_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          SHOPPING_LIST_SQL_COL_SID " INTEGER,"
                          SHOPPING_LIST_SQL_COL_IID " INTEGER,"
                          SHOPPING_LIST_SQL_COL_TIT " TEXT NOT NULL,"
                          SHOPPING_LIST_SQL_COL_DES " TEXT,"
                          SHOPPING_LIST_SQL_COL_WEE " TEXT,"
                          SHOPPING_LIST_SQL_COL_MON " TEXT,"
                          SHOPPING_LIST_SQL_COL_NLI " INTEGER,"
                          "FOREIGN KEY(" SHOPPING_LIST_SQL_COL_SID ") REFERENCES "
                          SUPPLIER_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL,"
                          "FOREIGN KEY(" SHOPPING_LIST_SQL_COL_IID ") REFERENCES "
                          IMAGE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          SHOPPING_LIST_ELEMENTS_SQL_LID " INTEGER NOT NULL,"
                          SHOPPING_LIST_ELEMENTS_SQL_PID " INTEGER NOT NULL,"
                          SHOPPING_LIST_ELEMENTS_SQL_AMM " REAL,"
                          SHOPPING_LIST_ELEMENTS_SQL_PRI " REAL,"
                          SHOPPING_LIST_ELEMENTS_SQL_ISP " BOOLEAN,"
                          SHOPPING_LIST_ELEMENTS_SQL_PUN " TEXT,"
                          SHOPPING_LIST_ELEMENTS_SQL_PAM " REAL,"
                          SHOPPING_LIST_ELEMENTS_SQL_SID " INTEGER,"
                          "FOREIGN KEY(" SHOPPING_LIST_ELEMENTS_SQL_LID ") REFERENCES "
                          SHOPPING_LIST_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" SHOPPING_LIST_ELEMENTS_SQL_PID ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" SHOPPING_LIST_ELEMENTS_SQL_SID ") REFERENCES "
                          SUPPLIER_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PRODUCT_CODE_ITEMS_SQL_COL_PID " INTEGER NOT NULL,"
                          PRODUCT_CODE_ITEMS_SQL_COL_COD " TEXT UNIQUE NOT NULL CHECK ("
                          PRODUCT_CODE_ITEMS_SQL_COL_COD " <> ''),"
                          "FOREIGN KEY(" PRODUCT_CODE_ITEMS_SQL_COL_PID ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " LOGIN_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          LOGIN_SQL_COL_PID " INTEGER,"
                          LOGIN_SQL_COL_UID " INTEGER UNIQUE NOT NULL,"
                          LOGIN_SQL_COL_MAC " TEXT,"
                          LOGIN_SQL_COL_LOG " TIMESTAMP,"
                          "FOREIGN KEY(" LOGIN_SQL_COL_UID ") REFERENCES "
                          USER_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE NO ACTION);");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " COUPON_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          COUPON_SQL_COL_TYP " INTEGER,"
                          COUPON_SQL_COL_COD " TEXT UNIQUE NOT NULL CHECK ("
                          COUPON_SQL_COL_COD " <> ''),"
                          COUPON_SQL_COL_CDT " TIMESTAMP,"
                          COUPON_SQL_COL_RED " BOOLEAN,"
                          COUPON_SQL_COL_RDT " TIMESTAMP,"
                          COUPON_SQL_COL_EXP " BOOLEAN,"
                          COUPON_SQL_COL_EDT " DATE,"
                          COUPON_SQL_COL_PCT " INTEGER,"
                          COUPON_SQL_COL_VAL " REAL,"
                          COUPON_SQL_COL_SID " INTEGER,"
                          "FOREIGN KEY(" COUPON_SQL_COL_SID ") REFERENCES "
                          STORE_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " COUPON_ELEMENTS_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          COUPON_ELEMENTS_SQL_COL_CID " INTEGER NOT NULL,"
                          COUPON_ELEMENTS_SQL_COL_PID " INTEGER,"
                          COUPON_ELEMENTS_SQL_COL_AMM " REAL,"
                          "FOREIGN KEY(" COUPON_ELEMENTS_SQL_COL_CID ") REFERENCES "
                          COUPON_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE,"
                          "FOREIGN KEY(" COUPON_ELEMENTS_SQL_COL_PID ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE SET NULL)");

  if (bSuccess)
    bSuccess = query.exec("CREATE TABLE IF NOT EXISTS " PRICE_SQL_TABLE_NAME " ("
                          SQL_COLID " SERIAL PRIMARY KEY,"
                          PRICE_SQL_COL_PID " INTEGER NOT NULL UNIQUE,"
                          PRICE_SQL_COL_VAL " INTEGER,"
                          "FOREIGN KEY(" PRICE_SQL_COL_PID ") REFERENCES "
                          PRODUCT_SQL_TABLE_NAME "(" SQL_COLID ") ON DELETE CASCADE)");

  if (bSuccess)
  {
    query.exec("SELECT * FROM " USER_SQL_TABLE_NAME " LIMIT 1");
    if (!query.next())
    {
      QString str = "INSERT INTO " USER_SQL_TABLE_NAME " ("
                    USER_SQL_COL_USE ","
                    USER_SQL_COL_PAS ","
                    USER_SQL_COL_APU ","
                    USER_SQL_COL_ARE ","
                    USER_SQL_COL_ACA ","
                    USER_SQL_COL_ASH ","
                    USER_SQL_COL_AUS ","
                    USER_SQL_COL_APR ","
                    USER_SQL_COL_ASE ","
                    USER_SQL_COL_ACT ","
                    USER_SQL_COL_AIM ","
                    USER_SQL_COL_ASL ","
                    USER_SQL_COL_AEM ","
                    USER_SQL_COL_ASU ","
                    USER_SQL_COL_AST ","
                    USER_SQL_COL_ATI ","
                    USER_SQL_COL_ACO ","
                    USER_SQL_COL_ACR ","
                    USER_SQL_COL_ART ")"
                    " VALUES ('"
                    USER_SQL_DEFAULT_NAME "',"
                    ":_password,"
                    "TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);";
      query.prepare(str);
      query.bindValue(":_password", User::st_strEncryptedPassword(USER_SQL_DEFAULT_PASSWORD));
      bSuccess = query.exec();
    }
  }

  return finishTransaction(db, query, bSuccess, error);
}
