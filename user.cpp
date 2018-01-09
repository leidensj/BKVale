#include "user.h"
#include <QCryptographicHash>
#include <QObject>

User::User()
{
  clear();
}

void User::clear()
{
  m_id = INVALID_USER_ID;
  m_strUser.clear();
  m_bAccessNote = false;
  m_bAccessReminder = false;
  m_bAccessCalculator = false;
  m_bAccessShop = false;
  m_bAccessConsumption = false;
  m_bAccessUser = false;
  m_bAccessItem = false;
  m_bAccessSettings = false;
}

QString User::strEncryptedPassword(const QString& strPassword)
{
  return QString(QCryptographicHash::hash(strPassword.toUtf8(),
                                          QCryptographicHash::Md5));
}
