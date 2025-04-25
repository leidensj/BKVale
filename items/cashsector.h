#ifndef CASHSECTOR_H
#define CASHSECTOR_H

#include "jsubitemsql.h"
#include "sector.h"
#include <QVector>

struct CashSector : public JSubItemSQL
{
  CashSector();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashSector>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  Sector m_sector;
};

#endif // CASHSECTOR_H
