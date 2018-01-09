#ifndef USER_H
#define USER_H

#include <QString>

#define INVALID_USER_ID     -1
#define MAX_USERNAME_LENGTH 20

enum class UserTableIndex : int
{
  ID = 0,
  User,
  Password,
  AccessNote,
  AccessReminder,
  AccessCalculator,
  AccessShop,
  AccessConsumption,
  AccessUser,
  AccessItem,
  AccessSettings
};

struct User
{
  User();

  void clear();

  mutable int m_id;
  QString m_strUser;
  bool m_bAccessNote;
  bool m_bAccessReminder;
  bool m_bAccessCalculator;
  bool m_bAccessShop;
  bool m_bAccessConsumption;
  bool m_bAccessUser;
  bool m_bAccessItem;
  bool m_bAccessSettings;

  static QString strEncryptedPassword(const QString& strPassword);
};

#endif // USER_H
