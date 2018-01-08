#include "user.h"
#include <QCryptographicHash>
#include <QObject>

#define ADMIN_USERNAME "ADMIN"
#define ADMIN_PASSWORD "a1b2c3d4"

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

User::User(const QString& strUser,
           bool bAccessNote,
           bool bAccessReminder,
           bool bAccessCalculator,
           bool bAccessShop,
           bool bAccessConsumption,
           bool bAccessUser,
           bool bAccessItem,
           bool bAccessSettings)
  : m_strUser(strUser)
  , m_bAccessNote(bAccessNote)
  , m_bAccessReminder(bAccessReminder)
  , m_bAccessCalculator(bAccessCalculator)
  , m_bAccessShop(bAccessShop)
  , m_bAccessConsumption(bAccessConsumption)
  , m_bAccessUser(bAccessUser)
  , m_bAccessItem(bAccessItem)
  , m_bAccessSettings(bAccessSettings)
{

}

QString User::strEncryptedPassword(const QString& strPassword) const
{
  return QString(QCryptographicHash::hash(strPassword.toUtf8(),
                                          QCryptographicHash::Md5));
}

UserLogin::UserLogin()
{

}

bool UserLogin::login(const QString& strUser,
           const QString& strPassword,
           QString& error)
{
  if (st_isAdmin(strUser))
    return loginAdmin(strUser, strPassword, error);
  error = QObject::tr("TODO");
  return false;
}
