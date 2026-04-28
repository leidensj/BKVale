#include "cashsummary.h"

CashSummary::CashSummary()
{
  CashSummary::clear();
}

void CashSummary::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_item = Item::NONE;
  m_bShow = false;
}

bool CashSummary::isValid() const
{
  return m_item != Item::NONE;
}

bool CashSummary::operator !=(const JItem& other) const
{
  const CashSummary& another = dynamic_cast<const CashSummary&>(other);
  return
      m_item != another.m_item &&
      m_bShow != another.m_bShow;
}

bool CashSummary::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool CashSummary::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_SUMMARY_SQL_TABLE_NAME " ("
                CASH_SUMMARY_SQL_COL_OID ","
                CASH_SUMMARY_SQL_COL_ITE ","
                CASH_SUMMARY_SQL_COL_SHO
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", (int)m_item);
  query.bindValue(":_v03", m_bShow);

  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CashSummary::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashSummary>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID
                " FROM " CASH_SUMMARY_SQL_TABLE_NAME
                " WHERE " CASH_SUMMARY_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool ok = query.exec();
  if (ok)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      CashSummary o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  return ok;
}

bool CashSummary::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                CASH_SUMMARY_SQL_COL_OID ","
                CASH_SUMMARY_SQL_COL_ITE ","
                CASH_SUMMARY_SQL_COL_SHO
                " FROM " CASH_SUMMARY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_item = (CashSummary::Item)query.value(1).toInt();
      m_bShow = query.value(2).toBool();
    }
    else
    {
      error = "Resumo do Caixa não encontrado.";
      ok = false;
    }
  }

  return ok;
}

bool CashSummary::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " CASH_SUMMARY_SQL_TABLE_NAME
                " WHERE " CASH_SUMMARY_SQL_COL_OID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}

QString CashSummary::st_ItemToText(Item item)
{
  switch (item)
  {
    case Item::Sales: return "Entradas";
    case Item::Debit: return "Assinadas";
    case Item::Credit: return "Créditos";
    case Item::Comission: return "Comissões";
    case Item::TotalGross: return "Bordero";
    case Item::TotalNet: return "Total";
    case Item::Tax: return "Taxas";
    case Item::RealTotal: return "Total Real";
    case Item::RealSales: return "Venda Real";
    case Item::Difference: return "Diferença";
    case Item::Discrepancy: return "Quebra";
    case Item::Cards: return "Cartão";
    case Item::NONE: default: return "VAZIO";
  }
}

QString CashSummary::itemToText() const
{
  return st_ItemToText(m_item);
}
