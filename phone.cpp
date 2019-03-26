#include "phone.h"

Phone::Phone()
{
  clear();
}

void Phone::clear()
{
  m_id.clear();
  m_countryCode = PHONE_DEFAULT_COUNTRY_CODE_VALUE;
  m_code = PHONE_DEFAULT_CODE_VALUE;
  m_number.clear();
  m_name.clear();
}

bool Phone::operator !=(const JItem& other) const
{
  const Phone& another = dynamic_cast<const Phone&>(other);
  return
      m_countryCode != another.m_countryCode ||
      m_code != another.m_code ||
      m_number != another.m_number ||
      m_name != another.m_name;
}

bool Phone::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Phone::isValid() const
{
  return !m_number.isEmpty();
}

QString Phone::SQL_tableName() const
{
  return PHONE_SQL_TABLE_NAME;
}

bool Phone::SQL_insert_proc(QSqlQuery& /*query*/)
{
  return false;
}

bool Phone::SQL_update_proc(QSqlQuery& /*query*/)
{
  return false;
}

bool Phone::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                PHONE_SQL_COL01 ","
                PHONE_SQL_COL02 ","
                PHONE_SQL_COL03 ","
                PHONE_SQL_COL04 ","
                PHONE_SQL_COL05
                " FROM " PHONE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      query.value(0).toLongLong(); //personId não usamos
      m_countryCode = query.value(1).toInt();
      m_code = query.value(2).toInt();
      m_number = query.value(3).toString();
      m_name = query.value(4).toString();
    }
    else
    {
      error = "Telefone não encontrado.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool Phone::SQL_remove_proc(QSqlQuery& /*query*/)
{
  return false;
}

QString Phone::strFormattedPhone() const
{
  return
    "+" +
    QString::number(m_countryCode) +
    " (" +
    QString::number(m_code) +
    ") " +
    m_number;
}

QString Phone::strFormattedPhoneWithName() const
{
  QString str;
  if (!m_name.isEmpty())
    str += m_name + " ";
  str += strFormattedPhone();
  return str;
}
