#include "shoppinglist.h"
#include "escpos.h"

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
  return Data::strMoney(subtotal());
}

QString ShoppingListItem::strAmmount() const
{
  return Data::strAmmount(m_ammount);
}

QString ShoppingListItem::strPrice() const
{
  return Data::strMoney(m_price);
}

ShoppingList::ShoppingList(Id id)
{
  ShoppingList::clear();
  m_id = id;
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
                SHOPPING_LIST_SQL_COL_SID ","
                SHOPPING_LIST_SQL_COL_IID ","
                SHOPPING_LIST_SQL_COL_TIT ","
                SHOPPING_LIST_SQL_COL_DES ","
                SHOPPING_LIST_SQL_COL_WEE ","
                SHOPPING_LIST_SQL_COL_MON ","
                SHOPPING_LIST_SQL_COL_NLI
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
      query.prepare("INSERT INTO " SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME " ("
                    SHOPPING_LIST_ELEMENTS_SQL_LID ","
                    SHOPPING_LIST_ELEMENTS_SQL_PID ","
                    SHOPPING_LIST_ELEMENTS_SQL_AMM ","
                    SHOPPING_LIST_ELEMENTS_SQL_PRI ","
                    SHOPPING_LIST_ELEMENTS_SQL_ISP ","
                    SHOPPING_LIST_ELEMENTS_SQL_PUN ","
                    SHOPPING_LIST_ELEMENTS_SQL_PAM ","
                    SHOPPING_LIST_ELEMENTS_SQL_SID
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
                SHOPPING_LIST_SQL_COL_SID " = (:_v01),"
                SHOPPING_LIST_SQL_COL_IID " = (:_v02),"
                SHOPPING_LIST_SQL_COL_TIT " = (:_v03),"
                SHOPPING_LIST_SQL_COL_DES " = (:_v04),"
                SHOPPING_LIST_SQL_COL_WEE " = (:_v05),"
                SHOPPING_LIST_SQL_COL_MON " = (:_v06),"
                SHOPPING_LIST_SQL_COL_NLI " = (:_v07) "
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

  if (bSuccess)
  {
    query.prepare("DELETE FROM " SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME " WHERE " SHOPPING_LIST_ELEMENTS_SQL_LID " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vItem.size(); ++i)
    {
      query.prepare("INSERT INTO " SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME " ("
                    SHOPPING_LIST_ELEMENTS_SQL_LID ","
                    SHOPPING_LIST_ELEMENTS_SQL_PID ","
                    SHOPPING_LIST_ELEMENTS_SQL_AMM ","
                    SHOPPING_LIST_ELEMENTS_SQL_PRI ","
                    SHOPPING_LIST_ELEMENTS_SQL_ISP ","
                    SHOPPING_LIST_ELEMENTS_SQL_PUN ","
                    SHOPPING_LIST_ELEMENTS_SQL_PAM ","
                    SHOPPING_LIST_ELEMENTS_SQL_SID
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

bool ShoppingList::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SHOPPING_LIST_SQL_COL_SID ","
                SHOPPING_LIST_SQL_COL_IID ","
                SHOPPING_LIST_SQL_COL_TIT ","
                SHOPPING_LIST_SQL_COL_DES ","
                SHOPPING_LIST_SQL_COL_WEE ","
                SHOPPING_LIST_SQL_COL_MON ","
                SHOPPING_LIST_SQL_COL_NLI
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
                  SHOPPING_LIST_ELEMENTS_SQL_PID ","
                  SHOPPING_LIST_ELEMENTS_SQL_AMM ","
                  SHOPPING_LIST_ELEMENTS_SQL_PRI ","
                  SHOPPING_LIST_ELEMENTS_SQL_ISP ","
                  SHOPPING_LIST_ELEMENTS_SQL_PUN ","
                  SHOPPING_LIST_ELEMENTS_SQL_PAM ","
                  SHOPPING_LIST_ELEMENTS_SQL_SID
                  " FROM " SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME
                  " WHERE " SHOPPING_LIST_ELEMENTS_SQL_LID " = (:_v01)");
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

QByteArray ShoppingList::printVersion(const QVariant& arg) const
{
  EscPos ep;
  QDateTime dt = DateTime::server();
  ep.expand(true);
  ep.align(true);
  ep.str("LISTA DE COMPRAS\n");
  ep.expand(false);
  ep.doublefont(true);
  ep.str(m_title + "\n");
  ep.doublefont(false);
  ep.expand(true);
  ep.str(dt.date().toString("dd/MM/yyyy") + "\n");
  ep.expand(false);
  ep.str(DateTime::strWeekDay(dt.date()) + "\n\n");
  ep.align();
  if (!m_description.isEmpty())
  {
    ep.expand(true);
    ep.str("Descricao:\n");
    ep.expand(false);
    ep.str(m_description + "\n\n");
  }

  if (m_supplier.m_id.isValid())
  {
    ep.align(true);
    ep.str("Fornecedor: ");
    ep.expand(true);
    ep.str(m_supplier.m_form.strAliasName() + "\n");
    ep.expand(false);
    ep.align(false);
  }

  if (m_vItem.size() != 0 && arg.toBool())
  {
    ep.align(false);
    ep.doublefont(true);
    ep.str("[ EST ][ COM ][ REC ]\n");
    ep.doublefont(false);
  }

  Id lastSupplier, supplier;
  for (int i = 0; i != m_vItem.size(); ++i)
  {
    supplier = m_vItem.at(i).m_supplier.m_id;
    bool bPrintSupplier = ((!lastSupplier.isValid() && supplier.isValid()) ||
                          (lastSupplier.isValid() && supplier.isValid() && lastSupplier != supplier)) &&
                          !m_supplier.m_id.isValid();
    bool bPrintSeparator = ((lastSupplier.isValid() && !supplier.isValid()) ||
                           (lastSupplier.isValid() && supplier.isValid() && lastSupplier != supplier)) &&
                            !m_supplier.m_id.isValid();
    lastSupplier = supplier;

    if (bPrintSeparator)
      ep.str(TABLE_FULL_LINE "\n");

    if (bPrintSupplier && !bPrintSeparator)
      ep.str(TABLE_FULL_LINE "\n");

    if (bPrintSupplier)
    {
      ep.align(true);
      ep.str("Fornecedor: " + m_vItem.at(i).m_supplier.m_form.strAliasName() + "\n");
      ep.align(false);
    }
    ep.bold(true);
    ep.str(m_vItem.at(i).m_product.m_name + "\n");
    ep.bold(false);

    if (m_vItem.at(i).m_price != 0.0)
      ep.str("Preco: " + m_vItem.at(i).strPrice() + "\n");

    if (arg.toBool())
    {
      ep.doublefont(true);
      ep.str("[     ][     ]");
      if (m_vItem.at(i).m_ammount != 0.0)
        ep.str("[" + Data::strFmt(m_vItem.at(i).m_ammount) +
               m_vItem.at(i).m_package.strUnity(m_vItem.at(i).m_product.m_unity) + "]");
      ep.doublefont(false);
      ep.str("\n");
    }
  }
  ep.align(false);

  if (m_nLines > 0)
  {
    for (int i = 0; i != m_nLines; ++i)
      ep.str("\n\n\n" TABLE_FULL_LINE "\n");
  }

  ep.str("\n\n\n");
  ep.cut();

  return ep.m_ba;
}
