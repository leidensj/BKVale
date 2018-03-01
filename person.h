#ifndef PERSON_H
#define PERSON_H

#include "image.h"
#include <QObject>
#include <QString>
#include <QVector>

#define INVALID_PERSON_ID                 -1
#define INVALID_PERSON_DATE               -1

#define MAX_PERSON_NAME_LENGTH            35
#define MAX_PERSON_DETAILS_LENGTH         35
#define EMPLOYEE_PINCODE_LENGTH            4

struct Person
{
  mutable int m_id;
  int m_imageId;
  QString m_name;
  QString m_alias;
  QString m_email;
  QString m_CPF_CNPJ;
  QString m_RG_IE;
  QString m_details;
  qint64 m_birthDate;
  qint64 m_creationDate;
  bool m_bCompany;
  bool m_bCustomer;
  bool m_bSupplier;
  bool m_bEmployee;

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
    m_birthDate = INVALID_PERSON_DATE;
    m_creationDate = INVALID_PERSON_DATE;
    m_bCompany = false;
    m_bCustomer = false;
    m_bSupplier = false;
    m_bEmployee = false;
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

    return b;
  }

  bool operator ==(const Person& other) const
  {
    return !(*this != other);
  }

  static bool st_isValid(const Person& person)
  {
    return !person.m_name.isEmpty();
  }

  bool isValid() const { return st_isValid(*this); }
  static bool st_isValidId(int id) { return id != INVALID_PERSON_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static QString getTableName() { return "_PERSONS"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_IMAGEID", QObject::tr("Id Imagem")));
    c.push_back(JTableColumn("_NAME", QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn("_ALIAS", QObject::tr("Apelido"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn("_EMAIL", QObject::tr("Email")));
    c.push_back(JTableColumn("_CPF", QObject::tr("CPF")));
    c.push_back(JTableColumn("_RG", QObject::tr("RG")));
    c.push_back(JTableColumn("_CNPJ", QObject::tr("CNPJ")));
    c.push_back(JTableColumn("_INSC", QObject::tr("INSC")));
    c.push_back(JTableColumn("_DETAILS", QObject::tr("Detalhes")));
    c.push_back(JTableColumn("_BIRTHDATE", QObject::tr("Data Nascimento")));
    c.push_back(JTableColumn("_IS_COMPANY", QObject::tr("Jurídica"), false));
    c.push_back(JTableColumn("_IS_CUSTOMER", QObject::tr("Cliente"), false));
    c.push_back(JTableColumn("_IS_SUPPLIER", QObject::tr("Fornecedor"), false));
    c.push_back(JTableColumn("_IS_EMPLOYEE", QObject::tr("Funcionário"), false));
    return c;
  }
};

#endif // PERSON_H
