#include "cashclosing.h"
#include <QSqlRecord>
#include "common.h"
#include "escpos.h"

CashClosing::CashClosing(Id id)
{
  CashClosing::clear();
  m_id = id;
}

void CashClosing::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_cash.clear();
  m_dt = DateTime::server();
  m_vcoins.clear();
  m_vsectors.clear();
  m_vinfos.clear();
}

bool CashClosing::operator != (const JItem& other) const
{
  const CashClosing& another = dynamic_cast<const CashClosing&>(other);
  return m_cash != another.m_cash ||
         m_vcoins != another.m_vcoins ||
         m_vsectors != another.m_vsectors ||
         m_vinfos != another.m_vinfos;
}

bool CashClosing::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool CashClosing::isValid() const
{
  return m_cash.m_id.isValid();
}

QString CashClosing::SQL_tableName() const
{
  return CASH_CLOSING_SQL_TABLE_NAME;
}

bool CashClosing::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_CLOSING_SQL_TABLE_NAME " ("
                CASH_CLOSING_SQL_COL_CID ", "
                CASH_CLOSING_SQL_COL_DAT
                ") VALUES ("
                "(:_v01) ,"
                "(:_v02))");
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);

  bool ok = query.exec();
  if (ok)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vcoins.size() && ok; ++i)
    {
      m_vcoins[i].m_ownerId = m_id;
      ok = m_vcoins.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vsectors.size() && ok; ++i)
    {
      m_vsectors[i].m_ownerId = m_id;
      ok = m_vsectors.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vinfos.size() && ok; ++i)
    {
      m_vinfos[i].m_ownerId = m_id;
      ok = m_vinfos.at(i).SQL_insert_proc(query);
    }
  }
  return ok;
}

bool CashClosing::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " CASH_CLOSING_SQL_TABLE_NAME " SET "
                CASH_CLOSING_SQL_COL_CID " = (:_v01),"
                CASH_CLOSING_SQL_COL_DAT " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);

  bool ok = query.exec();
  if (ok)
  {
    ok = CashClosingCoin::SQL_remove_by_owner_id_proc(query, m_id);
    if (ok)
      ok = CashClosingSector::SQL_remove_by_owner_id_proc(query, m_id);
    if (ok)
      ok = CashClosingInfo::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_vcoins.size() && ok; ++i)
    {
      m_vcoins[i].m_ownerId = m_id;
      ok = m_vcoins.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vsectors.size() && ok; ++i)
    {
      m_vsectors[i].m_ownerId = m_id;
      ok = m_vsectors.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vinfos.size() && ok; ++i)
    {
      m_vinfos[i].m_ownerId = m_id;
      ok = m_vinfos.at(i).SQL_insert_proc(query);
    }
  }
  return ok;
}

bool CashClosing::SQL_select_proc(QSqlQuery& query, QString& error)
{
  query.prepare("SELECT "
                CASH_CLOSING_SQL_COL_CID ", "
                CASH_CLOSING_SQL_COL_DAT
                " FROM " CASH_CLOSING_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();

  if (ok)
  {
    if (query.next())
    {
      m_cash.m_id.set(query.value(0).toLongLong());
      m_dt = query.value(1).toDateTime();
    }
    else
    {
      error = "Fechamento de Caixa não encontrado.";
      ok = false;
    }
  }

  if (ok)
    ok = m_cash.SQL_select_proc(query, error);
  if (ok)
    ok = CashClosingCoin::SQL_select_by_owner_id_proc(query, m_id, m_vcoins, error);
  if (ok)
    ok = CashClosingSector::SQL_select_by_owner_id_proc(query, m_id, m_vsectors, error);
      if (ok)
    ok = CashClosingInfo::SQL_select_by_owner_id_proc(query, m_id, m_vinfos, error);

  return ok;
}

bool CashClosing::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " CASH_CLOSING_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

QByteArray CashClosing::printVersion() const
{
  EscPos ep;
  ep.align(true);
  ep.doublefont(true);
  ep.str(QString("Fechamento de Caixa\n%1\n%2\n%3\n").arg(m_cash.m_name, m_dt.toString("dd/MM/yyyy HH:mm:ss")));
  ep.doublefont(false);
  ep.align(false);
  ep.expand(true);
  ep.str("Vendas\n\n");
  ep.expand(false);
  for (auto s : m_vsectors)
  {
    ep.bold(true);
    ep.str(s.m_sname +":\n");
    ep.bold(false);
    ep.str("   Financeiro: " + Data::strMoney(s.m_value) + "\n");
    ep.str("   Fisico: " + Data::strMoney(s.m_nvalue) + "\n\n");
  }
  ep.bold(true);
  ep.str("Total:\n"
         "   Financeiro: " + Data::strMoney(sumSectorsValue()) + "\n"
         "   Fisico:     " + Data::strInt(sumSectorsNValue()) + "\n\n");
  ep.bold(false);

  ep.expand(true);
  ep.str("Recebimentos\n\n");
  ep.expand(false);
  for (auto c : m_vcoins)
  {
    ep.bold(true);
    ep.str(c.m_cname + ":\n");
    ep.bold(false);
    ep.str("   Sem taxas:      " + Data::strMoney(c.m_value) + "\n" +
           "   Com taxas:      " + Data::strMoney(c.valueWithTaxes()) + "\n" +
           "   Total de taxas: " + Data::strMoney(c.taxesDifference()) + "\n");
  }
  ep.bold(true);
  ep.str("Total:\n"
         "   Sem taxas:      " + Data::strMoney(sumCoinsValue()) + "\n" +
         "   Com taxas:      " + Data::strMoney(sumCoinsWithTaxes()) + "\n" +
         "   Total de taxas: " + Data::strMoney(sumCoinsTaxesDifference()) + "\n\n");
  ep.bold(false);

  ep.expand(true);
  ep.str("Quebra de caixa:\n" + Data::strMoney(diff()) + "\n"
         "Diferenca de caixa:\n" + Data::strMoney(diffTax()) + "\n\n");
  ep.expand(false);

  ep.bold(true);
  ep.str("Informacoes:\n");
  ep.bold(false);
  for (auto i : m_vinfos)
    ep.str(i.m_iname + ": " + i.strValue() + "\n");

  ep.str("\n\n\n");
  ep.cut();
  return ep.m_ba;
}

double CashClosing::sumSectorsValue() const
{
  double sum = 0.0;
  for (auto s : m_vsectors)
    sum += s.m_value;
  return sum;
}

int CashClosing::sumSectorsNValue() const
{
  int sum = 0;
  for (auto s : m_vsectors)
    sum += s.m_nvalue;
  return sum;
}

double CashClosing::sumCoinsValue() const
{
  double sum = 0.0;
  for (auto c : m_vcoins)
    sum += c.m_value;
  return sum;
}

double CashClosing::sumCoinsWithTaxes() const
{
  double sum = 0.0;
  for (auto c : m_vcoins)
    sum += c.valueWithTaxes();
  return sum;
}

double CashClosing::sumCoinsTaxesDifference() const
{
  return sumCoinsValue() - sumCoinsWithTaxes();
}

double CashClosing::diff() const
{
  return sumSectorsValue() - sumCoinsValue();
}

double CashClosing::diffTax() const
{
  return sumSectorsValue() - sumCoinsWithTaxes();
}
