#include "user.h"
#include <QCryptographicHash>
#include <QObject>

User::User()
{
  clear();
}

void User::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_strUser.clear();
  m_bPurchase = false;
  m_bReminder = false;
  m_bCalculator = false;
  m_bShop = false;
  m_bUser = false;
  m_bProduct = false;
  m_bSettings = false;
  m_bForm = false;
  m_bEmployee = false;
  m_bSupplier = false;
  m_bCategory = false;
  m_bImage = false;
  m_bShoppingList = false;
  m_bStore = false;
  m_bTimeCard = false;
  m_password.clear();
}

QString User::st_strEncryptedPassword(const QString& strPassword)
{
  return QString(QCryptographicHash::hash(strPassword.toUtf8(),
                                          QCryptographicHash::Md5));
}

QString User::strEncryptedPassword() const
{
  return st_strEncryptedPassword(m_password);
}

bool User::operator != (const JItem& other) const
{
  const User& another = dynamic_cast<const User&>(other);
  return
      m_strUser != another.m_strUser ||
      m_bPurchase != another.m_bPurchase ||
      m_bReminder != another.m_bReminder ||
      m_bCalculator != another.m_bCalculator ||
      m_bShop != another.m_bCalculator ||
      m_bForm != another.m_bForm ||
      m_bEmployee != another.m_bEmployee ||
      m_bSupplier != another.m_bSupplier ||
      m_bUser != another.m_bUser ||
      m_bProduct != another.m_bProduct ||
      m_bSettings != another.m_bSettings ||
      m_bCategory != another.m_bCategory ||
      m_bImage != another.m_bImage ||
      m_bShoppingList != another.m_bShoppingList ||
      m_bStore != another.m_bStore ||
      m_bTimeCard != another.m_bTimeCard;
}

bool User::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool User::isValid() const
{
  return m_strUser.length() > 4;
}

QString User::SQL_tableName() const
{
  return USER_SQL_TABLE_NAME;
}

bool User::SQL_insert_proc(QSqlQuery& query) const
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
                USER_SQL_COL13 ","
                USER_SQL_COL14 ","
                USER_SQL_COL15 ","
                USER_SQL_COL16 ","
                USER_SQL_COL17 ","
                USER_SQL_COL18 ")"
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
                "(:_v16),"
                "(:_v17),"
                "(:_v18))");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", m_bPurchase);
  query.bindValue(":_v04", m_bReminder);
  query.bindValue(":_v05", m_bCalculator);
  query.bindValue(":_v06", m_bShop);
  query.bindValue(":_v07", m_bUser);
  query.bindValue(":_v08", m_bProduct);
  query.bindValue(":_v09", m_bSettings);
  query.bindValue(":_v10", m_bForm);
  query.bindValue(":_v11", m_bCategory);
  query.bindValue(":_v12", m_bImage);
  query.bindValue(":_v14", m_bShoppingList);
  query.bindValue(":_v15", m_bEmployee);
  query.bindValue(":_v16", m_bSupplier);
  query.bindValue(":_v17", m_bStore);
  query.bindValue(":_v18", m_bTimeCard);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool User::SQL_update_proc(QSqlQuery& query) const
{
  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL01 " = (:_v01),");
  if (!m_password.isEmpty())
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
              USER_SQL_COL16" = (:_v16),"
              USER_SQL_COL17" = (:_v17),"
              USER_SQL_COL18" = (:_v18)"
              " WHERE " SQL_COLID " = (:_v00)";

  query.prepare(strQuery);
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_strUser);
  if (!m_password.isEmpty())
    query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", m_bPurchase);
  query.bindValue(":_v04", m_bReminder);
  query.bindValue(":_v05", m_bCalculator);
  query.bindValue(":_v06", m_bShop);
  query.bindValue(":_v07", m_bUser);
  query.bindValue(":_v08", m_bProduct);
  query.bindValue(":_v09", m_bSettings);
  query.bindValue(":_v10", m_bForm);
  query.bindValue(":_v11", m_bCategory);
  query.bindValue(":_v12", m_bImage);
  query.bindValue(":_v14", m_bShoppingList);
  query.bindValue(":_v15", m_bEmployee);
  query.bindValue(":_v16", m_bSupplier);
  query.bindValue(":_v17", m_bStore);
  query.bindValue(":_v18", m_bTimeCard);

  return query.exec();
}

bool User::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
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
                USER_SQL_COL16 ","
                USER_SQL_COL17 ","
                USER_SQL_COL18
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_strUser = query.value(0).toString();
      query.value(1).toString(); // password nao precisamos
      m_bPurchase = query.value(2).toBool();
      m_bReminder = query.value(3).toBool();
      m_bCalculator = query.value(4).toBool();
      m_bShop = query.value(5).toBool();
      m_bUser = query.value(6).toBool();
      m_bProduct = query.value(7).toBool();
      m_bSettings = query.value(8).toBool();
      m_bForm = query.value(9).toBool();
      m_bCategory = query.value(10).toBool();
      m_bImage = query.value(11).toBool();
      m_bShoppingList = query.value(13).toBool();
      m_bEmployee = query.value(14).toBool();
      m_bSupplier = query.value(15).toBool();
      m_bStore = query.value(16).toBool();
      m_bTimeCard = query.value(17).toBool();
    }
    else
    {
      bSuccess = false;
      error = "Usuário não encontrado.";
    }
  }

  return bSuccess;
}

bool User::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool User::SQL_select_password_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SQL_COLID
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " USER_SQL_COL01 " = (:_v01) AND "
                USER_SQL_COL02 " = (:_v02) LIMIT 1");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
      bSuccess = SQL_select_proc(query, error);
    }
    else
    {
      error = "Usuário ou senha inválidos.";
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool User::hasPermission(const QString& tableName) const
{
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return m_bImage;
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return m_bCategory;
  if (tableName == STORE_SQL_TABLE_NAME)
    return m_bStore;
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return m_bProduct;
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return m_bPurchase;
  if (tableName == USER_SQL_TABLE_NAME)
    return m_bUser;
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return m_bReminder;
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return m_bShoppingList;
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return m_bEmployee;
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return m_bSupplier;
  return false;
}
