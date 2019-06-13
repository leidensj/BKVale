#include "shoppinglist.h"
#include "jmodel.h"

class ShoppingListModel : public JModel
{
public:
  ShoppingListModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SHOPPING_LIST_SQL_TABLE_NAME "." SQL_COLID ","
                     SHOPPING_LIST_SQL_TABLE_NAME "." SHOPPING_LIST_SQL_COL03 ","
                     SHOPPING_LIST_SQL_TABLE_NAME "." SHOPPING_LIST_SQL_COL04
                     " FROM "
                     SHOPPING_LIST_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Título"));
    setHeaderData(2, Qt::Horizontal, tr("Descrição"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    }
  }
};

ShoppingListItem::ShoppingListItem()
{
  clear();
}

void ShoppingListItem::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
  m_price = 0.0;
  m_package.clear();
  m_supplier.clear();
}

bool ShoppingListItem::operator !=(const JItem& other) const
{
  const ShoppingListItem& another = dynamic_cast<const ShoppingListItem&>(other);
  return m_product.m_id != another.m_product.m_id ||
         m_ammount != another.m_ammount ||
         m_price != another.m_price ||
         m_package != another.m_package ||
         m_supplier.m_id != another.m_id;
}

bool ShoppingListItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool ShoppingListItem::isValid() const
{
  return m_product.m_id.isValid();
}

double ShoppingListItem::subtotal() const
{
  return m_ammount * m_price;
}

QString ShoppingListItem::strSubtotal() const
{
  return st_strMoney(subtotal());
}

QString ShoppingListItem::strAmmount() const
{
  return st_strAmmount(m_ammount);
}

QString ShoppingListItem::strPrice() const
{
  return st_strMoney(m_price);
}

ShoppingList::ShoppingList()
{
  clear();
}

void ShoppingList::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_title.clear();
  m_description.clear();
  m_supplier.clear();
  m_image.clear();
  for (int i = 0; i != 7; ++i)
    m_weekDays[i] = false;
  for (int i = 0; i != 31; ++i)
    m_monthDays[i] = false;
  m_vItem.clear();
  m_nLines = 0;
}

bool ShoppingList::isValid() const
{
  bool b = true;
  for (int i = 0; i != m_vItem.size(); ++i)
    b &= m_vItem.at(i).isValid();
  return b;
}

void ShoppingList::setWeekDays(const QString& strWeekDays)
{
  for (int i = 0; i != 7; ++i)
    m_weekDays[i] = false;
  QStringList lst = strWeekDays.split(SHOPPING_LIST_SEPARATOR);
  for (int i = 0; i != lst.size(); ++i)
  {
    int day = lst.at(i).toInt();
    if (day > 0 && day < 8)
      m_weekDays[day - 1] = true;
  }
}

void ShoppingList::setMonthDays(const QString& strMonthDays)
{
  for (int i = 0; i != 7; ++i)
    m_monthDays[i] = false;
  QStringList lst = strMonthDays.split(SHOPPING_LIST_SEPARATOR);
  for (int i = 0; i != lst.size(); ++i)
  {
    int day = lst.at(i).toInt();
    if (day > 0 && day < 32)
      m_monthDays[day - 1] = true;
  }
}

QString ShoppingList::getWeekDays() const
{
  QString strWeekDays;
  for (int i = 0; i != 7; ++i)
    if (m_weekDays[i])
      strWeekDays += SHOPPING_LIST_SEPARATOR + QString::number(i + 1);
  if (!strWeekDays.isEmpty())
    strWeekDays += SHOPPING_LIST_SEPARATOR;
  return strWeekDays;
}

QString ShoppingList::getMonthDays() const
{
  QString strMonthDays;
  for (int i = 0; i != 31; ++i)
    if (m_monthDays[i])
      strMonthDays += SHOPPING_LIST_SEPARATOR + QString::number(i + 1);
  if (!strMonthDays.isEmpty())
    strMonthDays += SHOPPING_LIST_SEPARATOR;
  return strMonthDays;
}

