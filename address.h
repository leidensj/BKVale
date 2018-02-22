#ifndef ADDRESS_H
#define ADDRESS_H

#include "jtablecolumn.h"
#include "person.h"
#include <QObject>
#include <QString>

#define INVALID_ADDRESS_ID     -1
#define INVALID_ADDRESS_NUMBER 0

#define MAX_ADDRESS_STREET_LENGTH         35
#define MAX_ADDRESS_NUMBER                35

struct Address
{
  mutable int m_id;
  int m_personId;
  QString m_cep;
  QString m_neighborhood;
  QString m_street;
  int m_number;
  QString m_astate;
  QString m_country;
  QString m_complement;
  QString m_reference;

  void clear()
  {
    m_id = INVALID_ADDRESS_ID;
    m_personId = INVALID_PERSON_ID;
    m_addressCep.clear();
    m_addressNeighborhood.clear();
    m_addressStreet.clear();
    m_addressNumber = INVALID_ADDRESS_NUMBER;
    m_addressState.clear();
    m_addressCountry.clear();
    m_addressComplement.clear();
    m_addressReference.clear();
  }

  Address()
  {
    clear();
  }

  bool operator !=(const Address& other)
  {
    return
        m_addressCep != other.m_addressCep ||
        m_addressNeighborhood != other.m_addressNeighborhood ||
        m_addressStreet != other.m_addressStreet ||
        m_addressNumber != other.m_addressNumber ||
        m_addressState != other.m_addressState ||
        m_addressCountry != other.m_addressCountry ||
        m_addressComplement != other.m_addressComplement ||
        m_addressReference != other.m_addressReference;
  }

  static bool st_isValid(const Address& address)
  {
    return
        m_personId != INVALID_PERSON_ID &&
        !address.m_addressNeighborhood.isEmpty() &&
        !address.m_addressStreet.isEmpty() &&
        address.m_addressNumber != INVALID_ADDRESS_NUMBER &&
        !address.m_addressState.isEmpty() &&
        !address.m_addressCountry.isEmpty();
  }

  bool isValid() const { return st_isValid(*this); }
  static bool st_isValidId(int id) { return id != INVALID_ADDRESS_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static QString getTableName() { return "_ADDRESSES"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_PERSONID", QObject::tr("Pessoa"), false, true));
    c.push_back(JTableColumn("_CEP", QObject::tr("CEP")));
    c.push_back(JTableColumn("_NEIGHBORHOOD", QObject::tr("Bairro"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn("_STREET", QObject::tr("Rua"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn("_NUMBER", QObject::tr("Email")));
    c.push_back(JTableColumn("_STATE", QObject::tr("CPF")));
    c.push_back(JTableColumn("_COUNTRY", QObject::tr("RG")));
    c.push_back(JTableColumn("_COMPLEMENT", QObject::tr("CNPJ")));
    c.push_back(JTableColumn("_REFERENCE", QObject::tr("INSC")));
    return c;
  }
};

#endif // ADDRESS_H
