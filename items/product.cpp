#include "product.h"

Package::Package()
{
  clear();
}

void Package::clear()
{
  m_bIsPackage = false;
  m_unity.clear();
  m_ammount = 1.0;
}

bool Package::operator !=(const Package& other) const
{
  return
      m_bIsPackage != other.m_bIsPackage ||
      m_unity != other.m_unity ||
      m_ammount != other.m_ammount;
}

QString Package::strFormattedUnity(const QString& productUnity) const
{
  return !m_bIsPackage
      ? productUnity
      : m_unity + " (" +
        QString::number(m_ammount, 'f', 3).remove(QRegularExpression("\\.?0*$")) +
        productUnity + ")";
}

QString Package::strUnity(const QString& productUnity) const
{
  return m_bIsPackage ? m_unity : productUnity;
}

ProductCode::ProductCode(Id id)
{
  clear();
  m_id = id;
}

void ProductCode::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_productId.clear();
  m_code.clear();
}

bool ProductCode::operator != (const JItem& other) const
{
  const ProductCode& another = dynamic_cast<const ProductCode&>(other);
  return m_code != another.m_code || m_productId != another.m_productId;
}

bool ProductCode::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool ProductCode::isValid() const
{
  return !m_code.isEmpty();
}

QString ProductCode::SQL_tableName() const
{
  return PRODUCT_CODE_ITEMS_SQL_TABLE_NAME;
}

bool ProductCode::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME " ("
                PRODUCT_CODE_ITEMS_SQL_COL_PID ","
                PRODUCT_CODE_ITEMS_SQL_COL_COD ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", m_productId.get());
  query.bindValue(":_v02", m_code);
  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool ProductCode::SQL_update_proc(QSqlQuery& /*query*/) const
{
  return false;
}

bool ProductCode::SQL_select_proc(QSqlQuery& /*query*/, QString& /*error*/)
{
  return false;
}

bool ProductCode::SQL_remove_proc(QSqlQuery& /*query*/) const
{
  return false;
}

Product::Product()
{
  clear();
}

void Product::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_category.clear();
  m_image.clear();
  m_name.clear();
  m_unity.clear();
  m_details.clear();
  m_bBuy = false;
  m_bSell = false;
  m_vCode.clear();
}

bool Product::operator != (const JItem& other) const
{
  const Product& another = dynamic_cast<const Product&>(other);
  return
      m_image.m_id != another.m_image.m_id ||
      m_category.m_id != another.m_category.m_id ||
      m_name != another.m_name ||
      m_unity != another.m_unity ||
      m_details != another.m_details ||
      m_bBuy != another.m_bBuy ||
      m_bSell != another.m_bSell ||
      m_vCode != another.m_vCode;
}

bool Product::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Product::isValid() const
{
  return
      !m_name.isEmpty() &&
      !m_unity.isEmpty();
}

QString Product::SQL_tableName() const
{
  return PRODUCT_SQL_TABLE_NAME;
}

bool Product::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PRODUCT_SQL_TABLE_NAME " ("
                PRODUCT_SQL_COL_NAM ","
                PRODUCT_SQL_COL_CID ","
                PRODUCT_SQL_COL_IID ","
                PRODUCT_SQL_COL_UNI ","
                PRODUCT_SQL_COL_DET ","
                PRODUCT_SQL_COL_ATB ","
                PRODUCT_SQL_COL_ATS ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");
  query.bindValue(":_v01", m_name);
  query.bindValue(":_v02", m_category.m_id.getIdNull());
  query.bindValue(":_v03", m_image.m_id.getIdNull());
  query.bindValue(":_v04", m_unity);
  query.bindValue(":_v05", m_details);
  query.bindValue(":_v06", m_bBuy);
  query.bindValue(":_v07", m_bSell);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    query.prepare("DELETE FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                  " WHERE " PRODUCT_CODE_ITEMS_SQL_COL_PID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      for (int i = 0; i != m_vCode.size(); ++i)
      {
        m_vCode[i].m_productId = m_id;
        bSuccess = m_vCode[i].SQL_insert_proc(query);
        if (!bSuccess)
          break;
      }
    }
  }

  return bSuccess;
}

bool Product::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " PRODUCT_SQL_TABLE_NAME " SET "
                PRODUCT_SQL_COL_NAM " = (:_v01),"
                PRODUCT_SQL_COL_CID " = (:_v02),"
                PRODUCT_SQL_COL_IID " = (:_v03),"
                PRODUCT_SQL_COL_UNI " = (:_v04),"
                PRODUCT_SQL_COL_DET " = (:_v05),"
                PRODUCT_SQL_COL_ATB " = (:_v06),"
                PRODUCT_SQL_COL_ATS " = (:_v07)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_name);
  query.bindValue(":_v02", m_category.m_id.getIdNull());
  query.bindValue(":_v03", m_image.m_id.getIdNull());
  query.bindValue(":_v04", m_unity);
  query.bindValue(":_v05", m_details);
  query.bindValue(":_v06", m_bBuy);
  query.bindValue(":_v07", m_bSell);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    query.prepare("DELETE FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                  " WHERE " PRODUCT_CODE_ITEMS_SQL_COL_PID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      for (int i = 0; i != m_vCode.size(); ++i)
      {
        m_vCode[i].m_productId = m_id;
        bSuccess = m_vCode[i].SQL_insert_proc(query);
        if (!bSuccess)
          break;
      }
    }
  }

  return bSuccess;
}

