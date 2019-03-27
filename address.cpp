#include "address.h"

void Address::clear()
{
  m_id.clear();
  m_cep.clear();
  m_neighborhood.clear();
  m_street.clear();
  m_number = ADDRESS_STARTING_NUMBER;
  m_state = EBRState::RS;
  m_complement.clear();
  m_reference.clear();
}

Address::Address()
{
  clear();
}

QString Address::getFormattedAddress() const
{
  return m_street + ", Nº " +
      QString::number(m_number) + ". " +
      m_city + " - " +
      getBRState().m_abv + ".";
}

QString Address::getFormattedAddress2() const
{
  return m_street + ", Nº " +
      QString::number(m_number) + ". " +
      m_neighborhood;
}

QString Address::getFormattedAddress3() const
{
  return m_city + " / " + getBRState().m_abv;
}

bool Address::operator !=(const JItem& other) const
{
  const Address& another = dynamic_cast<const Address&>(other);
  return
      m_cep != another.m_cep ||
      m_neighborhood != another.m_neighborhood ||
      m_street != another.m_street ||
      m_number != another.m_number ||
      m_state != another.m_state ||
      m_complement != another.m_complement ||
      m_reference != another.m_reference;
}

bool Address::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Address::isValid() const
{
  return
      !m_neighborhood.isEmpty() &&
      !m_street.isEmpty();
}

QString Address::strTableName() const
{
  return ADDRESS_SQL_TABLE_NAME;
}

Address::EBRState Address::st_getEBRState(const QString& abv)
{
  if (abv == "RS")
    return EBRState::RS;
  else if (abv == "AC")
    return EBRState::AC;
  else if (abv == "AL")
    return EBRState::AL;
  else if (abv == "AP")
    return EBRState::AP;
  else if (abv == "AM")
    return EBRState::AM;
  else if (abv == "BA")
    return EBRState::BA;
  else if (abv == "CE")
    return EBRState::CE;
  else if (abv == "DF")
    return EBRState::DF;
  else if (abv == "ES")
    return EBRState::ES;
  else if (abv == "GO")
    return EBRState::GO;
  else if (abv == "MA")
    return EBRState::MA;
  else if (abv == "MT")
    return EBRState::MT;
  else if (abv == "MS")
    return EBRState::MS;
  else if (abv == "MG")
    return EBRState::MG;
  else if (abv == "PA")
    return EBRState::PA;
  else if (abv == "PB")
    return EBRState::PB;
  else if (abv == "PR")
    return EBRState::PR;
  else if (abv == "PE")
    return EBRState::PE;
  else if (abv == "PI")
    return EBRState::PI;
  else if (abv == "RJ")
    return EBRState::RJ;
  else if (abv == "RN")
    return EBRState::RN;
  else if (abv == "RO")
    return EBRState::RO;
  else if (abv == "RR")
    return EBRState::RR;
  else if (abv == "SC")
    return EBRState::SC;
  else if (abv == "SP")
    return EBRState::SP;
  else if (abv == "SE")
    return EBRState::SE;
  else if (abv == "TO")
    return EBRState::TO;
  else return EBRState::RS;
}

Address::BRState Address::st_getBRState(EBRState s)
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

Address::BRState Address::getBRState() const
{
  return st_getBRState(m_state);
}
