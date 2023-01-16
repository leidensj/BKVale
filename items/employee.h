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

  bool SQL_select_by_pincode(QString& error);
  bool SQL_select_by_pincode_proc(QSqlQuery& query, QString& error);
  bool SQL_select_formid_proc(QSqlQuery& query) const;

  QString name() const { return m_form.name(); }
  QByteArray image() const { return m_form.m_image.m_image; }

  Form m_form;
  QString m_pincode;
  QMap<Functionality::Idx, bool> m_createEditPermissions;
  QMap<Functionality::Idx, bool> m_removePermissions;

  bool hasPermissionToCreateEdit(Functionality::Idx idx) const;
  bool hasPermissionToCreateEdit(const QString& tableName) const;
  bool hasPermissionToRemove(Functionality::Idx idx) const;
  bool hasPermissionToRemove(const QString& tableName) const;
  void setPermissionToCreateEdit(Functionality::Idx idx, bool bSet);
  void setPermissionToRemove(Functionality::Idx idx, bool bSet);

  QString strHours() const;
  QVector<TimeInterval> m_hours;
  QVariant getPincodeNull() const { return !m_pincode.isEmpty() ? m_pincode : QVariant(QVariant::String); }
};

#endif
