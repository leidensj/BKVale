#include "cashview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/cashinfotable.h"
#include <QLayout>
#include <QFormLayout>

CashView::CashView(QWidget* parent)
  : JItemView(CASH_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_sectorPicker(nullptr)
  , m_coinPicker(nullptr)
  , m_infoTable(nullptr)
  , m_btnAddRemove(nullptr)
{
  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edName->setPlaceholderText(tr("*"));
  m_sectorPicker = new DatabasePicker(SECTOR_SQL_TABLE_NAME, true);
  m_coinPicker = new DatabasePicker(COIN_SQL_TABLE_NAME, true);
  m_btnAddRemove = new JAddRemoveButtons;
  m_infoTable = new CashInfoTable(m_btnAddRemove);

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(tr("Nome:"), m_edName);
  ltMain->addRow(JItemHelper::text(SECTOR_SQL_TABLE_NAME) + ":", m_sectorPicker);
  ltMain->addRow(JItemHelper::text(COIN_SQL_TABLE_NAME) + ":", m_coinPicker);
  ltMain->addRow(tr("Informações:"), m_btnAddRemove);
  ltMain->addRow("", m_infoTable);
  ltMain->setAlignment(Qt::AlignTop);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/cashier.png"), tr("Caixa"));

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
}

void CashView::setItem(const JItemSQL& o)
{
  const Cash& _o = static_cast<const Cash&>(o);
  m_edName->setText(_o.m_name);
  m_coinPicker->clear();
  for (int i = 0; i != _o.m_vcoins.size(); ++i)
    m_coinPicker->addItem(_o.m_vcoins.at(i).m_coin);
  m_sectorPicker->clear();
  for (int i = 0; i != _o.m_vsectors.size(); ++i)
    m_sectorPicker->addItem(_o.m_vsectors.at(i).m_sector);
  m_infoTable->set(_o.m_vinfos);
}
