#include "discount.h"
#include "jmodel.h"

class DiscountModel : public JModel
{
public:
  DiscountModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     DISCOUNT_SQL_COL01 ","
                     DISCOUNT_SQL_COL04 ","
                     DISCOUNT_SQL_COL08 ","
                     DISCOUNT_SQL_COL07
                     " FROM "
                     DISCOUNT_SQL_TABLE_NAME);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Código"));
    setHeaderData(2, Qt::Horizontal, tr("Tipo"));
    setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    setHeaderData(4, Qt::Horizontal, tr("Regatado"));
    if (header != nullptr && header->count() == 5)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
    }
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
    if (!index.isValid())
      return QModelIndex();

    QVariant value = QSqlQueryModel::data(index, role);
    if (role == Qt::DisplayRole)
    {
      if (index.column() == 2)
        value = Discount::strType((Discount::Type)value.toInt());
      else if (index.column() == 4)
        value = value.toBool() ? tr("Sim") : "";
    }
    return value;
  }
};

DiscountItem::DiscountItem()
{
  clear();
}

QString DiscountItem::strAmmount() const
{
  return JItem::st_strAmmount(m_ammount);
}

void DiscountItem::clear()
{
  m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
}

bool DiscountItem::operator !=(const JItem& other) const
{
  const DiscountItem& another = dynamic_cast<const DiscountItem&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount;
}

bool DiscountItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool DiscountItem::isValid() const
{
  return m_ammount >= 0;
}

Discount::Discount()
{
  clear();
}

QString Discount::strType(Type type)
{
  switch (type)
  {
    case Type::Value: return "Valor";
    case Type::Percentage: return "Porcentagem";
    case Type::Product: return "Produto";
    case Type::None:
    default:
      return "Nenhum";
  }
}

QString Discount::strExpDate() const
{
  return m_dtExp.toString("dd/MM/yyyy");
}

QString Discount::getRandomCode(const int length)
{
  const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

  QString randomString;
  for(int i = 0; i < length; ++i)
  {
    int idx = qrand() % possibleCharacters.length();
    QChar nextChar = possibleCharacters.at(idx);
    randomString.append(nextChar);
  }
  return randomString;
}

void Discount::clear()
{
  m_id.clear();
  m_code.clear();
  m_type = Type::Value;
  m_value = 0.0;
  m_percentage = 0.0;
  m_items.clear();
  m_bRedeemed = false;
  m_bExpires = false;
  m_dtExp = QDate::currentDate();
  m_description.clear();
}

bool Discount::operator !=(const JItem& other) const
{
  const Discount& another = dynamic_cast<const Discount&>(other);
  return
      m_type != another.m_type ||
      m_items != another.m_items ||
      m_bRedeemed != another.m_bRedeemed ||
      m_bExpires != another.m_bExpires ||
      m_dtExp != another.m_dtExp ||
      m_value != another.m_value ||
      m_percentage != another.m_percentage ||
      m_description != another.m_description;
}

bool Discount::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Discount::isValid() const
{
  switch (m_type)
  {
    case Type::Percentage:
      return m_percentage >= 0.0 && m_percentage <= 100.0;
    case Type::Value:
      return m_value >= 0.0;
    case Type::Product:
      for (int i = 0; i != m_items.size(); ++i)
        if (!m_items.at(i).isValid())
          return false;
      return true;
    case Type::None:
    default:
      return false;
  }
}

QString Discount::SQL_tableName() const
{
  return DISCOUNT_SQL_TABLE_NAME;
}

