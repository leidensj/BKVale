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
  m_debit = 0.0;
  m_credit = 0.0;
  m_comission = 0.0;
}

bool CashClosing::operator != (const JItem& other) const
{
  const CashClosing& another = dynamic_cast<const CashClosing&>(other);
  return m_cash != another.m_cash ||
         m_vcoins != another.m_vcoins ||
         m_vsectors != another.m_vsectors ||
         m_vinfos != another.m_vinfos ||
         m_debit != another.m_debit ||
         m_credit != another.m_credit ||
         m_comission != another.m_comission;
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
                CASH_CLOSING_SQL_COL_DAT ", "
                CASH_CLOSING_SQL_COL_DEB ", "
                CASH_CLOSING_SQL_COL_CRE ", "
                CASH_CLOSING_SQL_COL_COM
                ") VALUES ("
                "(:_v01) ,"
                "(:_v02) ,"
                "(:_v03) ,"
                "(:_v04) ,"
                "(:_v05))");
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);
  query.bindValue(":_v03", m_debit);
  query.bindValue(":_v04", m_credit);
  query.bindValue(":_v05", m_comission);

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
                CASH_CLOSING_SQL_COL_DAT " = (:_v02),"
                CASH_CLOSING_SQL_COL_DEB " = (:_v03),"
                CASH_CLOSING_SQL_COL_CRE " = (:_v04),"
                CASH_CLOSING_SQL_COL_COM " = (:_v05)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);
  query.bindValue(":_v03", m_debit);
  query.bindValue(":_v04", m_credit);
  query.bindValue(":_v05", m_comission);

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
                CASH_CLOSING_SQL_COL_DAT ", "
                CASH_CLOSING_SQL_COL_DEB ", "
                CASH_CLOSING_SQL_COL_CRE ", "
                CASH_CLOSING_SQL_COL_COM
                " FROM " CASH_CLOSING_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool ok = query.exec();

  if (ok)
  {
    if (query.next())
    {
      m_cash.m_id.set(query.value(0).toLongLong());
      m_dt = query.value(1).toDateTime().toLocalTime();
      m_debit = query.value(2).toDouble();
      m_credit = query.value(3).toDouble();
      m_comission = query.value(4).toDouble();
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

QByteArray CashClosing::printVersion(const QVariant& /*arg*/) const
{
  EscPos ep;
  ep.align(true);
  ep.doublefont(true);
  ep.str(QString("Fechamento de Caixa\n%1\n%2\n").arg(m_cash.m_name, m_dt.toString("dd/MM/yyyy HH:mm:ss")));
  ep.doublefont(false);
  ep.align(false);

  ep.align(true);
  ep.str("----------------------------------\n");
  ep.expand(true);
  ep.str("Entradas\n");
  ep.expand(false);
  ep.align(false);
  for (const auto& s : m_vsectors)
  {
    if (s.m_value != 0.0)
    {
      ep.bold(true);
      ep.str(s.m_sname +":\n");
      ep.bold(false);
      ep.str("   Valor:         " + Data::strMoney(s.m_value) + "\n");
    }
  }
  ep.bold(true);
  ep.str("TOTAL:\n"
         "   Valor:         " + Data::strMoney(sumSectorsValue()) + "\n");
  ep.bold(false);

  ep.align(true);
  ep.str("----------------------------------\n");
  ep.expand(true);
  ep.str("Recebimentos\n");
  ep.expand(false);
  ep.align(false);
  for (const auto& c : m_vcoins)
  {
    if (c.m_value == 0.0)
      continue;
    ep.bold(true);
    ep.str(c.m_cname + (c.m_ctax == 0.0 ? ":\n" : (": (" + Data::strPercentage(c.m_ctax) + ")\n")));
    ep.bold(false);
    if (c.taxesDifference() != 0)
    ep.str("   Valor bruto   :");
    else
    ep.str("   Valor:         ");
    ep.str(Data::strMoney(c.m_value) + "\n");
    if (c.taxesDifference() != 0)
    ep.str("   Taxas:         " + Data::strMoney(c.taxesDifference()*-1) + "\n"
           "   Valor liquido: " + Data::strMoney(c.valueWithTaxes()) + "\n");
  }
  ep.bold(true);
  ep.str("TOTAL:\n"
         "   Valor bruto:   " + Data::strMoney(sumCoinsValue()) + "\n");
  if (sumCoinsTaxesDifference() != 0)
  ep.str("   Taxas:         " + Data::strMoney(sumCoinsTaxesDifference()*-1) + "\n"
         "   Valor liquido: " + Data::strMoney(sumCoinsWithTaxes()) + "\n\n");

  ep.bold(false);

  ep.align(true);
  ep.str("----------------------------------\n");
  ep.str("Resumo\n\n");
  ep.expand(true);
  ep.str("Entradas:\n" + Data::strMoney(sumSectorsValue()) + "\n\n");
  ep.str("Assinadas:\n" + Data::strMoney(m_debit) + "\n\n");
  ep.str("Créditos:\n" + Data::strMoney(m_credit) + "\n\n");
  ep.str("Comissões:\n" + Data::strMoney(m_comission) + "\n\n");
  ep.str("Total:\n" + Data::strMoney(sumCoinsWithTaxes()) + "\n\n");
  ep.str("Venda Real:\n" + Data::strMoney(sumCoinsWithTaxes() + m_debit - m_credit - m_comission) + "\n\n");
  ep.str("Diferenca de caixa:\n" + Data::strMoney(diffTax()) + "\n\n");
  ep.str("Quebra de caixa:\n" + Data::strMoney(diff()) + "\n\n");
  ep.expand(false);
  ep.str("----------------------------------\n");
  ep.align(false);

  ep.bold(true);
  ep.str("Informacoes:\n");
  ep.bold(false);
  for (const auto& i : m_vinfos)
    ep.str(i.m_iname + ": " + i.strValue() + "\n");

  ep.cut(true);
  return ep.m_ba;
}

double CashClosing::sumSectorsValue() const
{
  double sum = 0.0;
  for (const auto& s : m_vsectors)
    sum += s.m_value;
  return sum;
}

int CashClosing::sumSectorsNValue() const
{
  int sum = 0;
  for (const auto& s : m_vsectors)
    sum += s.m_nvalue;
  return sum;
}

double CashClosing::sumCoinsValue() const
{
  double sum = 0.0;
  for (const auto& c : m_vcoins)
    sum += c.m_value;
  return sum;
}

double CashClosing::sumCoinsWithTaxes() const
{
  double sum = 0.0;
  for (const auto& c : m_vcoins)
    sum += c.valueWithTaxes();
  return sum;
}

double CashClosing::sumCoinsTaxesDifference() const
{
  return sumCoinsValue() - sumCoinsWithTaxes();
}

double CashClosing::diff() const
{
  return sumCoinsValue() - sumSectorsValue();
}

double CashClosing::diffTax() const
{
  return sumCoinsWithTaxes() - sumSectorsValue();
}
