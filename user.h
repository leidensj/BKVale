#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jtablecolumn.h"

#define SQL_USER_TABLE_NAME "_USERS"
#define SQL_USER_COL00 "_ID"
#define SQL_USER_COL01 "_USER"
#define SQL_USER_COL02 "_PASSWORD"
#define SQL_USER_COL03 "_ACCESS_NOTE"
#define SQL_USER_COL04 "_ACCESS_REMINDER"
#define SQL_USER_COL05 "_ACCESS_CALCULATOR"
#define SQL_USER_COL06 "_ACCESS_SHOP"
#define SQL_USER_COL07 "_ACCESS_CONSUMPTION"
#define SQL_USER_COL08 "_ACCESS_USER"
#define SQL_USER_COL09 "_ACCESS_PRODUCT"
#define SQL_USER_COL10 "_ACCESS_SETTINGS"

#define SQL_USER_DEFAULT_NAME "ADMIN"
#define SQL_USER_DEFAULT_PASSWORD "admin"

#define INVALID_USER_ID     -1
#define USER_MAX_USERNAME_LENGTH 20
#define USER_MAX_PASSWORD_LENGTH 20

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
  AccessProduct,
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
  bool m_bAccessProduct;
  bool m_bAccessSettings;

  static bool st_isValidID(int id) { return id != INVALID_USER_ID; }
  static QString st_strEncryptedPassword(const QString& strPassword);
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(SQL_USER_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(SQL_USER_COL01, QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(SQL_USER_COL02, QObject::tr("Senha")));
    c.push_back(JTableColumn(SQL_USER_COL03, QObject::tr("Notas")));
    c.push_back(JTableColumn(SQL_USER_COL04, QObject::tr("Lembretes")));
    c.push_back(JTableColumn(SQL_USER_COL05, QObject::tr("Calculadora")));
    c.push_back(JTableColumn(SQL_USER_COL06, QObject::tr("Compras")));
    c.push_back(JTableColumn(SQL_USER_COL07, QObject::tr("Consumo")));
    c.push_back(JTableColumn(SQL_USER_COL08, QObject::tr("Usuários")));
    c.push_back(JTableColumn(SQL_USER_COL09, QObject::tr("Produtos")));
    c.push_back(JTableColumn(SQL_USER_COL10, QObject::tr("Configurações")));
    return c;
  }
};

#endif // USER_H
