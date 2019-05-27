#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "form.h"
#include "timeinterval.h"
#include <QVector>

struct Employee : public JItemSQL
{
  Employee();
  void clear(bool bClearId = true);
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strAliasName() const;

  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject *parent) const;

  bool SQL_select_by_pincode(QString& error);
  bool SQL_select_by_pincode_proc(QSqlQuery& query, QString& error);
  bool SQL_select_formid_proc(QSqlQuery& query) const;

  QString name() const { return m_form.strAliasName(); }
  QByteArray image() const { return m_form.m_image.m_image; }

  Form m_form;
  QString m_pincode;
  bool m_bNoteEdit;
  bool m_bNoteRemove;

  QString strHours() const;
  QVector<TimeInterval> m_hours;
  QVariant getPincodeNull() const { return !m_pincode.isEmpty() ? m_pincode : QVariant(QVariant::String); }
  bool hasPermissionToEdit(const QString& tableName) const;
  bool hasPermissionToRemove(const QString& tableName) const;
};

#endif
