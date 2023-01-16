#include "form.h"

Form::Form()
{
  clear();
}

void Form::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_image.clear();
  m_name.clear();
  m_alias.clear();
  m_email.clear();
  m_CPF_CNPJ.clear();
  m_RG_IE.clear();
  m_details.clear();
  m_dtBirth = QDate::currentDate();
  m_dtCreation = DateTime::server().date();
  m_bCompany = false;
  m_vPhone.clear();
  m_vAddress.clear();
  m_bBirth = false;
  m_bSex = false;
}

bool Form::operator !=(const JItem& other) const
{
  const Form& another = dynamic_cast<const Form&>(other);
  bool b =  m_image.m_id != another.m_image.m_id ||
            m_name != another.m_name ||
            m_alias != another.m_alias ||
            m_email != another.m_email ||
            m_CPF_CNPJ != another.m_CPF_CNPJ ||
            m_RG_IE != another.m_RG_IE ||
            m_details != another.m_details ||
            m_bCompany != another.m_bCompany ||
            m_vPhone != another.m_vPhone ||
            m_vAddress != another.m_vAddress ||
            m_dtBirth != another.m_dtBirth ||
            m_bSex != another.m_bSex;
  return b;
}

bool Form::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Form::isValid() const
{
  bool b = !m_name.isEmpty();
  return b;
}

QString Form::strAliasName() const
{
  return m_alias.isEmpty() ? m_name : m_alias;
}

QString Form::SQL_tableName() const
{
  return FORM_SQL_TABLE_NAME;
}

bool Form::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " FORM_SQL_TABLE_NAME " ("
                FORM_SQL_COL_IID ","
                FORM_SQL_COL_NAM ","
                FORM_SQL_COL_ALI ","
                FORM_SQL_COL_EMA ","
                FORM_SQL_COL_CPF ","
                FORM_SQL_COL_RGI ","
                FORM_SQL_COL_DET ","
                FORM_SQL_COL_BIR ","
                FORM_SQL_COL_CRE ","
                FORM_SQL_COL_ISC ","
                FORM_SQL_COL_HBI ","
                FORM_SQL_COL_SEX ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07),"
                "(:_v08),"
                "(:_v09),"
                "(:_v10),"
                "(:_v11),"
                "(:_v12))");
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", m_alias);
  query.bindValue(":_v04", m_email);
  query.bindValue(":_v05", m_CPF_CNPJ);
  query.bindValue(":_v06", m_RG_IE);
  query.bindValue(":_v07", m_details);
  query.bindValue(":_v08", m_dtBirth);
  query.bindValue(":_v09", m_dtCreation);
  query.bindValue(":_v10", m_bCompany);
  query.bindValue(":_v11", m_bBirth);
  query.bindValue(":_v12", m_bSex);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());

    for (int i = 0; i != m_vPhone.size(); ++i)
    {
      query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                    PHONE_SQL_COL_FID ","
                    PHONE_SQL_COL_COU ","
                    PHONE_SQL_COL_COD ","
                    PHONE_SQL_COL_NUM ","
                    PHONE_SQL_COL_NAM ")"
                                    " VALUES ("
                                    "(:_v01),"
                                    "(:_v02),"
                                    "(:_v03),"
                                    "(:_v04),"
                                    "(:_v05))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", m_vPhone.at(i).m_code);
      query.bindValue(":_v04", m_vPhone.at(i).m_number);
      query.bindValue(":_v05", m_vPhone.at(i).m_name);
      bSuccess = query.exec();
      if (bSuccess)
        m_vPhone.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }

    if (bSuccess)
    {
      for (int i = 0; i != m_vAddress.size(); ++i)
      {
        query.prepare("INSERT INTO " ADDRESS_SQL_TABLE_NAME " ("
                      ADDRESS_SQL_COL_FID ","
                      ADDRESS_SQL_COL_CEP ","
                      ADDRESS_SQL_COL_NEI ","
                      ADDRESS_SQL_COL_STR ","
                      ADDRESS_SQL_COL_NUM ","
                      ADDRESS_SQL_COL_CIT ","
                      ADDRESS_SQL_COL_STA ","
                      ADDRESS_SQL_COL_COM ","
                      ADDRESS_SQL_COL_REF ")"
                      " VALUES ("
                      "(:_v01),"
                      "(:_v02),"
                      "(:_v03),"
                      "(:_v04),"
                      "(:_v05),"
                      "(:_v06),"
                      "(:_v07),"
                      "(:_v08),"
                      "(:_v09))");
        query.bindValue(":_v01", m_id.get());
        query.bindValue(":_v02", m_vAddress.at(i).m_cep);
        query.bindValue(":_v03", m_vAddress.at(i).m_neighborhood);
        query.bindValue(":_v04", m_vAddress.at(i).m_street);
        query.bindValue(":_v05", m_vAddress.at(i).m_number);
        query.bindValue(":_v06", m_vAddress.at(i).m_city);
        query.bindValue(":_v07", (int)m_vAddress.at(i).m_state);
        query.bindValue(":_v08", m_vAddress.at(i).m_complement);
        query.bindValue(":_v09", m_vAddress.at(i).m_reference);
        bSuccess = query.exec();
        if (bSuccess)
          m_vAddress.at(i).m_id.set(query.lastInsertId().toLongLong());
        else
          break;
      }
    }
  }

  return bSuccess;
}