bool Discount::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = false;

  do
  {
    m_code = Discount::getRandomCode();
    query.prepare("SELECT " SQL_COLID " FROM "
                  DISCOUNT_SQL_TABLE_NAME " WHERE "
                  DISCOUNT_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_code);
    bSuccess = query.exec();
  } while (bSuccess && query.next());

  if (bSuccess)
  {
    query.prepare("INSERT INTO " DISCOUNT_SQL_TABLE_NAME " ("
                  DISCOUNT_SQL_COL01 ","
                  DISCOUNT_SQL_COL02 ","
                  DISCOUNT_SQL_COL03 ","
                  DISCOUNT_SQL_COL04 ","
                  DISCOUNT_SQL_COL05 ","
                  DISCOUNT_SQL_COL06 ","
                  DISCOUNT_SQL_COL07 ","
                  DISCOUNT_SQL_COL08
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04),"
                  "(:_v05),"
                  "(:_v06),"
                  "(:_v07),"
                  "(:_v08))");
      query.bindValue(":_v01", m_code);
      query.bindValue(":_v02", m_bExpires);
      query.bindValue(":_v03", m_dtExp);
      query.bindValue(":_v04", (int)m_type);
      query.bindValue(":_v05", m_value);
      query.bindValue(":_v06", m_percentage);
      query.bindValue(":_v07", m_bRedeemed);
      query.bindValue(":_v08", m_description);
      bSuccess = query.exec();
  }

  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_items.size(); ++i)
    {
      query.prepare("INSERT INTO " DISCOUNT_ITEMS_SQL_TABLE_NAME " ("
                    DISCOUNT_ITEMS_SQL_COL01 ","
                    DISCOUNT_ITEMS_SQL_COL02 ","
                    DISCOUNT_ITEMS_SQL_COL03
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_items.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_items.at(i).m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        m_items.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return bSuccess;
}

bool Discount::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " DISCOUNT_SQL_TABLE_NAME " SET "
                DISCOUNT_SQL_COL01 " = (:_v01),"
                DISCOUNT_SQL_COL02 " = (:_v02),"
                DISCOUNT_SQL_COL03 " = (:_v03),"
                DISCOUNT_SQL_COL04 " = (:_v04),"
                DISCOUNT_SQL_COL05 " = (:_v05),"
                DISCOUNT_SQL_COL06 " = (:_v06),"
                DISCOUNT_SQL_COL07 " = (:_v07),"
                DISCOUNT_SQL_COL08 " = (:_v08) "
                "WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_code);
  query.bindValue(":_v02", m_bExpires);
  query.bindValue(":_v03", m_dtExp);
  query.bindValue(":_v04", (int)m_type);
  query.bindValue(":_v05", m_value);
  query.bindValue(":_v06", m_percentage);
  query.bindValue(":_v07", m_bRedeemed);
  query.bindValue(":_v08", m_description);
  bool bSuccess = query.exec();

  query.prepare("DELETE FROM " DISCOUNT_ITEMS_SQL_TABLE_NAME " WHERE " DISCOUNT_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", m_id.get());
  bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != m_items.size(); ++i)
    {
      query.prepare("INSERT INTO " DISCOUNT_ITEMS_SQL_TABLE_NAME " ("
                    DISCOUNT_ITEMS_SQL_COL01 ","
                    DISCOUNT_ITEMS_SQL_COL02 ","
                    DISCOUNT_ITEMS_SQL_COL03
                    " ) VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_items.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_items.at(i).m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        m_items.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return bSuccess;
}

bool Discount::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                DISCOUNT_SQL_COL01 ","
                DISCOUNT_SQL_COL02 ","
                DISCOUNT_SQL_COL03 ","
                DISCOUNT_SQL_COL04 ","
                DISCOUNT_SQL_COL05 ","
                DISCOUNT_SQL_COL06 ","
                DISCOUNT_SQL_COL07 ","
                DISCOUNT_SQL_COL08
                " FROM " DISCOUNT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_code = query.value(0).toString();
      m_bExpires = query.value(1).toBool();
      m_dtExp = query.value(2).toDate();
      m_type = (Discount::Type)query.value(3).toInt();
      m_value = query.value(4).toDouble();
      m_percentage = query.value(5).toDouble();
      m_bRedeemed = query.value(6).toBool();
      m_description = query.value(7).toString();
    }
    else
    {
      error = "Desconto não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  DISCOUNT_ITEMS_SQL_COL02 ","
                  DISCOUNT_ITEMS_SQL_COL03
                  " FROM " DISCOUNT_ITEMS_SQL_TABLE_NAME
                  " WHERE " DISCOUNT_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (bSuccess && query.next())
      {
        DiscountItem oi;
        oi.m_id.set(query.value(0).toLongLong());
        oi.m_product.m_id.set(query.value(1).toLongLong());
        oi.m_ammount = query.value(2).toDouble();
        m_items.push_back(oi);
      }
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_items.size(); ++i)
    {
      if (m_items.at(i).m_product.m_id.isValid())
        bSuccess = m_items[i].m_product.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
    }
  }

  return bSuccess;
}

bool Discount::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " DISCOUNT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

bool Discount::SQL_redeem(const QString& code, bool& redeemed, QString& error)
{
  redeemed = false;
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                SQL_COLID
                " FROM " DISCOUNT_SQL_TABLE_NAME
                " WHERE " DISCOUNT_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", code);
  bool bSuccess = query.exec();

  if (bSuccess && query.next())
  {
    m_id.set(query.value(0).toLongLong());
    bSuccess = SQL_select_proc(query, error);

    if (bSuccess)
    {
      redeemed = m_bRedeemed;
      query.prepare("UPDATE " DISCOUNT_SQL_TABLE_NAME " SET "
                    DISCOUNT_SQL_COL07 " = TRUE "
                    "WHERE " SQL_COLID " = (:_v00)");
      query.bindValue(":_v00", m_id.get());
      bSuccess = query.exec();
      if (bSuccess)
        m_bRedeemed = true;
    }
  }
  else
  {
    bSuccess = false;
    error = "Código informado não encontrado.";
  }

  return SQL_finish(db, query, bSuccess, error);
}

JModel* Discount::SQL_table_model(QObject* parent) const
{
  return new DiscountModel(parent);
}
