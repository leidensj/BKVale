#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitem.h"
#include "defines.h"


struct User : public SQL_JItem
{
  User();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  QString strEncryptedPassword() const;
  static QString st_strEncryptedPassword(const QString& strPassword);

  QString m_strUser;
  bool m_bAccessNote;
  bool m_bAccessReminder;
  bool m_bAccessCalculator;
  bool m_bAccessShop;
  bool m_bAccessUser;
  bool m_bAccessProduct;
  bool m_bAccessSettings;
  bool m_bAccessPerson;
  bool m_bAccessEmployee;
  bool m_bAccessSupplier;
  bool m_bAccessCategory;
  bool m_bAccessImage;
  bool m_bAccessReservation;
  bool m_bAccessShoppingList;
  QString m_password;
};

#endif // USER_H
