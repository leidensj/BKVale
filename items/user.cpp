#include "user.h"
#include <QCryptographicHash>
#include <QObject>

User::User()
{
  User::clear();
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
  m_bEmployee = false;
  m_bSupplier = false;
  m_bCategory = false;
  m_bImage = false;
  m_bShoppingList = false;
  m_bStore = false;
  m_bTimeCard = false;
  m_bCoupon = false;
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
      m_bEmployee != another.m_bEmployee ||
      m_bSupplier != another.m_bSupplier ||
      m_bUser != another.m_bUser ||
      m_bProduct != another.m_bProduct ||
      m_bSettings != another.m_bSettings ||
      m_bCategory != another.m_bCategory ||
      m_bImage != another.m_bImage ||
      m_bShoppingList != another.m_bShoppingList ||
      m_bStore != another.m_bStore ||
      m_bTimeCard != another.m_bTimeCard ||
      m_bCoupon != another.m_bCoupon;
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
                USER_SQL_COL_ACO ")"
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
                "(:_v17))");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", m_bPurchase);
  query.bindValue(":_v04", m_bReminder);
  query.bindValue(":_v05", m_bCalculator);
  query.bindValue(":_v06", m_bShop);
  query.bindValue(":_v07", m_bUser);
  query.bindValue(":_v08", m_bProduct);
  query.bindValue(":_v09", m_bSettings);
  query.bindValue(":_v10", m_bCategory);
  query.bindValue(":_v11", m_bImage);
  query.bindValue(":_v12", m_bShoppingList);
  query.bindValue(":_v13", m_bEmployee);
  query.bindValue(":_v14", m_bSupplier);
  query.bindValue(":_v15", m_bStore);
  query.bindValue(":_v16", m_bTimeCard);
  query.bindValue(":_v17", m_bCoupon);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool User::SQL_update_proc(QSqlQuery& query) const
{
  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL_USE " = (:_v01),");
  if (!m_password.isEmpty())
    strQuery += USER_SQL_COL_PAS " = (:_v02),";
  strQuery += USER_SQL_COL_APU " = (:_v03),"
              USER_SQL_COL_ARE " = (:_v04),"
              USER_SQL_COL_ACA " = (:_v05),"
              USER_SQL_COL_ASH " = (:_v06),"
              USER_SQL_COL_AUS " = (:_v07),"
              USER_SQL_COL_APR " = (:_v08),"
              USER_SQL_COL_ASE " = (:_v09),"
              USER_SQL_COL_ACT " = (:_v10),"
              USER_SQL_COL_AIM " = (:_v11),"
              USER_SQL_COL_ASL " = (:_v12),"
              USER_SQL_COL_AEM " = (:_v13),"
              USER_SQL_COL_ASU " = (:_v14),"
              USER_SQL_COL_AST " = (:_v15),"
              USER_SQL_COL_ATI " = (:_v16)"
              USER_SQL_COL_ACO " = (:_v17)"
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
  query.bindValue(":_v10", m_bCategory);
  query.bindValue(":_v11", m_bImage);
  query.bindValue(":_v12", m_bShoppingList);
  query.bindValue(":_v13", m_bEmployee);
  query.bindValue(":_v14", m_bSupplier);
  query.bindValue(":_v15", m_bStore);
  query.bindValue(":_v16", m_bTimeCard);
  query.bindValue(":_v17", m_bCoupon);

  return query.exec();
}

bool User::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
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
                USER_SQL_COL_ACO
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
      m_bCategory = query.value(9).toBool();
      m_bImage = query.value(10).toBool();
      m_bShoppingList = query.value(11).toBool();
      m_bEmployee = query.value(12).toBool();
      m_bSupplier = query.value(13).toBool();
      m_bStore = query.value(14).toBool();
      m_bTimeCard = query.value(15).toBool();
      m_bCoupon = query.value(16).toBool();
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
                " WHERE " USER_SQL_COL_USE " = (:_v01) AND "
                USER_SQL_COL_PAS " = (:_v02) LIMIT 1");
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

bool User::hasPermission(Functionality::Idx idx) const
{
  switch (idx)
  {
    case Functionality::Idx::User:
      return m_bUser;
    case Functionality::Idx::Category:
      return m_bCategory;
    case Functionality::Idx::Product:
      return m_bProduct;
    case Functionality::Idx::Image:
      return m_bImage;
    case Functionality::Idx::Employee:
      return m_bEmployee;
    case Functionality::Idx::Supplier:
      return m_bSupplier;
    case Functionality::Idx::Store:
      return m_bStore;
    case Functionality::Idx::Purchase:
      return m_bPurchase;
    case Functionality::Idx::Calculator:
      return m_bCalculator;
    case Functionality::Idx::Reminder:
      return m_bReminder;
    case Functionality::Idx::ShoppingList:
      return m_bShoppingList;
    case Functionality::Idx::Shop:
      return m_bShop;
    case Functionality::Idx::TimeCard:
      return m_bTimeCard;
    case Functionality::Idx::Coupon:
      return m_bCoupon;
    case Functionality::Idx::Settings:
      return m_bSettings;
    case Functionality::Idx::_END:
    default:
      return false;
  }
}

bool User::hasPermission(const QString& tableName) const
{
  return hasPermission(Functionality::tableNameToIdx(tableName));
}
