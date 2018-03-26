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
  m_bAccessConsumption = false;
  m_bAccessUser = false;
  m_bAccessProduct = false;
  m_bAccessSettings = false;
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
