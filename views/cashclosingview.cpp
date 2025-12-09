#include "cashclosingview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/cashclosingcointable.h"
#include "tables/cashclosingsectortable.h"
#include "tables/cashclosinginfotable.h"
#include "controls/calculatordialog.h"
#include "controls/postitdialog.h"
#include "filters/cashclosingfilter.h"
#include "escposprinter.h"
#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

CashClosingView::CashClosingView(QWidget* parent)
  : JItemView(CASH_CLOSING_SQL_TABLE_NAME, parent)
  , m_cashPicker(nullptr)
  , m_dt(nullptr)
  , m_coinTable(nullptr)
  , m_sectorTable(nullptr)
  , m_infoTable(nullptr)
  , m_edDebit(nullptr)
  , m_edCredit(nullptr)
  , m_edComission(nullptr)
  , m_edSector(nullptr)
  , m_edCoin(nullptr)
  , m_edTotal(nullptr)
  , m_edRealTotal(nullptr)
  , m_edSales(nullptr)
  , m_edCards(nullptr)
  , m_edDiff1(nullptr)
  , m_edDiff2(nullptr)
  , m_btnCalc(nullptr)
  , m_btnPostit(nullptr)
  , m_filter(nullptr)
{
  m_viewer->setSortRole(Qt::EditRole);
  m_cashPicker = new DatabasePicker(CASH_SQL_TABLE_NAME);
  m_cashPicker->setPlaceholderText(true);
  m_dt = new JLineEdit(Text::Input::All, false);
  m_dt->setReadOnly(true);
  m_dt->setPlaceholderText(tr("Horário"));
  m_dt->hide();
  m_coinTable = new CashClosingCoinTable;
  m_sectorTable = new CashClosingSectorTable;
  m_infoTable = new CashClosingInfoTable;
  m_edSector = new JExpLineEdit(Data::Type::Money);
  m_edCoin = new JExpLineEdit(Data::Type::Money);
  m_edTotal = new JExpLineEdit(Data::Type::Money);
  m_edRealTotal = new JExpLineEdit(Data::Type::Money);
  m_edSales = new JExpLineEdit(Data::Type::Money);
  m_edCards = new JExpLineEdit(Data::Type::Money);
  m_edDiff1 = new JExpLineEdit(Data::Type::Money);
  m_edDiff2 = new JExpLineEdit(Data::Type::Money);
  m_edDebit = new JExpLineEdit(Data::Type::Money);
  m_edCredit = new JExpLineEdit(Data::Type::Money);
  m_edComission = new JExpLineEdit(Data::Type::Money);
  m_edSector->setReadOnly(true);
  m_edCoin->setReadOnly(true);
  m_edTotal->setReadOnly(true);
  m_edRealTotal->setReadOnly(true);
  m_edSales->setReadOnly(true);
  m_edDiff1->setReadOnly(true);
  m_edDiff2->setReadOnly(true);
  m_edDebit->setInvertColors(true);
  m_edTotal->setInvertColors(true);
  m_edRealTotal->setInvertColors(true);
  m_edSales->setInvertColors(true);
  m_edCards->setInvertColors(true);
  m_edCoin->setInvertColors(true);
  m_edDiff1->setInvertColors(true);
  m_edDiff2->setInvertColors(true);
  m_edTotal->setToolTip(tr("Total dos recebimentos descontando as taxas."));
  m_edRealTotal->setToolTip(tr("Total dos recebimentos descontando as taxas e comissões."));
  m_edSales->setToolTip(tr("Total dos recebimentos descontando as taxas mais as assinadas, menos os créditos e comissões."));
  m_edCards->setToolTip(tr("Total bruto recebido em meios digitais de pagamentos que envolvem taxas."));
  m_edDiff1->setToolTip(tr("Diferença entre entradas e recebimentos (bruto). Representa o que faltou ou sobrou no caixa. Um valor próximo de zero indica que o caixa fechou."));
  m_edDiff2->setToolTip(tr("Diferença entre entradas e recebimentos (líquido). Representa a quebra de caixa + o valor pago em taxas."));
  m_btnCalc = new QPushButton(QIcon(":/icons/res/calculator.png"), "");
  m_btnCalc->setFlat(true);
  m_btnCalc->setIconSize(QSize(24, 24));
  m_ltButton->addWidget(m_btnCalc);
  m_btnPostit = new QPushButton(QIcon(":/icons/res/postit2.png"), "");
  m_btnPostit->setFlat(true);
  m_btnPostit->setIconSize(QSize(24, 24));
  m_ltButton->addWidget(m_btnPostit);

  m_filter = new CashClosingFilter;
  m_tabDb->addTab(m_filter, QIcon(":/icons/res/filter.png"), tr("Filtro"));

  QLabel* cash = new QLabel(tr("Caixa"));
  QLabel* sector = new QLabel(tr("Entradas"));
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

  auto ldebit = new QFormLayout;
  ldebit->addRow(tr("Assinadas:"), m_edDebit);
  auto lcredit = new QFormLayout;
  lcredit->addRow(tr("Créditos:"), m_edCredit);
  auto lcomission = new QFormLayout;
  lcomission->addRow(tr("Comissões"), m_edComission);
  auto lextras = new QHBoxLayout;
  lextras->addLayout(ldebit);
  lextras->addLayout(lcredit);
  lextras->addLayout(lcomission);

  auto linfo = new QVBoxLayout;
  linfo->addWidget(info);
  linfo->addWidget(m_infoTable);

  auto results1 = new QFormLayout;
  results1->setAlignment(Qt::AlignTop);
  results1->addRow(tr("Entradas:"), m_edSector);
  results1->addRow(tr("Recebimentos:"), m_edCoin);
  results1->addRow(tr("Diferença de caixa:"), m_edDiff2);
  results1->addRow(tr("Quebra de Caixa:"), m_edDiff1);

  auto results2 = new QFormLayout;
  results2->addRow(tr("Total:"), m_edTotal);
  results2->addRow(tr("Total Real:"), m_edRealTotal);
  results2->addRow(tr("Venda Real:"), m_edSales);
  results2->addRow(tr("Cartão:"), m_edCards);

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
  lmain->addWidget(m_dt);
  lmain->addLayout(ltables1);
  lmain->addLayout(lextras);
  lmain->addLayout(ltables2);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(lmain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/cashier.png"), tr("Fechamento de Caixa"));

  m_btnPrint->setEnabled(true);
  m_btnPrint->show();

  connect(m_cashPicker, SIGNAL(changedSignal()), this, SLOT(cashChanged()));
  connect(m_sectorTable, SIGNAL(changedSignal(int, int)), this, SLOT(update()));
  connect(m_coinTable, SIGNAL(changedSignal(int, int)), this, SLOT(update()));
  connect(m_edDebit, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_edCredit, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_edComission, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_btnCalc, SIGNAL(clicked(bool)), this, SLOT(showCalculator()));
  connect(m_btnPostit, SIGNAL(clicked(bool)), this, SLOT(showPostit()));
  connect(m_filter, SIGNAL(filterChangedSignal(const QString&)), m_viewer, SLOT(setDynamicFilter(const QString&)));

  setFocusWidgetOnClear(m_cashPicker);
  m_viewer->refresh();
  clear();
}

void CashClosingView::getItem(JItemSQL& o) const
{
  CashClosing& _o = dynamic_cast<CashClosing&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_cash.m_id = m_cashPicker->getFirstId();
  _o.m_dt = QDateTime::fromString(m_dt->text(), "dd/MM/yyyy hh:mm:ss");
  m_coinTable->get(_o.m_vcoins);
  m_sectorTable->get(_o.m_vsectors);
  m_infoTable->get(_o.m_vinfos);
  _o.m_debit = m_edDebit->value();
  _o.m_credit = m_edCredit->value();
  _o.m_comission = m_edComission->value();
}

void CashClosingView::setItem(const JItemSQL& o)
{
  const CashClosing& _o = static_cast<const CashClosing&>(o);
  m_sectorTable->set(_o.m_vsectors);
  m_coinTable->set(_o.m_vcoins);
  m_infoTable->set(_o.m_vinfos);
  m_dt->setText(_o.m_dt.toString("dd/MM/yyyy hh:mm:ss"));
  m_dt->setVisible(_o.m_id.isValid());

  m_cashPicker->blockSignals(true);
  m_cashPicker->setEnabled(!_o.m_id.isValid());
  m_cashPicker->clear();
  m_cashPicker->addItem(_o.m_cash);
  m_cashPicker->blockSignals(false);

  m_edDebit->setValue(_o.m_debit);
  m_edCredit->setValue(_o.m_credit);
  m_edComission->setValue(_o.m_comission);

  m_sectorTable->setFocus();
  update();
}

void CashClosingView::save()
{
  CashClosing o;
  getItem(o);
  QString error;
  const Id id = m_id;
  if (id.isValid())
  {
    JItemView::save();
    o.m_id = id;
    o.SQL_select(error);
  }
  else
  {
    JItemView::save();
    o.m_cash.SQL_select(error);
  }
  if (m_btnPrint->isChecked())
    JItemHelper::print(o, 0, this);
}

void CashClosingView::cashChanged()
{
  CashClosing o;
  o.m_cash.m_id = m_cashPicker->getFirstId();
  QString error;
  bool ok = o.m_cash.SQL_select(error);
  if (ok)
  {
    for (const auto& _o : o.m_cash.m_vsectors)
    {
      CashClosingSector s;
      s.m_sname = _o.m_sector.m_name;
      s.m_simage = _o.m_sector.m_image;
      o.m_vsectors.push_back(s);
    }
    for (const auto& _o : o.m_cash.m_vcoins)
    {
      CashClosingCoin c;
      c.m_cname = _o.m_coin.m_name;
      c.m_ctax = _o.m_coin.m_tax;
      c.m_cimage = _o.m_coin.m_image;
      o.m_vcoins.push_back(c);
    }
    for (const auto& _o : o.m_cash.m_vinfos)
    {
      CashClosingInfo nfo;
      nfo.m_iname = _o.m_name;
      nfo.m_itype = (int)_o.m_type;
      o.m_vinfos.push_back(nfo);
    }
  }

  setItem(o);
}

void CashClosingView::update()
{
  m_btnSave->setEnabled(m_cashPicker->getFirstId().isValid());
  m_edSector->setValue(m_sectorTable->sum((int)CashClosingSectorTable::Column::Value));
  m_edCoin->setValue(m_coinTable->sum((int)CashClosingCoinTable::Column::Value));
  m_edTotal->setValue(m_coinTable->sumWithTaxes());
  m_edRealTotal->setValue(m_coinTable->sumWithTaxes() - m_edComission->value());
  m_edSales->setValue(m_coinTable->sumWithTaxes() + m_edDebit->value() - m_edCredit->value() - m_edComission->value());
  m_edCards->setValue(m_coinTable->cards());
  m_edDiff1->setValue(m_edCoin->value() - m_edSector->value());
  m_edDiff2->setValue(m_edTotal->value() - m_edSector->value());
}

void CashClosingView::showCalculator()
{
  CalculatorDialog dlg(this);
  dlg.exec();
}

void CashClosingView::showPostit()
{
  PostItDialog postit;
  postit.exec();
}