bool ShoppingList::operator !=(const JItem& other) const
{
  const ShoppingList& another = dynamic_cast<const ShoppingList&>(other);
  bool b = m_supplier.m_id != another.m_supplier.m_id ||
           m_description != another.m_description ||
           m_nLines != another.m_nLines ||
           m_vItem != another.m_vItem;
  if (!b)
  {
    for (int i = 0; i != 7; ++i)
    {
      b |= m_weekDays[i] != another.m_weekDays[i];
      if (b)
        break;
    }

    if (!b)
      for (int i = 0; i != 31; ++i)
      {
        b |= m_monthDays[i] != another.m_monthDays[i];
        if (b)
          break;
      }
  }

  return b;
}

bool ShoppingList::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString ShoppingList::SQL_tableName() const
{
  return SHOPPING_LIST_SQL_TABLE_NAME;
}

bool ShoppingList::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " SHOPPING_LIST_SQL_TABLE_NAME " ("
                SHOPPING_LIST_SQL_COL01 ","
                SHOPPING_LIST_SQL_COL02 ","
                SHOPPING_LIST_SQL_COL03 ","
                SHOPPING_LIST_SQL_COL04 ","
                SHOPPING_LIST_SQL_COL05 ","
                SHOPPING_LIST_SQL_COL06 ","
                SHOPPING_LIST_SQL_COL07
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");


  query.bindValue(":_v01", m_supplier.m_id.getIdNull());
  query.bindValue(":_v02", m_image.m_id.getIdNull());
  query.bindValue(":_v03", m_title);
  query.bindValue(":_v04", m_description);
  query.bindValue(":_v05", getWeekDays());
  query.bindValue(":_v06", getMonthDays());
  query.bindValue(":_v07", m_nLines);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vItem.size(); ++i)
    {
      query.prepare("INSERT INTO " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " ("
                    SHOPPING_LIST_ITEMS_SQL_COL01 ","
                    SHOPPING_LIST_ITEMS_SQL_COL02 ","
                    SHOPPING_LIST_ITEMS_SQL_COL03 ","
                    SHOPPING_LIST_ITEMS_SQL_COL04 ","
                    SHOPPING_LIST_ITEMS_SQL_COL05 ","
                    SHOPPING_LIST_ITEMS_SQL_COL06 ","
                    SHOPPING_LIST_ITEMS_SQL_COL07 ","
                    SHOPPING_LIST_ITEMS_SQL_COL08
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05),"
                    "(:_v06),"
                    "(:_v07),"
                    "(:_v08))");

      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_vItem.at(i).m_ammount);
      query.bindValue(":_v04", m_vItem.at(i).m_price);
      query.bindValue(":_v05", m_vItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", m_vItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", m_vItem.at(i).m_package.m_ammount);
      query.bindValue(":_v08", m_vItem.at(i).m_supplier.m_id.getIdNull());
      bSuccess = query.exec();
      if (bSuccess)
        m_vItem.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return bSuccess;
}

