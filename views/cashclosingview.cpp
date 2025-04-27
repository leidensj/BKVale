#include "cashclosingview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/cashclosingcointable.h"
#include "tables/cashclosingsectortable.h"
#include "tables/cashclosinginfotable.h"
#include <QLabel>
#include <QLayout>
#include <QFormLayout>

CashClosingView::CashClosingView(QWidget* parent)
  : JItemView(CASH_CLOSING_SQL_TABLE_NAME, parent)
  , m_cashPicker(nullptr)
  , m_coinTable(nullptr)
  , m_sectorTable(nullptr)
  , m_infoTable(nullptr)
  , m_edSector1(nullptr)
  , m_edSector2(nullptr)
  , m_edCoin1(nullptr)
  , m_edCoin2(nullptr)
  , m_edDiff1(nullptr)
  , m_edDiff2(nullptr)
{
  m_cashPicker = new DatabasePicker(CASH_SQL_TABLE_NAME);
  m_cashPicker->setPlaceholderText(true);
  m_coinTable = new CashClosingCoinTable;
  m_sectorTable = new CashClosingSectorTable;
  m_infoTable = new CashClosingInfoTable;
  m_edSector1 = new JExpLineEdit(Data::Type::Money);
  m_edSector2 = new JExpLineEdit(Data::Type::Integer);
  m_edCoin1 = new JExpLineEdit(Data::Type::Money);
  m_edCoin2 = new JExpLineEdit(Data::Type::Money);
  m_edDiff1 = new JExpLineEdit(Data::Type::Money);
  m_edDiff2 = new JExpLineEdit(Data::Type::Money);
  m_edSector1->setReadOnly(true);
  m_edSector2->setReadOnly(true);
  m_edCoin1->setReadOnly(true);
  m_edCoin2->setReadOnly(true);
  m_edDiff1->setReadOnly(true);
  m_edDiff2->setReadOnly(true);
  m_edSector1->setInvertColors(true);
  m_edSector2->setInvertColors(true);
  m_edCoin1->setInvertColors(true);
  m_edCoin2->setInvertColors(true);
  m_edDiff1->setInvertColors(true);
  m_edDiff2->setInvertColors(true);
  m_edDiff1->setToolTip(tr("Valor próximo de 0 indica que o caixa fechou."));
  m_edDiff2->setToolTip(tr("Diferença entre vendas e recebimentos, contando a quebra de caixa e a diferença das taxas."));

  QLabel* cash = new QLabel(tr("Caixa"));
  QLabel* sector = new QLabel(tr("Vendas"));
  QLabel* coin = new QLabel(tr("Recebimentos"));
  QLabel* info = new QLabel(tr("Informações"));
  QLabel* summary = new QLabel(tr("Resumo"));
  QFont font = cash->font();
  font.setPointSize(14);
  cash->setFont(font);
  sector->setFont(font);
  coin->setFont(font);
  info->setFont(font);
  summary->setFont(font);
  cash->setAlignment(Qt::AlignCenter);
  sector->setAlignment(Qt::AlignCenter);
  coin->setAlignment(Qt::AlignCenter);
  info->setAlignment(Qt::AlignCenter);
  summary->setAlignment(Qt::AlignCenter);

  auto lsector = new QVBoxLayout;
  lsector->addWidget(sector);
  lsector->addWidget(m_sectorTable);
  auto lcoin = new QVBoxLayout;
  lcoin->addWidget(coin);
  lcoin->addWidget(m_coinTable);
  auto ltables1 = new QHBoxLayout;
  ltables1->addLayout(lsector);
  ltables1->addLayout(lcoin);

  auto linfo = new QVBoxLayout;
  linfo->addWidget(info);
  linfo->addWidget(m_infoTable);

  auto results1 = new QFormLayout;
  results1->setAlignment(Qt::AlignTop);
  results1->addRow(tr("Vendas (Financeiro):"),m_edSector1);
  results1->addRow(tr("Recebimentos sem taxas:"),m_edCoin1);
  results1->addRow(tr("Quebra de Caixa:"),m_edDiff1);

  auto results2 = new QFormLayout;
  results2->addRow(tr("Vendas (Físico):"),m_edSector2);
  results2->addRow(tr("Recebimentos com taxas:"),m_edCoin2);
  results2->addRow(tr("Diferença de caixa:"),m_edDiff2);

  auto resulth = new QHBoxLayout;
  resulth->addLayout(results1);
  resulth->addLayout(results2);
  auto resultv = new QVBoxLayout;
  resultv->addWidget(summary);
  resultv->addLayout(resulth);
  resultv->setAlignment(Qt::AlignTop);

  auto ltables2 = new QHBoxLayout;
  ltables2->addLayout(linfo);
  ltables2->addLayout(resultv);

  auto lmain = new QVBoxLayout;
  lmain->addWidget(cash);
  lmain->addWidget(m_cashPicker);
  lmain->addLayout(ltables1);
  lmain->addLayout(ltables2);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(lmain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/cashier.png"), tr("Fechamento de Caixa"));

  connect(m_cashPicker, SIGNAL(changedSignal()), this, SLOT(cashChanged()));
  connect(m_sectorTable, SIGNAL(changedSignal(int, int)), this, SLOT(update()));
  connect(m_coinTable, SIGNAL(changedSignal(int, int)), this, SLOT(update()));

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
  m_coinTable->get(_o.m_vcoins);
  m_sectorTable->get(_o.m_vsectors);
  m_infoTable->get(_o.m_vinfos);
}

void CashClosingView::setItem(const JItemSQL& o)
{
  const CashClosing& _o = static_cast<const CashClosing&>(o);
  m_sectorTable->set(_o.m_vsectors);
  m_coinTable->set(_o.m_vcoins);
  m_infoTable->set(_o.m_vinfos);

  m_cashPicker->blockSignals(true);
  m_cashPicker->setEnabled(!_o.m_id.isValid());
  m_cashPicker->clear();
  m_cashPicker->addItem(_o.m_cash);
  m_cashPicker->blockSignals(false);
  update();
}

void CashClosingView::cashChanged()
{
  CashClosing o;
  o.m_cash.m_id = m_cashPicker->getFirstId();
  QString error;
  bool ok = o.m_cash.SQL_select(error);
  if (ok)
  {
    for (int i = 0; i != o.m_cash.m_vsectors.size(); ++i)
    {
      CashClosingSector s;
      s.m_sname = o.m_cash.m_vsectors.at(i).m_sector.m_name;
      s.m_simage = o.m_cash.m_vsectors.at(i).m_sector.m_image;
      o.m_vsectors.push_back(s);
    }
    for (int i = 0; i != o.m_cash.m_vcoins.size(); ++i)
    {
      CashClosingCoin c;
      c.m_cname = o.m_cash.m_vcoins.at(i).m_coin.m_name;
      c.m_ctax = o.m_cash.m_vcoins.at(i).m_coin.m_tax;
      c.m_cimage = o.m_cash.m_vcoins.at(i).m_coin.m_image;
      o.m_vcoins.push_back(c);
    }
    for (int i = 0; i != o.m_cash.m_vinfos.size(); ++i)
    {
      CashClosingInfo nfo;
      nfo.m_iname = o.m_cash.m_vinfos.at(i).m_name;
      nfo.m_itype = (int)o.m_cash.m_vinfos.at(i).m_type;
      o.m_vinfos.push_back(nfo);
    }
  }

  setItem(o);
}

void CashClosingView::update()
{
  m_edSector1->setValue(m_sectorTable->sum((int)CashClosingSectorTable::Column::Value));
  m_edSector2->setValue(m_sectorTable->sum((int)CashClosingSectorTable::Column::NValue));
  m_edCoin1->setValue(m_coinTable->sum((int)CashClosingCoinTable::Column::Value));
  m_edCoin2->setValue(m_coinTable->sumWithTaxes());
  m_edDiff1->setValue(m_edCoin1->value() - m_edSector1->value());
  m_edDiff2->setValue(m_edCoin2->value() - m_edSector1->value());
}
