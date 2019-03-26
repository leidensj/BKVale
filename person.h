#ifndef PERSON_H
#define PERSON_H

#include "image.h"
#include "phone.h"
#include "address.h"
#include "defines.h"
#include "jitem.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QDate>

struct Employee
{
  Employee();
  void clear();

  bool m_bIsEmployee;
  QString m_pincode;
  bool m_bNoteEdit;
  bool m_bNoteRemove;

  QVariant getPincodeNull() const { return !m_pincode.isEmpty() ? m_pincode : QVariant(QVariant::String); }
};

struct Supplier
{
  Supplier();
  void clear();

  bool m_bIsSupplier;
};

struct Person : public SQL_JItem
{
  Person();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strAliasName() const;

  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  bool SQL_select_by_pincode(const QString& pincode, QString& error);
  bool SQL_select_by_pincode_proc(QSqlQuery& query, const QString& pincode, QString& error);

  Image m_image;
  QString m_name;
  QString m_alias;
  QString m_email;
  QString m_CPF_CNPJ;
  QString m_RG_IE;
  QString m_details;
  bool m_bBirth;
  QDate m_dtBirth;
  QDate m_dtCreation;
  bool m_bCompany;
  Supplier m_supplier;
  Employee m_employee;
  QVector<Phone> m_vPhone;
  QVector<Address> m_vAddress;
};

#endif
