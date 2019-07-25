#ifndef PAYMENTELEMENT_H
#define PAYMENTELEMENT_H

#include "jitemelementsql.h"
#include <QDate>

struct PaymentElement : public JItemElementSQL
{
  double m_value;
  QDate m_date;

  PaymentElement();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<PaymentElement>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);
};

#endif // PAYMENTELEMENT_H
