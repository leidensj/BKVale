#ifndef PERSON_H
#define PERSON_H

#include "image.h"
#include <QObject>
#include <QString>
#include <QVector>

#define INVALID_PERSON_ID   -1

#define MAX_PERSON_NAME_LENGTH            35
#define MAX_PERSON_DETAILS_LENGTH         35

struct Person
{
  mutable int m_id;
  int m_imageId;
  QString m_name;
  QString m_alias;
  QString m_email;
  QString m_cpf;
  QString m_rg;
  QString m_cnpj;
  QString m_insc;
  QString m_details;
  qint64 m_birthDate;
  bool m_bCompany;
  bool m_bClient;
  bool m_bSupplier;
  bool m_bEmployee;

  void clear()
  {
    m_id = INVALID_PERSON_ID;
    m_imageId = INVALID_IMAGE_ID;
    m_name.clear();
    m_alias.clear();
    m_email.clear();
    m_cpf.clear();
    m_rg.clear();
    m_cnpj.clear();
    m_insc.clear();
    m_details.clear();
    m_birthDate = 0;
    m_bCompany = false;
    m_bClient = false;
    m_bSupplier = false;
    m_bEmployee = false;
  }

  Person()
  {
    clear();
  }

  bool operator !=(const Person& other)
  {
    bool b =  m_imageId != other.m_imageId ||
              m_name != other.m_name ||
              m_alias != other.m_alias ||
              m_email != other.m_email ||
              m_details != other.m_details;
    if (m_bCompany)
    {
      b = b ||
          m_cnpj != other.m_cnpj ||
          m_insc != other.m_insc;
    }
    else
    {
      b = b ||
          m_cpf != other.m_cpf ||
          m_rg != other.m_rg ||
          m_birthDate != other.m_birthDate;
    }
    return b;
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
    c.push_back(JTableColumn("_IS_CLIENT", QObject::tr("Cliente"), false));
    c.push_back(JTableColumn("_IS_SUPPLIER", QObject::tr("Fornecedor"), false));
    c.push_back(JTableColumn("_IS_EMPLOYEE", QObject::tr("Funcionário"), false));
    return c;
  }
};

#endif // PERSON_H
