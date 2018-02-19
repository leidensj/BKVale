#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jtablecolumn.h"

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
  static QString getTableName() { return "_USERS"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_USER", QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn("_PASSWORD", QObject::tr("Senha")));
    c.push_back(JTableColumn("_ACCESS_NOTE", QObject::tr("Notas")));
    c.push_back(JTableColumn("_ACCESS_REMINDER", QObject::tr("Lembretes")));
    c.push_back(JTableColumn("_ACCESS_CALCULATOR", QObject::tr("Calculadora")));
    c.push_back(JTableColumn("_ACCESS_SHOP", QObject::tr("Compras")));
    c.push_back(JTableColumn("_ACCESS_CONSUMPTION", QObject::tr("Consumo")));
    c.push_back(JTableColumn( "_ACCESS_USER", QObject::tr("Usuários")));
    c.push_back(JTableColumn("_ACCESS_PRODUCT", QObject::tr("Produtos")));
    c.push_back(JTableColumn("_ACCESS_SETTINGS", QObject::tr("Configurações")));
    return c;
  }
};

#endif // USER_H
