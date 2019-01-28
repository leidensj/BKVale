#include "user.h"
#include <QCryptographicHash>
#include <QObject>

User::User()
{
  clear();
}

void User::clear()
{
  m_id = INVALID_ID;
  m_strUser.clear();
  m_bAccessNote = false;
  m_bAccessReminder = false;
  m_bAccessCalculator = false;
  m_bAccessShop = false;
  m_bAccessUser = false;
  m_bAccessProduct = false;
  m_bAccessSettings = false;
  m_bAccessPerson = false;
  m_bAccessEmployee = false;
  m_bAccessSupplier = false;
  m_bAccessCategory = false;
  m_bAccessImage = false;
  m_bAccessReservation = false;
  m_bAccessShoppingList = false;
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
      m_bAccessNote != another.m_bAccessNote ||
      m_bAccessReminder != another.m_bAccessReminder ||
      m_bAccessCalculator != another.m_bAccessCalculator ||
      m_bAccessShop != another.m_bAccessCalculator ||
      m_bAccessPerson != another.m_bAccessPerson ||
      m_bAccessEmployee != another.m_bAccessEmployee ||
      m_bAccessSupplier != another.m_bAccessSupplier ||
      m_bAccessUser != another.m_bAccessUser ||
      m_bAccessProduct != another.m_bAccessProduct ||
      m_bAccessSettings != another.m_bAccessSettings ||
      m_bAccessReservation != another.m_bAccessReservation ||
      m_bAccessCategory != another.m_bAccessCategory ||
      m_bAccessImage != another.m_bAccessImage ||
      m_bAccessShoppingList != another.m_bAccessShoppingList;
}

bool User::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool User::isValid() const
{
  return m_strUser.length() > 4;
}

QString User::strTableName() const
{
  return USER_SQL_TABLE_NAME;
}
