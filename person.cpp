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

QString Person::strAliasName() const
{
  return m_alias.isEmpty() ? m_name : m_alias;
}

QString Person::SQL_tableName() const
{
  return PERSON_SQL_TABLE_NAME;
}

bool Person::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PERSON_SQL_TABLE_NAME " ("
                PERSON_SQL_COL01 ","
                PERSON_SQL_COL02 ","
                PERSON_SQL_COL03 ","
                PERSON_SQL_COL04 ","
                PERSON_SQL_COL05 ","
                PERSON_SQL_COL06 ","
                PERSON_SQL_COL07 ","
                PERSON_SQL_COL08 ","
                PERSON_SQL_COL09 ","
                PERSON_SQL_COL10 ","
                PERSON_SQL_COL11 ")"
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
                "(:_v11))");
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

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());

    if (m_employee.m_bIsEmployee)
    {
      query.prepare("INSERT INTO " EMPLOYEE_SQL_TABLE_NAME " ("
                    EMPLOYEE_SQL_COL01 ","
                    EMPLOYEE_SQL_COL02 ","
                    EMPLOYEE_SQL_COL03 ","
                    EMPLOYEE_SQL_COL04
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_employee.getPincodeNull());
      query.bindValue(":_v03", m_employee.m_bNoteEdit);
      query.bindValue(":_v04", m_employee.m_bNoteRemove);
      bSuccess = query.exec();
    }

    if (bSuccess && m_supplier.m_bIsSupplier)
    {
      query.prepare("INSERT INTO " SUPPLIER_SQL_TABLE_NAME " ("
                    SUPPLIER_SQL_COL01 ")"
                    " VALUES ("
                    "(:_v01))");
      query.bindValue(":_v01", m_id.get());
      bSuccess = query.exec();
    }

    if (bSuccess)
    {
      for (int i = 0; i != m_vPhone.size(); ++i)
      {
        query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                      PHONE_SQL_COL01 ","
                      PHONE_SQL_COL02 ","
                      PHONE_SQL_COL03 ","
                      PHONE_SQL_COL04 ","
                      PHONE_SQL_COL05 ")"
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
                      ADDRESS_SQL_COL01 ","
                      ADDRESS_SQL_COL02 ","
                      ADDRESS_SQL_COL03 ","
                      ADDRESS_SQL_COL04 ","
                      ADDRESS_SQL_COL05 ","
                      ADDRESS_SQL_COL06 ","
                      ADDRESS_SQL_COL07 ","
                      ADDRESS_SQL_COL08 ","
                      ADDRESS_SQL_COL09 ")"
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

bool Person::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " PERSON_SQL_TABLE_NAME " SET "
                PERSON_SQL_COL01 " = (:_v01),"
                PERSON_SQL_COL02 " = (:_v02),"
                PERSON_SQL_COL03 " = (:_v03),"
                PERSON_SQL_COL04 " = (:_v04),"
                PERSON_SQL_COL05 " = (:_v05),"
                PERSON_SQL_COL06 " = (:_v06),"
                PERSON_SQL_COL07 " = (:_v07),"
                PERSON_SQL_COL08 " = (:_v08),"
                PERSON_SQL_COL09 " = (:_v09),"
                PERSON_SQL_COL10 " = (:_v10),"
                PERSON_SQL_COL11 " = (:_v11)"
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
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " EMPLOYEE_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " SUPPLIER_SQL_TABLE_NAME
                  " WHERE " SUPPLIER_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess && m_employee.m_bIsEmployee)
  {
    query.prepare("INSERT INTO " EMPLOYEE_SQL_TABLE_NAME " ("
                  EMPLOYEE_SQL_COL01 ","
                  EMPLOYEE_SQL_COL02 ","
                  EMPLOYEE_SQL_COL03 ","
                  EMPLOYEE_SQL_COL04
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04))");
    query.bindValue(":_v01", m_id.get());
    query.bindValue(":_v02", m_employee.getPincodeNull());
    query.bindValue(":_v03", m_employee.m_bNoteEdit);
    query.bindValue(":_v04", m_employee.m_bNoteRemove);
    bSuccess = query.exec();
  }

  if (bSuccess && m_supplier.m_bIsSupplier)
  {
    query.prepare("INSERT INTO " SUPPLIER_SQL_TABLE_NAME " ("
                  SUPPLIER_SQL_COL01 ")"
                  " VALUES ("
                  "(:_v01))");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " ADDRESS_SQL_TABLE_NAME " WHERE " ADDRESS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " PHONE_SQL_TABLE_NAME " WHERE " PHONE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vPhone.size(); ++i)
    {
      query.prepare("INSERT INTO " PHONE_SQL_TABLE_NAME " ("
                    PHONE_SQL_COL01 ","
                    PHONE_SQL_COL02 ","
                    PHONE_SQL_COL03 ","
                    PHONE_SQL_COL04 ","
                    PHONE_SQL_COL05 ")"
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
                    ADDRESS_SQL_COL01 ","
                    ADDRESS_SQL_COL02 ","
                    ADDRESS_SQL_COL03 ","
                    ADDRESS_SQL_COL04 ","
                    ADDRESS_SQL_COL05 ","
                    ADDRESS_SQL_COL06 ","
                    ADDRESS_SQL_COL07 ","
                    ADDRESS_SQL_COL08 ","
                    ADDRESS_SQL_COL09 ")"
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

bool Person::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                PERSON_SQL_COL01 ","
                PERSON_SQL_COL02 ","
                PERSON_SQL_COL03 ","
                PERSON_SQL_COL04 ","
                PERSON_SQL_COL05 ","
                PERSON_SQL_COL06 ","
                PERSON_SQL_COL07 ","
                PERSON_SQL_COL08 ","
                PERSON_SQL_COL09 ","
                PERSON_SQL_COL10 ","
                PERSON_SQL_COL11
                " FROM " PERSON_SQL_TABLE_NAME
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
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  EMPLOYEE_SQL_COL02 ","
                  EMPLOYEE_SQL_COL03 ","
                  EMPLOYEE_SQL_COL04
                  " FROM " EMPLOYEE_SQL_TABLE_NAME
                  " WHERE " EMPLOYEE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess && query.next())
    {
      m_employee.m_bIsEmployee = true;
      m_employee.m_pincode = query.value(0).toString();
      m_employee.m_bNoteEdit = query.value(1).toBool();
      m_employee.m_bNoteRemove = query.value(2).toBool();
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT * "
                  " FROM " SUPPLIER_SQL_TABLE_NAME
                  " WHERE " SUPPLIER_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess && query.next())
    {
      m_supplier.m_bIsSupplier = true;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID
                  " FROM " ADDRESS_SQL_TABLE_NAME
                  " WHERE " ADDRESS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    QVector<Id> ids;
    while (bSuccess && query.next())
      ids.push_back(Id(query.value(0).toLongLong()));

    for (int i = 0; i != ids.size(); ++i)
    {
      Address o;
      o.m_id.set(ids.at(i).get());
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
                  " WHERE " PHONE_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    QVector<Id> ids;
    while (bSuccess && query.next())
      ids.push_back(Id(query.value(0).toLongLong()));

    for (int i = 0; i != ids.size(); ++i)
    {
      Phone o;
      o.m_id.set(ids.at(i).get());
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

bool Person::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " PERSON_SQL_TABLE_NAME " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool Person::SQL_select_by_pincode(const QString& pincode, QString& error)
{
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_select_by_pincode_proc(query, pincode, error);
  return SQL_finish(db, query, bSuccess, error);
}


bool Person::SQL_select_by_pincode_proc(QSqlQuery& query, const QString& pincode, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                EMPLOYEE_SQL_COL01
                " FROM " EMPLOYEE_SQL_TABLE_NAME
                " WHERE " EMPLOYEE_SQL_COL02 " = (:_v02) LIMIT 1");
  query.bindValue(":_v02", pincode);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
      bSuccess = SQL_select_proc(query, error);
    }
    else
    {
      error = "Código PIN informado não encontrado.";
      bSuccess = false;
    }
  }

  return bSuccess;
}