bool Form::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " FORM_SQL_TABLE_NAME " SET "
                FORM_SQL_COL_IID " = (:_v01),"
                FORM_SQL_COL_NAM " = (:_v02),"
                FORM_SQL_COL_ALI " = (:_v03),"
                FORM_SQL_COL_EMA " = (:_v04),"
                FORM_SQL_COL_CPF " = (:_v05),"
                FORM_SQL_COL_RGI " = (:_v06),"
                FORM_SQL_COL_DET " = (:_v07),"
                FORM_SQL_COL_BIR " = (:_v08),"
                FORM_SQL_COL_CRE " = (:_v09),"
                FORM_SQL_COL_ISC " = (:_v10),"
                FORM_SQL_COL_HBI " = (:_v11),"
                FORM_SQL_COL_SEX " = (:_v12)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);
  query.bindValue(":_v03", m_alias);
  query.bindValue(":_v04", m_email);
  query.bindValue(":_v05", m_CPF_CNPJ);
  query.bindValue(":_v06", m_RG_IE);
  query.bindValue(":_v07", m_details);
  query.bindValue(":_v08", m_dtBirth);
  query.bindValue(":_v09", m_dtCreation);
  query.bindValue(":_v10", m_bCompany);
  query.bindValue(":_v11", m_bBirth);
  query.bindValue(":_v12", m_bSex);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " ADDRESS_SQL_TABLE_NAME " WHERE " ADDRESS_SQL_COL_FID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " PHONE_SQL_TABLE_NAME " WHERE " PHONE_SQL_COL_FID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vPhone.size(); ++i)
    {
      query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                    PHONE_SQL_COL_FID ","
                    PHONE_SQL_COL_COU ","
                    PHONE_SQL_COL_COD ","
                    PHONE_SQL_COL_NUM ","
                    PHONE_SQL_COL_NAM ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vPhone.at(i).m_countryCode);
      query.bindValue(":_v03", m_vPhone.at(i).m_code);
      query.bindValue(":_v04", m_vPhone.at(i).m_number);
      query.bindValue(":_v05", m_vPhone.at(i).m_name);
      bSuccess = query.exec();
      if (bSuccess)
        m_vPhone.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vAddress.size(); ++i)
    {
      query.prepare("INSERT INTO " ADDRESS_SQL_TABLE_NAME " ("
                    ADDRESS_SQL_COL_FID ","
                    ADDRESS_SQL_COL_CEP ","
                    ADDRESS_SQL_COL_NEI ","
                    ADDRESS_SQL_COL_STR ","
                    ADDRESS_SQL_COL_NUM ","
                    ADDRESS_SQL_COL_CIT ","
                    ADDRESS_SQL_COL_STA ","
                    ADDRESS_SQL_COL_COM ","
                    ADDRESS_SQL_COL_REF ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05),"
                    "(:_v06),"
                    "(:_v07),"
                    "(:_v08),"
                    "(:_v09))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vAddress.at(i).m_cep);
      query.bindValue(":_v03", m_vAddress.at(i).m_neighborhood);
      query.bindValue(":_v04", m_vAddress.at(i).m_street);
      query.bindValue(":_v05", m_vAddress.at(i).m_number);
      query.bindValue(":_v06", m_vAddress.at(i).m_city);
      query.bindValue(":_v07", (int)m_vAddress.at(i).m_state);
      query.bindValue(":_v08", m_vAddress.at(i).m_complement);
      query.bindValue(":_v09", m_vAddress.at(i).m_reference);
      bSuccess = query.exec();
      if (bSuccess)
        m_vAddress.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return bSuccess;
}

bool Form::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                FORM_SQL_COL_IID ","
                FORM_SQL_COL_NAM ","
                FORM_SQL_COL_ALI ","
                FORM_SQL_COL_EMA ","
                FORM_SQL_COL_CPF ","
                FORM_SQL_COL_RGI ","
                FORM_SQL_COL_DET ","
                FORM_SQL_COL_BIR ","
                FORM_SQL_COL_CRE ","
                FORM_SQL_COL_ISC ","
                FORM_SQL_COL_HBI ","
                FORM_SQL_COL_SEX
                " FROM " FORM_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_image.m_id.set(query.value(0).toLongLong());
      m_name = query.value(1).toString();
      m_alias = query.value(2).toString();
      m_email = query.value(3).toString();
      m_CPF_CNPJ = query.value(4).toString();
      m_RG_IE = query.value(5).toString();
      m_details = query.value(6).toString();
      m_dtBirth = query.value(7).toDate();
      m_dtCreation = query.value(8).toDate();
      m_bCompany = query.value(9).toBool();
      m_bBirth = query.value(10).toBool();
      m_bSex = query.value(11).toBool();
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " ADDRESS_SQL_TABLE_NAME
                  " WHERE " ADDRESS_SQL_COL_FID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    Ids ids;
    while (bSuccess && query.next())
      ids.push_back(Id(query.value(0).toLongLong()));

    for (auto id : ids)
    {
      Address o(id);
      bSuccess = o.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
      m_vAddress.push_back(o);
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " PHONE_SQL_TABLE_NAME
                  " WHERE " PHONE_SQL_COL_FID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    Ids ids;
    while (bSuccess && query.next())
      ids.push_back(Id(query.value(0).toLongLong()));

    for (auto id : ids)
    {
      Phone o(id);
      bSuccess = o.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
      m_vPhone.push_back(o);
    }
  }

  if (bSuccess && m_image.m_id.isValid())
    bSuccess = m_image.SQL_select_proc(query, error);

  return bSuccess;
}

bool Form::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " FORM_SQL_TABLE_NAME " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}
