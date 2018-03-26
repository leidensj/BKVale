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
        m_bAccessSettings != another.m_bAccessSettings;
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
