#ifndef PAYMENT_H
#define PAYMENT_H

#include "jitemsql.h"
#include <QDate>
#include <QVector>

struct PaymentPart : public JItem
{
  double m_value;
  QDate m_date;

  PaymentPart();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
};

struct Payment : public JItemSQL
{
  enum class Method
  {
    Credit,
    Cash,
    Bonus
  };

  Payment();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject* parent) const;

  bool SQL_select_proc_by_noteid(QSqlQuery& query, QString& error);

  double total() const;
  bool isAllCash(double total) const;
  bool isAllBonus(double total) const;
  bool isAllCredit(double total) const;

  double m_cash;
  double m_bonus;
  QVector<PaymentPart> m_vCredit;
  mutable Id m_noteId;
};

#endif // PAYMENT_H
