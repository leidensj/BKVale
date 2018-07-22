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
  bool m_bCustomer;
  bool m_bSupplier;
  bool m_bEmployee;
  QString m_employeePinCode;
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
    m_bCustomer = false;
    m_bSupplier = false;
    m_bEmployee = false;
    m_employeePinCode.clear();
    m_vPhone.clear();
    m_vAddress.clear();
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
              m_bCustomer != another.m_bCustomer ||
              m_bSupplier != another.m_bSupplier ||
              m_bEmployee != another.m_bEmployee ||
              m_vPhone != another.m_vPhone ||
              m_vAddress != another.m_vAddress;

    if (!m_bCompany)
      b = b || m_dtBirth != another.m_dtBirth;

    if (m_bEmployee)
      b = b || m_employeePinCode != another.m_employeePinCode;

    return b;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    bool b = !m_name.isEmpty();
    if (m_bEmployee)
      b = b && !m_employeePinCode.isEmpty();
    return b;
  }
};

#endif // PERSON_H
