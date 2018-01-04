#ifndef USER_H
#define USER_H

#include <QString>

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
  bool login(const QString& strUser,
             const QString& strPassword,
             QString& error);

private:
  static bool st_isAdmin(const QString& strUser);
  bool loginAdmin(const QString& strUser,
                  const QString& strPassword,
                  QString& error);
  void enableAll(bool bEnable);

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
  bool isAdmin() const;
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
