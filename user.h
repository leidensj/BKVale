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
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject* parent) const;

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
  bool m_bAccessForm;
  bool m_bAccessEmployee;
  bool m_bAccessSupplier;
  bool m_bAccessCategory;
  bool m_bAccessImage;
  bool m_bAccessReservation;
  bool m_bAccessShoppingList;
  QString m_password;

  QString text() const { return "Usuário"; }
  QString icon() const { return ":/icons/res/user.png"; }
};

#endif // USER_H
