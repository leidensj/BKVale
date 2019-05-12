#include "product.h"
#include "jmodel.h"

class ProductModel : public JModel
{
public:
  ProductModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     PRODUCT_SQL_TABLE_NAME "." SQL_COLID ","
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL01 ","
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL04 ","
                     CATEGORY_SQL_TABLE_NAME "." CATEGORY_SQL_COL02
                     " FROM "
                     PRODUCT_SQL_TABLE_NAME
                     " LEFT OUTER JOIN "
                     CATEGORY_SQL_TABLE_NAME
                     " ON "
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL02
                     " = "
                     CATEGORY_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    setHeaderData(2, Qt::Horizontal, tr("Unidade"));
    setHeaderData(3, Qt::Horizontal, tr("Categoria"));
    if (header != nullptr && header->count() == 4)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    }
  }
};

class ProductCodeModel : public JModel
{
public:
  ProductCodeModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "." SQL_COLID ","
                     PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "." PRODUCT_CODE_ITEMS_SQL_COL02 ","
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL01
                     " FROM "
                     PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                     " LEFT OUTER JOIN "
                     PRODUCT_SQL_TABLE_NAME
                     " ON " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "." PRODUCT_CODE_ITEMS_SQL_COL01
                     " = " PRODUCT_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Código"));
    setHeaderData(2, Qt::Horizontal, tr("Produto"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    }
  }
};

Package::Package()
{
  clear();
}

void Package::clear()
{
  m_bIsPackage = false;
  m_unity.clear();
  m_ammount = 0.0;
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
        QString::number(m_ammount, 'f', 3).remove(QRegExp("\\.?0*$")) +
        productUnity + ")";
}

QString Package::strUnity(const QString& productUnity) const
{
  return m_bIsPackage ? m_unity : productUnity;
}

ProductCode::ProductCode()
{
  clear();
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
                PRODUCT_CODE_ITEMS_SQL_COL01 ","
                PRODUCT_CODE_ITEMS_SQL_COL02 ")"
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

JModel* ProductCode::SQL_table_model(QObject* parent) const
{
  return new ProductCodeModel(parent);
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
                PRODUCT_SQL_COL01 ","
                PRODUCT_SQL_COL02 ","
                PRODUCT_SQL_COL03 ","
                PRODUCT_SQL_COL04 ","
                PRODUCT_SQL_COL05 ","
                PRODUCT_SQL_COL06 ","
                PRODUCT_SQL_COL07 ")"
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
                  " WHERE " PRODUCT_CODE_ITEMS_SQL_COL01 " = (:_v01)");
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
                PRODUCT_SQL_COL01 " = (:_v01),"
                PRODUCT_SQL_COL02 " = (:_v02),"
                PRODUCT_SQL_COL03 " = (:_v03),"
                PRODUCT_SQL_COL04 " = (:_v04),"
                PRODUCT_SQL_COL05 " = (:_v05),"
                PRODUCT_SQL_COL06 " = (:_v06),"
                PRODUCT_SQL_COL07 " = (:_v07)"
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
                  " WHERE " PRODUCT_CODE_ITEMS_SQL_COL01 " = (:_v01)");
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
                PRODUCT_SQL_COL01 ","
                PRODUCT_SQL_COL02 ","
                PRODUCT_SQL_COL03 ","
                PRODUCT_SQL_COL04 ","
                PRODUCT_SQL_COL05 ","
                PRODUCT_SQL_COL06 ","
                PRODUCT_SQL_COL07
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
                      PRODUCT_CODE_ITEMS_SQL_COL02
                      " FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                      " WHERE " PRODUCT_CODE_ITEMS_SQL_COL01 " = (:_v01)");
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
                PRODUCT_CODE_ITEMS_SQL_COL01
                " FROM " PRODUCT_CODE_ITEMS_SQL_TABLE_NAME
                " WHERE %1 = (:_v00)").arg(code.m_id.isValid()
                                           ? SQL_COLID
                                           : PRODUCT_CODE_ITEMS_SQL_COL02));
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

JModel* Product::SQL_table_model(QObject* parent) const
{
  return new ProductModel(parent);
}
