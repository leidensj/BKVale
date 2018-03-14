#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jtablecolumn.h"
#include "defines.h"

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

  mutable qlonglong m_id;
  QString m_strUser;
  bool m_bAccessNote;
  bool m_bAccessReminder;
  bool m_bAccessCalculator;
  bool m_bAccessShop;
  bool m_bAccessConsumption;
  bool m_bAccessUser;
  bool m_bAccessProduct;
  bool m_bAccessSettings;

  bool isValidId() const { return IS_VALID_ID(m_id); }
  static QString st_strEncryptedPassword(const QString& strPassword);
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(USER_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(USER_SQL_COL01, QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(USER_SQL_COL02, QObject::tr("Senha")));
    c.push_back(JTableColumn(USER_SQL_COL03, QObject::tr("Notas")));
    c.push_back(JTableColumn(USER_SQL_COL04, QObject::tr("Lembretes")));
    c.push_back(JTableColumn(USER_SQL_COL05, QObject::tr("Calculadora")));
    c.push_back(JTableColumn(USER_SQL_COL06, QObject::tr("Compras")));
    c.push_back(JTableColumn(USER_SQL_COL07, QObject::tr("Consumo")));
    c.push_back(JTableColumn(USER_SQL_COL08, QObject::tr("Usuários")));
    c.push_back(JTableColumn(USER_SQL_COL09, QObject::tr("Produtos")));
    c.push_back(JTableColumn(USER_SQL_COL10, QObject::tr("Configurações")));
    return c;
  }
};

#endif // USER_H
