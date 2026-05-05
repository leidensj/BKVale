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
  QDateTime dt = DateTime::server();
  m_dt = dt;
  m_day = dt.date();
  if (0 <= dt.time().hour() && dt.time().hour() <= 12)
    m_day = m_day.addDays(-1);
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

bool CashClosing::SQL_insert_update(QString& error) const
{
  // só insere/atualiza se já não tem um mesmo caixa no mesmo dia
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);

  query.prepare("SELECT "
                 CASH_SQL_TABLE_NAME "." CASH_SQL_COL_NAM ", "
                 CASH_CLOSING_SQL_TABLE_NAME "." CASH_CLOSING_SQL_COL_DAY ", "
                 CASH_CLOSING_SQL_TABLE_NAME "." SQL_COLID
                 " FROM " CASH_CLOSING_SQL_TABLE_NAME " LEFT JOIN "
                 CASH_SQL_TABLE_NAME " ON "
                 CASH_CLOSING_SQL_TABLE_NAME ". " CASH_CLOSING_SQL_COL_CID " = "
                 CASH_SQL_TABLE_NAME "." SQL_COLID
                 " WHERE " CASH_SQL_TABLE_NAME "." SQL_COLID " = (:_v00) AND "
                 CASH_CLOSING_SQL_TABLE_NAME "." CASH_CLOSING_SQL_COL_DAY " = (:_v01)");

  query.bindValue(":_v00", m_cash.m_id.get());
  query.bindValue(":_v01", m_day);
  bool ok = false;
  if (query.exec())
  {
    if (query.next())
    {
      if (m_id.isValid() && (query.value(2).toLongLong() == m_id.get()))
        ok = true;
      else
        error = QString("Já existe um caixa %1 para o dia %2.\nSe precisa alterar o caixa, editar o caixa existente.").arg(query.value(0).toString(), query.value(1).toDate().toString("dd/MM/yyyy"));
    }
    else
      ok = true;
  }
  else
    error = query.lastError().text();

  if (ok)
    ok = JItemSQL::SQL_insert_update(error);
  return ok;
}

bool CashClosing::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CASH_CLOSING_SQL_TABLE_NAME " ("
                CASH_CLOSING_SQL_COL_CID ", "
                CASH_CLOSING_SQL_COL_DAT ", "
                CASH_CLOSING_SQL_COL_DAY ", "
                CASH_CLOSING_SQL_COL_DEB ", "
                CASH_CLOSING_SQL_COL_CRE ", "
                CASH_CLOSING_SQL_COL_COM
                ") VALUES ("
                "(:_v01), "
                "(:_v02), "
                "(:_v03), "
                "(:_v04), "
                "(:_v05), "
                "(:_v06))");
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);
  query.bindValue(":_v03", m_day);
  query.bindValue(":_v04", m_debit);
  query.bindValue(":_v05", m_credit);
  query.bindValue(":_v06", m_comission);

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
                CASH_CLOSING_SQL_COL_DAY " = (:_v03),"
                CASH_CLOSING_SQL_COL_DEB " = (:_v04),"
                CASH_CLOSING_SQL_COL_CRE " = (:_v05),"
                CASH_CLOSING_SQL_COL_COM " = (:_v06)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_cash.m_id.get());
  query.bindValue(":_v02", m_dt);
  query.bindValue(":_v03", m_day);
  query.bindValue(":_v04", m_debit);
  query.bindValue(":_v05", m_credit);
  query.bindValue(":_v06", m_comission);

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
                CASH_CLOSING_SQL_COL_DAY ", "
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
      m_day = query.value(2).toDate();
      m_debit = query.value(3).toDouble();
      m_credit = query.value(4).toDouble();
      m_comission = query.value(5).toDouble();
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
  ep.str(QString("Fechamento de Caixa\n%1\n%2\n").arg(m_cash.m_name, QLocale(QLocale::Portuguese, QLocale::Brazil).toString(m_day, "dddd dd/MM/yyyy")));
  ep.doublefont(false);
  ep.str(m_dt.toString("dd/MM/yyyy HH:mm:ss\n"));
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
         "   Valor:         " + Data::strMoney(calculate(CashSummary::Item::Sales)) + "\n");
  ep.bold(false);

  ep.align(true);
  ep.str("----------------------------------\n");
  ep.expand(true);
  ep.str("Bordero\n");
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
          "   Valor bruto:   " + Data::strMoney(calculate(CashSummary::Item::TotalGross)) + "\n");
  if (calculate(CashSummary::Item::Tax) != 0)
  ep.str("   Taxas:         " + Data::strMoney(calculate(CashSummary::Item::Tax)*-1) + "\n"
         "   Valor liquido: " + Data::strMoney(calculate(CashSummary::Item::TotalNet)) + "\n\n");

  ep.bold(false);

  ep.align(true);
  ep.str("----------------------------------\n");
  ep.str("Resumo\n\n");
  ep.expand(true);
  for (int i = 0; i != m_cash.m_vsummary.size(); ++i)
    if (m_cash.m_vsummary.at(i).m_bShow)
      ep.str(QString("%1:\n%2\n\n").arg(m_cash.m_vsummary.at(i).itemToText(), Data::strMoney(calculate(m_cash.m_vsummary.at(i).m_item))));
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

double CashClosing::calculate(CashSummary::Item item) const
{
  switch (item)
  {
    case CashSummary::Item::Sales:
    {
        double sum = 0.0;
        for (const auto& s : m_vsectors)
          sum += s.m_value;
        return sum;
    }
    case CashSummary::Item::Debit:
      return m_debit;
    case CashSummary::Item::Credit:
      return m_credit;
    case CashSummary::Item::Comission:
      return m_comission;
    case CashSummary::Item::TotalGross:
    {
      double sum = 0.0;
      for (const auto& c : m_vcoins)
        sum += c.m_value;
      return sum;
    }
    case CashSummary::Item::TotalNet:
    {
      double sum = 0.0;
      for (const auto& c : m_vcoins)
        sum += c.valueWithTaxes();
      return sum;
    }
    case CashSummary::Item::Tax:
      return calculate(CashSummary::Item::TotalGross) - calculate(CashSummary::Item::TotalNet);
    case CashSummary::Item::RealTotal:
      return calculate(CashSummary::Item::TotalNet) - m_comission;
    case CashSummary::Item::RealSales:
      return calculate(CashSummary::Item::TotalNet) + m_debit - m_credit - m_comission;
    case CashSummary::Item::Difference:
      return calculate(CashSummary::Item::TotalNet) - calculate(CashSummary::Item::Sales);
    case CashSummary::Item::Discrepancy:
      return calculate(CashSummary::Item::TotalGross) - calculate(CashSummary::Item::Sales);
    case CashSummary::Item::Cards:
    {
      double sum = 0.0;
      for (const auto& c : m_vcoins)
        if (c.m_ctax != 0.0)
          sum += c.m_value;
      return sum;
    }
    case CashSummary::Item::NONE:
    default:
      return 0.0;
  }
}
