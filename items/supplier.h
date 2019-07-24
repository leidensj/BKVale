#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "jitemsql.h"
#include "form.h"

struct Supplier : public JItemSQL
{
  Supplier();
  void clear(bool bClearId = true);
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;

  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  bool SQL_select_formid_proc(QSqlQuery& query) const;

  Form m_form;

  QString name() const { return m_form.strAliasName(); }
  QByteArray image() const { return m_form.m_image.m_image; }
};

#endif
