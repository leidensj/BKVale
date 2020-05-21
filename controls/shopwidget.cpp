#include "shopwidget.h"
#include "controls/databaseviewer.h"
#include "defines.h"
#include "printutils.h"
#include "escpos.h"
#include "items/jitemex.h"
#include <QLabel>
#include "widgets/jdatepicker.h"
#include <QLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QDialogButtonBox>

ShopWidget::ShopWidget(QWidget* parent)
  : QWidget(parent)
  , m_viewer(nullptr)
  , m_dt(nullptr)
  , m_lblWeekDay(nullptr)
{
  m_viewer = new DatabaseViewer(SHOPPING_LIST_SQL_TABLE_NAME, DatabaseViewer::Mode::ReadOnly);
  m_viewer->layout()->setContentsMargins(0, 0, 0, 0);
  m_dt = new JDatePicker;
  m_lblWeekDay = new QLabel;

  QHBoxLayout* headerLayout = new QHBoxLayout;
  headerLayout->setContentsMargins(0, 0, 0, 0);
  headerLayout->setAlignment(Qt::AlignLeft);
  headerLayout->addWidget(m_dt);
  headerLayout->addWidget(m_lblWeekDay);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(headerLayout);
  mainLayout->addWidget(m_viewer);
  setLayout(mainLayout);

  connect(m_dt, SIGNAL(dateChangedSignal(const QDate&)), this, SLOT(updateControls()));
  connect(m_dt, SIGNAL(dateChangedSignal(const QDate&)), this, SLOT(emitChangedSignal()));
  connect(m_viewer, SIGNAL(currentRowChangedSignal(int)), this, SLOT(emitChangedSignal()));
  updateControls();
}

void ShopWidget::updateControls()
{
  QDate date = m_dt->getDate();
  m_lblWeekDay->setText(date.toString("dddd"));
  QString filter(SHOPPING_LIST_SQL_COL_WEE " LIKE '%" SHOPPING_LIST_SEPARATOR +
                 QString::number(date.dayOfWeek()) + SHOPPING_LIST_SEPARATOR "%'"
                 " OR " SHOPPING_LIST_SQL_COL_MON " LIKE '%" SHOPPING_LIST_SEPARATOR +
                 QString::number(date.day()) + SHOPPING_LIST_SEPARATOR "%'");
  m_viewer->setFixedFilter(filter);
}

ShoppingList ShopWidget::getShoppingList()
{
  ShoppingList o(m_viewer->getFirstSelectedId());
  JItemEx::select(o, this);
  return o;
}

void ShopWidget::emitChangedSignal()
{
  emit changedSignal();
}

ShopPrintDialog::ShopPrintDialog(QWidget* parent)
  : QDialog(parent)
  , m_cbCount(nullptr)
{
  m_cbCount = new QCheckBox;
  m_cbCount->setText(tr("Imprimir contagem"));
  m_cbCount->setChecked(true);

  QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btn, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_cbCount);
  lt->addWidget(btn);
  lt->setSizeConstraint(QLayout::SetFixedSize);

  setWindowTitle(tr("Imprimir"));
  setWindowIcon(QIcon(":/icons/res/printer.png"));

  setLayout(lt);
  m_cbCount->setFocus();
}

bool ShopPrintDialog::getCount() const
{
  return m_cbCount->isChecked();
}
