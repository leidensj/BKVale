#include "person.h"

Employee::Employee()
{
  clear();
}

void Employee::clear()
{
  m_bIsEmployee = false;
  m_pincode.clear();
  m_bNoteEdit = false;
  m_bNoteRemove = false;
}

Supplier::Supplier()
{
  clear();
}

void Supplier::clear()
{
  m_bIsSupplier = false;
}

Person::Person()
{
  clear();
}

void Person::clear()
{
  m_id.clear();
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
  m_bBirth = false;
}

bool Person::operator !=(const JItem& other) const
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

bool Person::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Person::isValid() const
{
  bool b = !m_name.isEmpty();
  return b;
}

QString Person::strTableName() const
{
return PERSON_SQL_TABLE_NAME
}

QString Person::strAliasName() const
{
  return m_alias.isEmpty() ? m_name : m_alias;
}
