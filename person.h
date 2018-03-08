#ifndef PERSON_H
#define PERSON_H

#include "image.h"
#include <QObject>
#include <QString>
#include <QVector>

#define SQL_PERSON_TABLE_NAME "_PERSONS"
#define SQL_PERSON_COL00      "_ID"
#define SQL_PERSON_COL01      "_IMAGEID"
#define SQL_PERSON_COL02      "_NAME"
#define SQL_PERSON_COL03      "_ALIAS"
#define SQL_PERSON_COL04      "_EMAIL"
#define SQL_PERSON_COL05      "_CPF_CNPJ"
#define SQL_PERSON_COL06      "_RG_IE"
#define SQL_PERSON_COL07      "_DETAILS"
#define SQL_PERSON_COL08      "_BIRTHDATE"
#define SQL_PERSON_COL09      "_CREATIONDATE"
#define SQL_PERSON_COL10      "_IS_COMPANY"
#define SQL_PERSON_COL11      "_IS_CUSTOMER"
#define SQL_PERSON_COL12      "_IS_SUPPLIER"
#define SQL_PERSON_COL13      "_IS_EMPLOYEE"
#define SQL_PERSON_COL14      "_EMPLOYEE_PIN_CODE"

#define INVALID_PERSON_ID                 -1

#define MAX_PERSON_NAME_LENGTH            35
#define MAX_PERSON_DETAILS_LENGTH         35
#define EMPLOYEE_PINCODE_LENGTH            4

struct Person
{
  mutable qlonglong m_id;
  qlonglong m_imageId;
  QString m_name;
  QString m_alias;
  QString m_email;
  QString m_CPF_CNPJ;
  QString m_RG_IE;
  QString m_details;
  QString m_birthDate;
  QString m_creationDate;
  bool m_bCompany;
  bool m_bCustomer;
  bool m_bSupplier;
  bool m_bEmployee;
  QString m_employeePinCode;

  void clear()
  {
    m_id = INVALID_PERSON_ID;
    m_imageId = INVALID_IMAGE_ID;
    m_name.clear();
    m_alias.clear();
    m_email.clear();
    m_CPF_CNPJ.clear();
    m_RG_IE.clear();
    m_details.clear();
    m_birthDate.clear();
    m_creationDate.clear();
    m_bCompany = false;
    m_bCustomer = false;
    m_bSupplier = false;
    m_bEmployee = false;
    m_employeePinCode.clear();
  }

  Person()
  {
    clear();
  }

  bool operator !=(const Person& other) const
  {
    bool b =  m_imageId != other.m_imageId ||
              m_name != other.m_name ||
              m_alias != other.m_alias ||
              m_email != other.m_email ||
              m_CPF_CNPJ != other.m_CPF_CNPJ ||
              m_RG_IE != other.m_RG_IE ||
              m_details != other.m_details ||
              m_bCompany != other.m_bCompany ||
              m_bCustomer != other.m_bCustomer ||
              m_bSupplier != other.m_bSupplier ||
              m_bEmployee != other.m_bEmployee;

    if (!m_bCompany)
      b = b || m_birthDate != other.m_birthDate;

    if (m_bEmployee)
      b = b || m_employeePinCode != other.m_employeePinCode;

    return b;
  }

  bool operator ==(const Person& other) const
  {
    return !(*this != other);
  }

  static bool st_isValid(const Person& person)
  {
    bool b = !person.m_name.isEmpty();
    if (person.m_bEmployee)
      b = b && !person.m_employeePinCode.isEmpty();
    return b;
  }

  bool isValid() const { return st_isValid(*this); }
  static bool st_isValidId(qlonglong id) { return id != INVALID_PERSON_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(SQL_PERSON_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(SQL_PERSON_COL01, QObject::tr("Id Imagem")));
    c.push_back(JTableColumn(SQL_PERSON_COL02, QObject::tr("Nome"), false, false, JResizeMode::Interactive));
    c.push_back(JTableColumn(SQL_PERSON_COL03, QObject::tr("Apelido"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(SQL_PERSON_COL04, QObject::tr("Email")));
    c.push_back(JTableColumn(SQL_PERSON_COL05, QObject::tr("CPF/CNPJ")));
    c.push_back(JTableColumn(SQL_PERSON_COL06, QObject::tr("RG/IE")));
    c.push_back(JTableColumn(SQL_PERSON_COL07, QObject::tr("Detalhes")));
    c.push_back(JTableColumn(SQL_PERSON_COL08, QObject::tr("Data de Nascimento")));
    c.push_back(JTableColumn(SQL_PERSON_COL09, QObject::tr("Data de Criação")));
    c.push_back(JTableColumn(SQL_PERSON_COL10, QObject::tr("Empresa")));
    c.push_back(JTableColumn(SQL_PERSON_COL11, QObject::tr("Cliente")));
    c.push_back(JTableColumn(SQL_PERSON_COL12, QObject::tr("Fornecedor")));
    c.push_back(JTableColumn(SQL_PERSON_COL13, QObject::tr("Funcionário")));
    c.push_back(JTableColumn(SQL_PERSON_COL14, QObject::tr("Código PIN")));
    return c;
  }
};

#endif // PERSON_H
