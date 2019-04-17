#ifndef PAYMENT_H
#define PAYMENT_H

#include "jitemsql.h"
#include <QDate>

struct Payment : public JItemSQL
{
  enum class Method
  {
    Credit,
    Cash,
    Bonus
  };

  Payment();
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

  double m_value;
  QDate m_date;
  mutable Id m_noteId;
};

#endif // PAYMENT_H
