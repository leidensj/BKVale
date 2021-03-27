#ifndef JSUBITEMSQL_H
#define JSUBITEMSQL_H

#include "jitem.h"
#include <QVector>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

struct JSubItemSQL : public JItem
{
  mutable Id m_ownerId;

  virtual bool SQL_insert_proc(QSqlQuery& query) const = 0;
  virtual bool SQL_select_proc(QSqlQuery& query, QString& error) = 0;
};

#endif
