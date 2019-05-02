#ifndef STORE_H
#define STORE_H

#include "employee.h"
#include <QVector>

struct Store : public JItemSQL
{
  Store();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator ==(const JItem& other) const;
  bool operator !=(const JItem& other) const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject* parent) const;

  Form m_form;
  Address m_address;
  Phone m_phone;
  QString m_description;
  QVector<Employee> m_vEmployee;

  QString name() const { return  m_form.m_name; }
  QByteArray image() const { return m_form.m_image.m_image; }
};

#endif // STORE_H