bool Product::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                PRODUCT_SQL_COL_NAM ","
                PRODUCT_SQL_COL_CID ","
                PRODUCT_SQL_COL_IID ","
                PRODUCT_SQL_COL_UNI ","
                PRODUCT_SQL_COL_DET ","
                PRODUCT_SQL_COL_ATB ","
                PRODUCT_SQL_COL_ATS
                " FROM " PRODUCT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_name = query.value(0).toString();
      m_category.m_id.set(query.value(1).toLongLong());
      m_image.m_id.set(query.value(2).toLongLong());
      m_unity = query.value(3).toString();
      m_details = query.value(4).toString();
      m_bBuy = query.value(5).toBool();
      m_bSell = query.value(6).toBool();

      QString error2;
      if (bSuccess && m_image.m_id.isValid())
        bSuccess = m_image.SQL_select_proc(query, error2);

      if (bSuccess && m_category.m_id.isValid())
        bSuccess = m_category.SQL_select_proc(query, error);

      if (bSuccess)
      {
        query.prepare("SELECT "
                      PRODUCT_CODE_ITEMS_SQL_COL_COD
                      " FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                      " WHERE " PRODUCT_CODE_ITEMS_SQL_COL_PID " = (:_v01)");
        query.bindValue(":_v01", m_id.get());
        bSuccess = query.exec();
        if (bSuccess)
        {
          while (query.next())
          {
            ProductCode code;
            code.m_code = query.value(0).toString();
            m_vCode.push_back(code);
          }
        }
      }
    }
  }

  return bSuccess;
}

bool Product::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " PRODUCT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool Product::SQL_select_by_code(const ProductCode& code, QString& error)
{
  error.clear();
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare(
        QString("SELECT "
                PRODUCT_CODE_ITEMS_SQL_COL_PID
                " FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                " WHERE %1 = (:_v00)").arg(code.m_id.isValid()
                                           ? SQL_COLID
                                           : PRODUCT_CODE_ITEMS_SQL_COL_COD));
  if (code.m_id.isValid())
    query.bindValue(":_v00",  code.m_id.get());
  else
    query.bindValue(":_v00",  code.m_code);

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
      bSuccess = false;
      error = "Código do produto não encontrado.";
    }
  }
  return SQL_finish(db, query, bSuccess, error);
}

bool Product::SQL_update_unity(const Package& pck, QString& error) const
{
  error.clear();
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("UPDATE " PRODUCT_SQL_TABLE_NAME " SET "
                PRODUCT_SQL_COL_UNI " = (:_v01)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", pck.m_unity);
  bool bSuccess = query.exec();

  if (bSuccess && pck.m_ammount != 0.0)
  {
    query.prepare("UPDATE " PURCHASE_ELEMENTS_SQL_TABLE_NAME " SET "
                  PURCHASE_ELEMENTS_SQL_COL_AMM " = CASE WHEN "
                  PURCHASE_ELEMENTS_SQL_COL_ISP " = FALSE THEN "
                  PURCHASE_ELEMENTS_SQL_COL_AMM "/(:_v01)"
                  " ELSE " PURCHASE_ELEMENTS_SQL_COL_AMM " END,"
                  PURCHASE_ELEMENTS_SQL_COL_PRI " = CASE WHEN "
                  PURCHASE_ELEMENTS_SQL_COL_ISP " = FALSE THEN "
                  PURCHASE_ELEMENTS_SQL_COL_PRI "*(:_v01)"
                  " ELSE " PURCHASE_ELEMENTS_SQL_COL_PRI " END,"
                  PURCHASE_ELEMENTS_SQL_COL_PAM " = CASE WHEN "
                  PURCHASE_ELEMENTS_SQL_COL_ISP " = TRUE THEN "
                  PURCHASE_ELEMENTS_SQL_COL_PAM "/(:_v01)"
                  " ELSE " PURCHASE_ELEMENTS_SQL_COL_PAM " END"
                  " WHERE " PURCHASE_ELEMENTS_SQL_COL_PID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    query.bindValue(":_v01", pck.m_ammount);
    bSuccess = query.exec();
    if (bSuccess)
    {
      query.prepare("UPDATE " SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME " SET "
                    SHOPPING_LIST_ELEMENTS_SQL_AMM " = CASE WHEN "
                    SHOPPING_LIST_ELEMENTS_SQL_ISP " = FALSE THEN "
                    SHOPPING_LIST_ELEMENTS_SQL_AMM "/(:_v01)"
                    " ELSE " SHOPPING_LIST_ELEMENTS_SQL_AMM " END,"
                    SHOPPING_LIST_ELEMENTS_SQL_PRI " = CASE WHEN "
                    SHOPPING_LIST_ELEMENTS_SQL_ISP " = FALSE THEN "
                    SHOPPING_LIST_ELEMENTS_SQL_PRI "*(:_v01)"
                    " ELSE " SHOPPING_LIST_ELEMENTS_SQL_PRI " END,"
                    SHOPPING_LIST_ELEMENTS_SQL_PAM " = CASE WHEN "
                    SHOPPING_LIST_ELEMENTS_SQL_ISP " = TRUE THEN "
                    SHOPPING_LIST_ELEMENTS_SQL_PAM "/(:_v01)"
                    " ELSE " SHOPPING_LIST_ELEMENTS_SQL_PAM " END"
                    " WHERE " PURCHASE_ELEMENTS_SQL_COL_PID " = (:_v00)");
      query.bindValue(":_v00", m_id.get());
      query.bindValue(":_v01", pck.m_ammount);
      bSuccess = query.exec();
    }
  }
  return SQL_finish(db, query, bSuccess, error);
}
