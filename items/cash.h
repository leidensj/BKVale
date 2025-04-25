#ifndef CASH_H
#define CASH_H

#include <QString>
#include <QObject>
#include <QDate>
#include "jitemsql.h"
#include "cashcoin.h"
#include "cashsector.h"
#include "cashinfo.h"

struct Cash : public JItemSQL
{
  Cash(Id id = Id());
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  QString m_name;
  QVector<CashCoin> m_vcoins;
  QVector<CashSector> m_vsectors;
  QVector<CashInfo> m_vinfos;
  QString name() const { return m_name; }
};

#endif // CASH_H
