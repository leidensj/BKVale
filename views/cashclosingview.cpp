#include "cashclosingview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/cashclosingcointable.h"
#include "tables/cashclosingsectortable.h"
#include "tables/cashclosinginfotable.h"
#include "controls/calculatorwidget.h"
#include "controls/postitdialog.h"
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
  , m_edSector1(nullptr)
  , m_edSector2(nullptr)
  , m_edCoin1(nullptr)
  , m_edCoin2(nullptr)
  , m_edDiff1(nullptr)
  , m_edDiff2(nullptr)
  , m_btnCalc(nullptr)
  , m_btnPostit(nullptr)
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
  m_btnCalc = new QPushButton(QIcon(":/icons/res/calculator.png"), "");
  m_btnCalc->setFlat(true);
  m_btnCalc->setIconSize(QSize(24, 24));
  m_ltButton->addWidget(m_btnCalc);
  m_btnPostit = new QPushButton(QIcon(":/icons/res/postit2.png"), "");
  m_btnPostit->setFlat(true);
  m_btnPostit->setIconSize(QSize(24, 24));
  m_ltButton->addWidget(m_btnPostit);

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
  results1->addRow(tr("Vendas (Valor):"),m_edSector1);
  results1->addRow(tr("Recebimentos (Valor Bruto):"),m_edCoin1);
  results1->addRow(tr("Quebra de Caixa:"),m_edDiff1);

  auto results2 = new QFormLayout;
  results2->addRow(tr("Vendas (Quantidade):"),m_edSector2);
  results2->addRow(tr("Recebimentos (Valor Líquido):"),m_edCoin2);
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
  lmain->addWidget(m_dt);
  lmain->addLayout(ltables1);
  lmain->addLayout(ltables2);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(lmain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/cashier.png"), tr("Fechamento de Caixa"));

  m_btnPrint->setEnabled(true);
  m_btnPrint->show();

  connect(m_cashPicker, SIGNAL(changedSignal()), this, SLOT(cashChanged()));
  connect(m_sectorTable, SIGNAL(changedSignal(int, int)), this, SLOT(update()));
  connect(m_coinTable, SIGNAL(changedSignal(int, int)), this, SLOT(update()));
  connect(m_btnCalc, SIGNAL(clicked(bool)), this, SLOT(showCalculator()));
  connect(m_btnPostit, SIGNAL(clicked(bool)), this, SLOT(showPostit()));

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

  m_sectorTable->setFocus();
  update();
}

void CashClosingView::save()
{
  print();
  JItemView::save();
}

bool CashClosingView::print()
{
  if (!m_btnPrint->isChecked())
    return true;
  CashClosing o;
  getItem(o);
  QString error;
  bool ok = false;
  if (m_id.isValid())
  {
    o.m_id = m_id;
    ok = o.SQL_select(error);
  }
  else
    ok = o.m_cash.SQL_select(error);

  if (ok)
  {
    EscPosPrinter printer;
    ok = printer.connectToPrinter(error);
    if (ok)
      ok = printer.printRawData(o.printVersion(), error);
  }

  if (!ok)
    QMessageBox::warning(this, tr("Erro ao imprimir"), error, QMessageBox::Ok);

  return ok;
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
  m_edSector1->setValue(m_sectorTable->sum((int)CashClosingSectorTable::Column::Value));
  m_edSector2->setValue(m_sectorTable->sum((int)CashClosingSectorTable::Column::NValue));
  m_edCoin1->setValue(m_coinTable->sum((int)CashClosingCoinTable::Column::Value));
  m_edCoin2->setValue(m_coinTable->sumWithTaxes());
  m_edDiff1->setValue(m_edCoin1->value() - m_edSector1->value());
  m_edDiff2->setValue(m_edCoin2->value() - m_edSector1->value());
}

void CashClosingView::showCalculator()
{
  auto calc = new CalculatorWidget;
  QDialog dlg(this);
  QHBoxLayout *l = new QHBoxLayout;
  dlg.setLayout(l);
  l->addWidget(calc);
  dlg.setWindowFlags(Qt::Dialog);
  dlg.setWindowTitle(tr("Calculadora"));
  dlg.setWindowIcon(QIcon(":/icons/res/calculator.png"));
  dlg.setModal(true);
  dlg.exec();
}

void CashClosingView::showPostit()
{
  PostItDialog postit;
  postit.exec();
}
