#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "form.h"
#include "timeinterval.h"
#include "employeepermission.h"
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

  static bool SQL_select_all(Ids& ids, QString& error);
  bool SQL_select_by_pincode(QString& error);
  bool SQL_select_by_pincode_proc(QSqlQuery& query, QString& error);
  bool SQL_select_formid_proc(QSqlQuery& query) const;

  QString name() const { return m_form.name(); }
  QByteArray image() const { return m_form.m_image.m_image; }
  QString strHours() const;

  bool hasPermissionToCreate(Functionality::Idx idx) const;
  bool hasPermissionToEdit(Functionality::Idx idx) const;
  bool hasPermissionToRemove(Functionality::Idx idx) const;
  bool hasPermissionToCreate(const QString& tableName) const;
  bool hasPermissionToEdit(const QString& tableName) const;
  bool hasPermissionToRemove(const QString& tableName) const;

  void setPermissionToCreate(Functionality::Idx idx, bool bSet);
  void setPermissionToEdit(Functionality::Idx idx, bool bSet);
  void setPermissionToRemove(Functionality::Idx idx, bool bSet);
  void setPermissionToCreate(const QString& tableName, bool bSet);
  void setPermissionToEdit(const QString& tableName, bool bSet);
  void setPermissionToRemove(const QString& tableName, bool bSet);

  Form m_form;
  QString m_pincode;
  QVector<TimeInterval> m_hours;
  QVector<EmployeePermission> m_permissions;
  QVariant getPincodeNull() const { return !m_pincode.isEmpty() ? m_pincode : QVariant(QVariant::String); }
};

#endif
