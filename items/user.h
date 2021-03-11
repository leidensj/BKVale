#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitemsql.h"
#include "defines.h"

struct User : public JItemSQL
{
  User();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  bool SQL_select_password_proc(QSqlQuery& query, QString& error);

  QString strEncryptedPassword() const;
  static QString st_strEncryptedPassword(const QString& strPassword);

  bool hasPermission(const QString& tableName) const;

  QString m_strUser;
  bool m_bPurchase;
  bool m_bReminder;
  bool m_bCalculator;
  bool m_bShop;
  bool m_bUser;
  bool m_bProduct;
  bool m_bSettings;
  bool m_bEmployee;
  bool m_bSupplier;
  bool m_bCategory;
  bool m_bImage;
  bool m_bShoppingList;
  bool m_bStore;
  bool m_bTimeCard;
  bool m_bCoupon;
  QString m_password;
};

#endif // USER_H
