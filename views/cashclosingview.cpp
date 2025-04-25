#include "cashclosingview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/cashclosingcointable.h"
#include "tables/cashclosingsectortable.h"
#include "tables/cashclosinginfotable.h"
#include <QLayout>
#include <QFormLayout>

CashClosingView::CashClosingView(QWidget* parent)
  : JItemView(CASH_CLOSING_SQL_TABLE_NAME, parent)
  , m_cashPicker(nullptr)
  , m_coinTable(nullptr)
  , m_sectorTable(nullptr)
  , m_infoTable(nullptr)
{
  m_cashPicker = new DatabasePicker(CASH_SQL_TABLE_NAME);
  m_coinTable = new CashClosingCoinTable;
  m_sectorTable = new CashClosingSectorTable;
  m_infoTable = new CashClosingInfoTable;

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(tr("Caixa:"), m_cashPicker);
  ltMain->addRow(tr("Vendas:"), m_sectorTable);
  ltMain->addRow(tr("Recebimentos:"), m_coinTable);
  ltMain->addRow(tr("Informações:"), m_infoTable);
  ltMain->setAlignment(Qt::AlignTop);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/cashier.png"), tr("Fechamento de Caixa"));

  connect(m_cashPicker, SIGNAL(changedSignal()), this, SLOT(cashChanged()));

  setFocusWidgetOnClear(m_cashPicker);
  m_viewer->refresh();
  clear();
}

void CashClosingView::getItem(JItemSQL& o) const
{
  CashClosing& _o = dynamic_cast<CashClosing&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_cash.m_id= m_cashPicker->getFirstId();
  _o.m_dt = DateTime::server();
}

void CashClosingView::setItem(const JItemSQL& o)
{
  const CashClosing& _o = static_cast<const CashClosing&>(o);
  m_cashPicker->clear();
  m_cashPicker->addItem(_o.m_cash);
}

void CashClosingView::cashChanged()
{
  Cash o;
  o.m_id = m_cashPicker->getFirstId();
  QString error;
  bool bok = o.SQL_select(error);
  if (bok)
  {
    CashClosing _o;
    for (int i = 0; i != o.m_vsectors.size(); ++i)
    {
      CashClosingSector s;
      s.m_sname = o.m_vsectors.at(i).m_sector.m_name;
      s.m_simage = o.m_vsectors.at(i).m_sector.m_image;
      _o.m_vsectors.push_back(s);
    }
    for (int i = 0; i != o.m_vcoins.size(); ++i)
    {
      CashClosingCoin c;
      c.m_cname = o.m_vcoins.at(i).m_coin.m_name;
      c.m_ctax = o.m_vcoins.at(i).m_coin.m_tax;
      c.m_cimage = o.m_vcoins.at(i).m_coin.m_image;
      _o.m_vcoins.push_back(c);
    }
    for (int i = 0; i != o.m_vinfos.size(); ++i)
    {
      CashClosingInfo nfo;
      nfo.m_iname = o.m_vinfos.at(i).m_name;
      nfo.m_itype = (int)o.m_vinfos.at(i).m_type;
      _o.m_vinfos.push_back(nfo);
    }
    m_sectorTable->set(_o.m_vsectors);
    m_coinTable->set(_o.m_vcoins);
    m_infoTable->set(_o.m_vinfos);
  }
  else
  {
    m_sectorTable->removeAllItems();
    m_coinTable->removeAllItems();
    m_infoTable->removeAllItems();
  }
}