bool ShoppingList::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " SHOPPING_LIST_SQL_TABLE_NAME " SET "
                SHOPPING_LIST_SQL_COL01 " = (:_v01),"
                SHOPPING_LIST_SQL_COL02 " = (:_v02),"
                SHOPPING_LIST_SQL_COL03 " = (:_v03),"
                SHOPPING_LIST_SQL_COL04 " = (:_v04),"
                SHOPPING_LIST_SQL_COL05 " = (:_v05),"
                SHOPPING_LIST_SQL_COL06 " = (:_v06),"
                SHOPPING_LIST_SQL_COL07 " = (:_v07) "
                "WHERE " SQL_COLID " = (:_v00)");

  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_supplier.m_id.getIdNull());
  query.bindValue(":_v02", m_image.m_id.getIdNull());
  query.bindValue(":_v03", m_title);
  query.bindValue(":_v04", m_description);
  query.bindValue(":_v05", getWeekDays());
  query.bindValue(":_v06", getMonthDays());
  query.bindValue(":_v07", m_nLines);
  bool bSuccess = query.exec();

  query.prepare("DELETE FROM " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " WHERE " SHOPPING_LIST_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", m_id.get());
  bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != m_vItem.size(); ++i)
    {
      query.prepare("INSERT INTO " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME " ("
                    SHOPPING_LIST_ITEMS_SQL_COL01 ","
                    SHOPPING_LIST_ITEMS_SQL_COL02 ","
                    SHOPPING_LIST_ITEMS_SQL_COL03 ","
                    SHOPPING_LIST_ITEMS_SQL_COL04 ","
                    SHOPPING_LIST_ITEMS_SQL_COL05 ","
                    SHOPPING_LIST_ITEMS_SQL_COL06 ","
                    SHOPPING_LIST_ITEMS_SQL_COL07 ","
                    SHOPPING_LIST_ITEMS_SQL_COL08
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05),"
                    "(:_v06),"
                    "(:_v07),"
                    "(:_v08),"
                    "(:_v09),"
                    "(:_v08))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_vItem.at(i).m_ammount);
      query.bindValue(":_v04", m_vItem.at(i).m_price);
      query.bindValue(":_v05", m_vItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", m_vItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", m_vItem.at(i).m_package.m_ammount);
      query.bindValue(":_v08", m_vItem.at(i).m_supplier.m_id.getIdNull());
      bSuccess = query.exec();
      if (bSuccess)
        m_vItem.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  return bSuccess;
}

bool ShoppingList::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SHOPPING_LIST_SQL_COL01 ","
                SHOPPING_LIST_SQL_COL02 ","
                SHOPPING_LIST_SQL_COL03 ","
                SHOPPING_LIST_SQL_COL04 ","
                SHOPPING_LIST_SQL_COL05 ","
                SHOPPING_LIST_SQL_COL06 ","
                SHOPPING_LIST_SQL_COL07
                " FROM " SHOPPING_LIST_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_supplier.m_id.set(query.value(0).toLongLong());
      m_image.m_id.set(query.value(1).toLongLong());
      m_title = query.value(2).toString();
      m_description = query.value(3).toString();
      setWeekDays(query.value(4).toString());
      setMonthDays(query.value(5).toString());
      m_nLines = query.value(6).toInt();
    }
    else
    {
      error = "Lista não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  SHOPPING_LIST_ITEMS_SQL_COL02 ","
                  SHOPPING_LIST_ITEMS_SQL_COL03 ","
                  SHOPPING_LIST_ITEMS_SQL_COL04 ","
                  SHOPPING_LIST_ITEMS_SQL_COL05 ","
                  SHOPPING_LIST_ITEMS_SQL_COL06 ","
                  SHOPPING_LIST_ITEMS_SQL_COL07 ","
                  SHOPPING_LIST_ITEMS_SQL_COL08
                  " FROM " SHOPPING_LIST_ITEMS_SQL_TABLE_NAME
                  " WHERE " SHOPPING_LIST_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (bSuccess && query.next())
      {
        ShoppingListItem item;
        item.m_id.set(query.value(0).toLongLong());
        item.m_product.m_id.set(query.value(1).toLongLong());
        item.m_ammount = query.value(2).toDouble();
        item.m_price = query.value(3).toDouble();
        item.m_package.m_bIsPackage = query.value(4).toBool();
        item.m_package.m_unity = query.value(5).toString();
        item.m_package.m_ammount = query.value(6).toDouble();
        item.m_supplier.m_id.set(query.value(7).toDouble());
        m_vItem.push_back(item);
      }
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vItem.size(); ++i)
    {
      if (m_vItem.at(i).m_supplier.m_id.isValid())
        bSuccess = m_vItem[i].m_supplier.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
      if (m_vItem.at(i).m_product.m_id.isValid())
        bSuccess = m_vItem[i].m_product.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
    }
  }

  if (bSuccess && m_supplier.m_id.isValid())
    bSuccess = m_supplier.SQL_select_proc(query, error);

  if (bSuccess && m_image.m_id.isValid())
    bSuccess = m_image.SQL_select_proc(query, error);

  return bSuccess;
}

bool ShoppingList::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " SHOPPING_LIST_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

JModel* ShoppingList::SQL_table_model(QObject* parent) const
{
  return new ShoppingListModel(parent);
}
