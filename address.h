#ifndef ADDRESS_H
#define ADDRESS_H

#include "jtablecolumn.h"
#include <QObject>
#include <QString>

#define INVALID_ADDRESS_ID     -1
#define INVALID_ADDRESS_NUMBER 0

#define MAX_ADDRESS_STREET_LENGTH         35
#define MAX_ADDRESS_NUMBER                35
#define NUMBER_OF_BRAZILIAN_STATES        27

struct Address
{
  mutable int m_id;
  QString m_cep;
  QString m_neighborhood;
  QString m_street;
  int m_number;
  QString m_state;
  QString m_complement;
  QString m_reference;

  void clear()
  {
    m_id = INVALID_ADDRESS_ID;
    m_cep.clear();
    m_neighborhood.clear();
    m_street.clear();
    m_number = INVALID_ADDRESS_NUMBER;
    m_state.clear();
    m_complement.clear();
    m_reference.clear();
  }

  Address()
  {
    clear();
  }

  bool operator !=(const Address& other)
  {
    return
        m_cep != other.m_cep ||
        m_neighborhood != other.m_neighborhood ||
        m_street != other.m_street ||
        m_number != other.m_number ||
        m_state != other.m_state ||
        m_complement != other.m_complement ||
        m_reference != other.m_reference;
  }

  static bool st_isValid(const Address& address)
  {
    return
        !address.m_neighborhood.isEmpty() &&
        !address.m_street.isEmpty() &&
        address.m_number != INVALID_ADDRESS_NUMBER &&
        !address.m_state.isEmpty();
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

  enum class EBRState
  {
    AC, AL, AP, AM, BA, CE, DF,
    ES, GO, MA, MT, MS, MG, PA,
    PB, PR, PE, PI, RJ, RN, RS,
    RO, RR, SC, SP, SE, TO
  };

  struct BRState
  {
    QString m_abv;
    QString m_name;
    QString m_mask;
  };

  static BRState st_BRState(EBRState s)
  {
    switch (s)
    {
      case EBRState::RS:
        return BRState{"RS", "RIO GRANDE DO SUL", "999-9999999;_"};
      case EBRState::SC:
        return BRState{"SC", "SANTA CATARINA", "999.999.999;_"};
      case EBRState::PR:
        return BRState{"PR", "PARANA", "99999999-99;_"};
      case EBRState::SP:
        return BRState{"SP", "SAO PAULO", "999.999.999.999;_"};
      case EBRState::MG:
        return BRState{"MG", "MINAS GERAIS", "999.999.999/9999;_"};
      case EBRState::RJ:
        return BRState{"RJ", "RIO DE JANEIRO", "99.999.99-9;_"};
      case EBRState::ES:
        return BRState{"ES", "ESPIRITO SANTO", "999.999.99-9;_"};
      case EBRState::BA:
        return BRState{"BA", "BAHIA", "999.999.99-9;_"};
      case EBRState::SE:
        return BRState{"SE", "SERGIPE", "999999999-9;_"};
      case EBRState::AL:
        return BRState{"AL", "ALAGOAS", "999999999;_"};
      case EBRState::PE:
        return BRState{"PE", "PERNAMBUCO", "99.9.999.9999999-9;_"};
      case EBRState::PB:
        return BRState{"PB", "PARAIBA", "99999999-9;_"};
      case EBRState::RN:
        return BRState{"RN", "RIO GRANDE DO NORTE", "99.999.999-9;_"};
      case EBRState::PI:
        return BRState{"PI", "PIAUI", "999999999;_"};
      case EBRState::MA:
        return BRState{"MA", "MARANHAO", "999999999;_"};
      case EBRState::CE:
        return BRState{"CE", "CEARA", "99999999-9;_"};
      case EBRState::GO:
        return BRState{"GO", "GOIAS", "99.999.999-9;_"};
      case EBRState::TO:
        return BRState{"TO", "TOCANTINS", "99999999999;_"};
      case EBRState::MT:
        return BRState{"MT", "MATO GROSSO", "999999999;_"};
      case EBRState::MS:
        return BRState{"MS", "MATO GROSSO DO SUL", "999999999;_"};
      case EBRState::DF:
        return BRState{"DF", "DISTRITO FEDERAL", "99999999999-99;_"};
      case EBRState::AM:
        return BRState{"AM", "AMZONAS", "99.999.999-9;_"};
      case EBRState::AC:
        return BRState{"AC", "ACRE", "99.999.999/999-99;_"};
      case EBRState::PA:
        return BRState{"PA", "PARA", "99-999999-9;_"};
      case EBRState::RO:
        return BRState{"RO", "RONDONIA", "999.99999-9;_"};
      case EBRState::RR:
        return BRState{"RR", "RORAIMA", "99999999-9;_"};
      case EBRState::AP:
        return BRState{"AP", "AMAPA", "999999999;_"};
      default:
        return BRState{"", "", ""};
    }
  }
};

#endif // ADDRESS_H
