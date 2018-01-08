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

  User(const QString& strUser,
       const QString& strPassword,
       bool bAccessNote,
       bool bAccessReminder,
       bool bAccessCalculator,
       bool bAccessShop,
       bool bAccessConsumption,
       bool bAccessUser,
       bool bAccessItem,
       bool bAccessSettings);

  mutable int m_id;
  QString m_strUser;
  QString m_strPassword;
  bool m_bAccessNote;
  bool m_bAccessReminder;
  bool m_bAccessCalculator;
  bool m_bAccessShop;
  bool m_bAccessConsumption;
  bool m_bAccessUser;
  bool m_bAccessItem;
  bool m_bAccessSettings;
};

class UserLogin
{
public:
  UserLogin();

  bool login(const QString& strUser,
             const QString& strPassword,
             QString& error);

private:
  User m_user;
  static bool st_isAdmin(const QString& strUser);
  bool loginAdmin(const QString& strUser,
                  const QString& strPassword,
                  QString& error);
  void enableAll(bool bEnable);

public:
  bool isValid() const { return m_user.m_id != INVALID_USER_ID; }
  bool isAdmin() const;
  QString strUser() const { return m_user.m_strUser; }
  QString strPassword() const { return m_user.m_strPassword; }
  bool hasAccessToNote() const { return isValid() && m_user.m_bAccessNote; }
  bool hasAccessToReminder() const { return isValid() && m_user.m_bAccessReminder; }
  bool hasAccessToCalculator() const { return isValid() && m_user.m_bAccessCalculator; }
  bool hasAccessToShop() const { return isValid() && m_user.m_bAccessShop; }
  bool hasAccessToConsumption() const { return isValid() && m_user.m_bAccessConsumption; }
  bool hasAccessToUsers() const { return isValid() && m_user.m_bAccessUser; }
  bool hasAccessToItems() const { return isValid() && m_user.m_bAccessItem; }
  bool hasAccessToSettings() const { return isValid() && m_user.m_bAccessSettings; }
};

#endif // USER_H
