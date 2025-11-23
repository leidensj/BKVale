#ifndef CASHCLOSING_H
#define CASHCLOSING_H

#include <QString>
#include <QObject>
#include <QDate>
#include <QByteArray>
#include "jitemsql.h"
#include "cash.h"
#include "cashclosingcoin.h"
#include "cashclosingsector.h"
#include "cashclosinginfo.h"

struct CashClosing : public JItemSQL
{
  CashClosing(Id id = Id());
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  QByteArray printVersion(const QVariant& arg) const;

  double sumSectorsValue() const;
  int sumSectorsNValue() const;
  double sumCoinsValue() const;
  double sumCoinsWithTaxes() const;
  double sumCoinsTaxesDifference() const;
  double diff() const;
  double diffTax() const;

  Cash m_cash;
  QDateTime m_dt;
  QVector<CashClosingCoin> m_vcoins;
  QVector<CashClosingSector> m_vsectors;
  QVector<CashClosingInfo> m_vinfos;
  double m_debit;
  double m_credit;
  double m_comission;
};

#endif // CASHCLOSING_H
