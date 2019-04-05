#include "form.h"
#include "jmodel.h"

class FormModel : public JModel
{
public:
  FormModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     FORM_SQL_COL02 ","
                     FORM_SQL_COL03
                     " FROM "
                     FORM_SQL_TABLE_NAME);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    setHeaderData(2, Qt::Horizontal, tr("Apelido"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    }
  }
};

Form::Form()
{
  clear();
}

void Form::clear()
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
  m_vPhone.clear();
  m_vAddress.clear();
  m_bBirth = false;
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
            m_dtBirth != another.m_dtBirth;
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
                FORM_SQL_COL01 ","
                FORM_SQL_COL02 ","
                FORM_SQL_COL03 ","
                FORM_SQL_COL04 ","
                FORM_SQL_COL05 ","
                FORM_SQL_COL06 ","
                FORM_SQL_COL07 ","
                FORM_SQL_COL08 ","
                FORM_SQL_COL09 ","
                FORM_SQL_COL10 ","
                FORM_SQL_COL11 ")"
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

bool Form::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " FORM_SQL_TABLE_NAME " SET "
                FORM_SQL_COL01 " = (:_v01),"
                FORM_SQL_COL02 " = (:_v02),"
                FORM_SQL_COL03 " = (:_v03),"
                FORM_SQL_COL04 " = (:_v04),"
                FORM_SQL_COL05 " = (:_v05),"
                FORM_SQL_COL06 " = (:_v06),"
                FORM_SQL_COL07 " = (:_v07),"
                FORM_SQL_COL08 " = (:_v08),"
                FORM_SQL_COL09 " = (:_v09),"
                FORM_SQL_COL10 " = (:_v10),"
                FORM_SQL_COL11 " = (:_v11)"
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

bool Form::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                FORM_SQL_COL01 ","
                FORM_SQL_COL02 ","
                FORM_SQL_COL03 ","
                FORM_SQL_COL04 ","
                FORM_SQL_COL05 ","
                FORM_SQL_COL06 ","
                FORM_SQL_COL07 ","
                FORM_SQL_COL08 ","
                FORM_SQL_COL09 ","
                FORM_SQL_COL10 ","
                FORM_SQL_COL11
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

bool Form::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " FORM_SQL_TABLE_NAME " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

JModel* Form::SQL_table_model(QObject* parent) const
{
  return new FormModel(parent);
}
