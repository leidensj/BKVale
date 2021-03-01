#include "coupon.h"
#include <QSqlRecord>
#include <QRandomGenerator>

Coupon::Coupon(Id id)
{
  Coupon::clear();
  m_id = id;
}

void Coupon::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_type = Type::Percentage;
  m_code.clear();
  m_dtCreation = DateTime::server().date();;
  m_bRedeemed = false;
  m_dtRedeemed = m_dtCreation;
  m_bExpires = false;
  m_dtExpiration = m_dtCreation.addYears(1);
  m_percentage = 0;
  m_value = 0.0;
}

bool Coupon::operator != (const JItem& other) const
{
  const Coupon& another = dynamic_cast<const Coupon&>(other);
  return m_type != another.m_type ||
         m_code != another.m_code ||
         m_dtCreation != another.m_dtCreation ||
         m_bRedeemed != another.m_bRedeemed ||
         m_dtRedeemed != another.m_dtRedeemed ||
         m_bExpires != another.m_bExpires ||
         m_dtExpiration != another.m_dtExpiration ||
         m_percentage != another.m_percentage ||
         m_value != another.m_value;
}

bool Coupon::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Coupon::isValid() const
{
  return !m_code.isEmpty() &&
      m_type == Type::Percentage ? m_percentage > 0.0 && m_percentage <= 100.00 : true &&
      m_type == Type::Value ? m_value >= 0.0 : true &&
      m_bExpires ? DateTime::server().date() <= m_dtExpiration : true;
}

QString Coupon::SQL_tableName() const
{
  return COUPON_SQL_TABLE_NAME;
}

bool Coupon::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " COUPON_SQL_TABLE_NAME " ("
                COUPON_SQL_COL_TYP ","
                COUPON_SQL_COL_COD ","
                COUPON_SQL_COL_CDT ","
                COUPON_SQL_COL_RED ","
                COUPON_SQL_COL_RDT ","
                COUPON_SQL_COL_EXP ","
                COUPON_SQL_COL_EDT ","
                COUPON_SQL_COL_PCT ","
                COUPON_SQL_COL_VAL
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07),"
                "(:_v08),"
                "(:_v09))");
  query.bindValue(":_v01", (int)m_type);
  query.bindValue(":_v02", m_code);
  query.bindValue(":_v03", m_dtCreation);
  query.bindValue(":_v04", m_bRedeemed);
  query.bindValue(":_v05", m_dtRedeemed);
  query.bindValue(":_v06", m_bExpires);
  query.bindValue(":_v07", m_dtExpiration);
  query.bindValue(":_v08", m_percentage);
  query.bindValue(":_v09", m_value);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool Coupon::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " COUPON_SQL_TABLE_NAME " SET "
                COUPON_SQL_COL_TYP " = (:_v01),"
                COUPON_SQL_COL_COD " = (:_v02),"
                COUPON_SQL_COL_CDT " = (:_v03),"
                COUPON_SQL_COL_RED " = (:_v04),"
                COUPON_SQL_COL_RDT " = (:_v05),"
                COUPON_SQL_COL_EXP " = (:_v06),"
                COUPON_SQL_COL_EDT " = (:_v07),"
                COUPON_SQL_COL_PCT " = (:_v08),"
                COUPON_SQL_COL_VAL " = (:_v09)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", (int)m_type);
  query.bindValue(":_v02", m_code);
  query.bindValue(":_v03", m_dtCreation);
  query.bindValue(":_v04", m_bRedeemed);
  query.bindValue(":_v05", m_dtRedeemed);
  query.bindValue(":_v06", m_bExpires);
  query.bindValue(":_v07", m_dtExpiration);
  query.bindValue(":_v08", m_percentage);
  query.bindValue(":_v09", m_value);
  return query.exec();
}

bool Coupon::SQL_select_proc(QSqlQuery& query, QString& error)
{
  query.prepare("SELECT "
                COUPON_SQL_COL_TYP ","
                COUPON_SQL_COL_COD ","
                COUPON_SQL_COL_CDT ","
                COUPON_SQL_COL_RED ","
                COUPON_SQL_COL_RDT ","
                COUPON_SQL_COL_EXP ","
                COUPON_SQL_COL_EDT ","
                COUPON_SQL_COL_PCT ","
                COUPON_SQL_COL_VAL
                " FROM " COUPON_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_type = (Coupon::Type)query.value(0).toInt();
      m_code = query.value(1).toString();
      m_dtCreation = query.value(2).toDate();
      m_bRedeemed = query.value(3).toBool();
      m_dtRedeemed = query.value(4).toDate();
      m_bExpires = query.value(5).toBool();
      m_dtExpiration = query.value(6).toDate();
      m_percentage = query.value(7).toInt();
      m_value = query.value(8).toDouble();
    }
    else
    {
      error = "Coupom não encontrado.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool Coupon::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " COUPON_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

QString Coupon::st_strType(Type type)
{
  switch (type)
  {
    case Type::Percentage:
      return "Porcentagem";
    case Type::Value:
      return "Valor";
    case Type::Product:
      return "Produto";
    default:
      return "";
  }
}

QString Coupon::strType() const
{
  return st_strType(m_type);
}

QString Coupon::st_newCode()
{
  QString code;
  for (int i = 0; i != 10; ++i)
    code += QChar(QRandomGenerator::global()->bounded(65, 91));
  return code;
}
