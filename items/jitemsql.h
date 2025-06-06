#ifndef JItemSQL_H
#define JItemSQL_H

#include "jitem.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QByteArray>

class JModel;
class QObject;

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
  virtual bool SQL_insert_update_proc(QSqlQuery& query) const;

  virtual void copy(const JItemSQL& other) { *this = other; }
  virtual QString name() const { return "Nome não implementado."; }
  virtual QByteArray printVersion(const QVariant& /*arg*/) const { return QByteArray(); }
  virtual QString firstName() const;
  virtual QString firstNameInitials() const;
  virtual QByteArray image() const { return QByteArray(); }
  virtual void setEmployee(const JItemSQL& /*e*/) const { }

  static bool SQL_isOpen(QString& error);
  static bool SQL_finish(QSqlDatabase db,
                         const QSqlQuery& query,
                         bool bExecSelectResult,
                         QString& error);
};

#endif // JItemSQL_H
