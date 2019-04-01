#ifndef JItemSQL_H
#define JItemSQL_H

#include "jitem.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

struct JItemSQL : public JItem
{
  virtual ~JItemSQL();
  virtual QString SQL_tableName() const = 0;
  virtual bool SQL_insert_proc(QSqlQuery& query) const = 0;
  virtual bool SQL_update_proc(QSqlQuery& query) const  = 0;
  virtual bool SQL_select_proc(QSqlQuery& query, QString& error) = 0;
  virtual bool SQL_remove_proc(QSqlQuery& query) const = 0;

  virtual bool SQL_insert_update(QString& error) const;
  virtual bool SQL_select(QString& error);
  virtual bool SQL_remove(QString& error) const;

  virtual bool SQL_authentication_insert_update() const { return false; }
  virtual bool SQL_authentication_remove() const { return false; }

  static bool SQL_isOpen(QString& error);
  static bool SQL_finish(QSqlDatabase db,
                         const QSqlQuery& query,
                         bool bExecSelectResult,
                         QString& error);
};

#endif // JItemSQL_H
