#ifndef CASHCLOSINGSECTOR_H
#define CASHCLOSINGSECTOR_H

#include "jsubitemsql.h"
#include "items/image.h"
#include <QVector>

struct CashClosingSector : public JSubItemSQL
{
  CashClosingSector();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashClosingSector>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  QString m_sname;
  Image m_simage;
  double m_value;
  int m_nvalue;
};

#endif // CASHCLOSINGSECTOR_H
