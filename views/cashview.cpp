#include "cashview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/cashinfotable.h"
#include "tables/cashsummarytable.h"
#include <QLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QLayout>

CashView::CashView(QWidget* parent)
  : JItemView(CASH_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_sectorPicker(nullptr)
  , m_coinPicker(nullptr)
  , m_infoTable(nullptr)
  , m_summaryTable(nullptr)
  , m_btnAddRemove(nullptr)
  , m_cbDebit(nullptr)
  , m_cbCredit(nullptr)
  , m_cbComission(nullptr)
{
  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edName->setPlaceholderText(tr("*"));
  m_sectorPicker = new DatabasePicker(SECTOR_SQL_TABLE_NAME, true);
  m_coinPicker = new DatabasePicker(COIN_SQL_TABLE_NAME, true);
  m_btnAddRemove = new JAddRemoveButtons;
  m_infoTable = new CashInfoTable(m_btnAddRemove);
  m_summaryTable = new CashSummaryTable;
  m_cbDebit = new QCheckBox(tr("Assinadas"));
  m_cbCredit = new QCheckBox(tr("Créditos"));
  m_cbComission = new QCheckBox(tr("Comissões"));

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(tr("Nome:"), m_edName);
  ltMain->addRow(JItemHelper::text(SECTOR_SQL_TABLE_NAME) + ":", m_sectorPicker);
  ltMain->addRow(JItemHelper::text(COIN_SQL_TABLE_NAME) + ":", m_coinPicker);
  ltMain->addRow(tr("Informações:"), m_btnAddRemove);
  ltMain->addRow("", m_infoTable);
  ltMain->addRow(tr("Resumo:"), m_summaryTable);
  ltMain->setAlignment(Qt::AlignTop);

  auto ltcb = new QHBoxLayout;
  ltcb->setAlignment(Qt::AlignLeft);
  ltcb->addWidget(m_cbDebit);
  ltcb->addWidget(m_cbCredit);
  ltcb->addWidget(m_cbComission);
  auto lt2 = new QFormLayout;
  lt2->addRow(tr("Obrigatório:"), ltcb);
  lt2->addRow(tr("Resumo:"), m_summaryTable);
  lt2->setAlignment(Qt::AlignTop);

  auto tab1 = new QFrame;
  tab1->setLayout(ltMain);
  m_tab->addTab(tab1, QIcon(":/icons/res/cashier.png"), tr("Caixa"));
  auto tab2 = new QFrame;
  tab2->setLayout(lt2);
  m_tab->addTab(tab2, QIcon(":/icons/res/details.png"), tr("Informações"));

  setFocusWidgetOnClear(m_edName);
  m_viewer->refresh();
  clear();
}

void CashView::getItem(JItemSQL& o) const
{
  Cash& _o = dynamic_cast<Cash&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_name = m_edName->text();
  _o.m_bDebit = m_cbDebit->isChecked();
  _o.m_bCredit = m_cbCredit->isChecked();
  _o.m_bComission = m_cbComission->isChecked();
  Ids ids = m_coinPicker->getIds();
  for (int i = 0; i != ids.size(); ++i)
  {
    Coin c;
    c.m_id = ids.at(i);
    CashCoin _c;
    _c.m_coin = c;
    _c.m_ownerId =_o.m_id;
    _o.m_vcoins.push_back(_c);
  }
  ids = m_sectorPicker->getIds();
  for (int i = 0; i != ids.size(); ++i)
  {
    Sector s;
    s.m_id = ids.at(i);
    CashSector _s;
    _s.m_sector = s;
    _s.m_ownerId =_o.m_id;
    _o.m_vsectors.push_back(_s);
  }
  m_infoTable->get(_o.m_vinfos);
  m_summaryTable->get(_o.m_vsummary);
}

void CashView::setItem(const JItemSQL& o)
{
  const Cash& _o = static_cast<const Cash&>(o);
  m_edName->setText(_o.m_name);
  m_cbDebit->setChecked(_o.m_bDebit);
  m_cbCredit->setChecked(_o.m_bCredit);
  m_cbComission->setChecked(_o.m_bComission);
  m_coinPicker->clear();
  for (int i = 0; i != _o.m_vcoins.size(); ++i)
    m_coinPicker->addItem(_o.m_vcoins.at(i).m_coin);
  m_sectorPicker->clear();
  for (int i = 0; i != _o.m_vsectors.size(); ++i)
    m_sectorPicker->addItem(_o.m_vsectors.at(i).m_sector);
  m_infoTable->set(_o.m_vinfos);
  m_summaryTable->set(_o.m_vsummary);
}
