#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitem.h"
#include "defines.h"

typedef QString Password;

struct User : public JItem
{
  User();

  void clear();

  QString m_strUser;
  bool m_bAccessNote;
  bool m_bAccessReminder;
  bool m_bAccessCalculator;
  bool m_bAccessShop;
  bool m_bAccessConsumption;
  bool m_bAccessUser;
  bool m_bAccessProduct;
  bool m_bAccessSettings;
  bool m_bAccessPerson;
  bool m_bAccessCategory;
  bool m_bAccessImage;
  bool m_bAccessReservation;
  bool m_bAccessShoppingList;
  Password m_password;

  bool operator != (const JItem& other) const
  {
    const User& another = dynamic_cast<const User&>(other);
    return
        m_strUser != another.m_strUser ||
        m_bAccessNote != another.m_bAccessNote ||
        m_bAccessReminder != another.m_bAccessReminder ||
        m_bAccessCalculator != another.m_bAccessCalculator ||
        m_bAccessShop != another.m_bAccessCalculator ||
        m_bAccessConsumption != another.m_bAccessConsumption ||
        m_bAccessUser != another.m_bAccessUser ||
        m_bAccessProduct != another.m_bAccessProduct ||
        m_bAccessSettings != another.m_bAccessSettings ||
        m_bAccessReservation != another.m_bAccessReservation ||
        m_bAccessShoppingList != another.m_bAccessShoppingList;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return m_strUser.length() > 4;
  }

  QString strEncryptedPassword() const;
  static QString st_strEncryptedPassword(const QString& strPassword);
};

#endif // USER_H
