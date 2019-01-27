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
};

struct Supplier
{
  Supplier();
  void clear();

  bool m_bIsSupplier;
};

struct Person : public JItem
{
  Person();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;
  QString strAliasName() const;

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
