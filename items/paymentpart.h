#ifndef PAYMENTPART_H
#define PAYMENTPART_H

#include "jsubitemsql.h"
#include <QDate>

struct PaymentPart : public JSubItemSQL
{
  double m_value;
  QDate m_date;

  PaymentPart();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<PaymentPart>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);
};

#endif // PAYMENTPART_H
