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

class User
{
public:
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

  bool login(const QString& strUser,
             const QString& strPassword,
             QString& error);

private:
  static bool st_isAdmin(const QString& strUser);
  bool loginAdmin(const QString& strUser,
                  const QString& strPassword,
                  QString& error);
  void enableAll(bool bEnable);

  int m_id;
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

public:
  bool isValid() const { return m_id != INVALID_USER_ID; }
  bool isAdmin() const;
  QString strUser() const { return m_strUser; }
  QString strPassword() const { return m_strPassword; }
  bool hasAccessToNote() const { return m_bAccessNote; }
  bool hasAccessToReminder() const { return m_bAccessReminder; }
  bool hasAccessToCalculator() const { return m_bAccessCalculator; }
  bool hasAccessToShop() const { return m_bAccessShop; }
  bool hasAccessToConsumption() const { return m_bAccessConsumption; }
  bool hasAccessToUsers() const { return m_bAccessUser; }
  bool hasAccessToItems() const { return m_bAccessItem; }
  bool hasAccessToSettings() const { return m_bAccessSettings; }
};

#endif // USER_H
