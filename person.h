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
  bool m_bIsEmployee;
  QString m_pincode;
  bool m_bNoteEdit;
  bool m_bNoteRemove;

  void clear()
  {
    m_bIsEmployee = false;
    m_pincode.clear();
    m_bNoteEdit = false;
    m_bNoteRemove = false;
  }

  Employee()
  {
    clear();
  }
};

struct Supplier
{
  bool m_bIsSupplier;

  void clear()
  {
    m_bIsSupplier = false;
  }

  Supplier()
  {
    clear();
  }
};

struct Person : public JItem
{
  Image m_image;
  QString m_name;
  QString m_alias;
  QString m_email;
  QString m_CPF_CNPJ;
  QString m_RG_IE;
  QString m_details;
  QDate m_dtBirth;
  QDate m_dtCreation;
  bool m_bCompany;
  Supplier m_supplier;
  Employee m_employee;
  QVector<Phone> m_vPhone;
  QVector<Address> m_vAddress;

  void clear()
  {
    m_id = INVALID_ID;
    m_image.clear();
    m_name.clear();
    m_alias.clear();
    m_email.clear();
    m_CPF_CNPJ.clear();
    m_RG_IE.clear();
    m_details.clear();
    m_dtBirth = QDate::currentDate();
    m_dtCreation = QDate::currentDate();
    m_bCompany = false;
    m_supplier.clear();
    m_employee.clear();
    m_vPhone.clear();
    m_vAddress.clear();
    m_supplier.clear();
    m_employee.clear();
  }

  Person()
  {
    clear();
  }

  QString strAliasName() const
  {
    return m_alias.isEmpty() ? m_name : m_alias;
  }

  bool operator !=(const JItem& other) const
  {
    const Person& another = dynamic_cast<const Person&>(other);
    bool b =  m_image.m_id != another.m_image.m_id ||
              m_name != another.m_name ||
              m_alias != another.m_alias ||
              m_email != another.m_email ||
              m_CPF_CNPJ != another.m_CPF_CNPJ ||
              m_RG_IE != another.m_RG_IE ||
              m_details != another.m_details ||
              m_bCompany != another.m_bCompany ||
              m_vPhone != another.m_vPhone ||
              m_vAddress != another.m_vAddress;

    if (!m_bCompany)
      b = b || m_dtBirth != another.m_dtBirth;

    if (m_employee.m_bIsEmployee)
    {
      b = b || m_employee.m_pincode != another.m_employee.m_pincode;
    }

    if (m_supplier.m_bIsSupplier)
      b = b; //NOTHING TODO

    return b;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    bool b = !m_name.isEmpty();
    return b;
  }
};

#endif // PERSON_H
