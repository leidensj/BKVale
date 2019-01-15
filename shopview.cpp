#include "shopview.h"
#include "jdatabase.h"
#include "defines.h"
#include "printutils.h"
#include "escpos.h"
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QLayout>
#include <QMessageBox>
#include <QCheckBox>

ShopView::ShopView(QWidget* parent)
  : QFrame(parent)
  , m_database(nullptr)
  , m_dtDate(nullptr)
  , m_btnToday(nullptr)
  , m_lblWeekDay(nullptr)
  , m_cbPrintCount(nullptr)
{
  m_database = new JDatabase(SHOPPING_LIST_SQL_TABLE_NAME, JDatabase::Mode::ReadOnly);
  m_database->layout()->setContentsMargins(0, 0, 0, 0);
  m_dtDate = new QDateEdit;
  m_dtDate->setCalendarPopup(true);
  m_dtDate->setDate(QDate::currentDate());
  m_btnToday = new QPushButton;
  m_btnToday->setFlat(true);
  m_btnToday->setIconSize(QSize(24, 24));
  m_btnToday->setIcon(QIcon(":/icons/res/calendarok.png"));
  m_btnToday->setToolTip(tr("Usar a data de hoje"));
  m_lblWeekDay = new QLabel;
  m_cbPrintCount = new QCheckBox;
  m_cbPrintCount->setText(tr("Imprimir contagem"));
  m_cbPrintCount->setChecked(true);
  QFrame* vFrame0 = new QFrame;
  vFrame0->setFrameShape(QFrame::VLine);

  QHBoxLayout* headerLayout = new QHBoxLayout;
  headerLayout->setContentsMargins(0, 0, 0, 0);
  headerLayout->setAlignment(Qt::AlignLeft);
  headerLayout->addWidget(m_dtDate);
  headerLayout->addWidget(m_btnToday);
  headerLayout->addWidget(m_lblWeekDay);
  headerLayout->addWidget(vFrame0);
  headerLayout->addWidget(m_cbPrintCount);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(headerLayout);
  mainLayout->addWidget(m_database);
  setLayout(mainLayout);

  connect(m_btnToday, SIGNAL(clicked(bool)), this, SLOT(setToday()));
  connect(m_dtDate, SIGNAL(dateChanged(const QDate&)), this, SLOT(updateControls()));
  connect(m_dtDate, SIGNAL(dateChanged(const QDate&)), this, SLOT(emitChangedSignal()));
  connect(m_database, SIGNAL(currentRowChangedSignal(int)), this, SLOT(emitChangedSignal()));

  setToday();
  updateControls();
}

void ShopView::updateControls()
{
  QDate date = m_dtDate->date();
  m_btnToday->setIcon(QIcon(m_dtDate->date() == QDate::currentDate()
                            ? ":/icons/res/calendarok.png"
                            : ":/icons/res/calendarwarning.png"));
  m_lblWeekDay->setText(date.toString("dddd"));
  QString filter(SHOPPING_LIST_SQL_COL05 " LIKE '%" SHOPPING_LIST_SEPARATOR +
                 QString::number(date.dayOfWeek()) + SHOPPING_LIST_SEPARATOR "%'"
                 " OR " SHOPPING_LIST_SQL_COL06 " LIKE '%" SHOPPING_LIST_SEPARATOR +
                 QString::number(date.day()) + SHOPPING_LIST_SEPARATOR "%'");
  m_database->setFixedFilter(filter);
}

void ShopView::setToday()
{
  m_dtDate->setDate(QDate::currentDate());
  updateControls();
}

ShoppingList ShopView::getShoppingList() const
{
  m_database->selectItem();
  ShoppingList* plst = dynamic_cast<ShoppingList*>(m_database->getCurrentItem());
  ShoppingList lst;
  if (plst != nullptr)
    lst = *plst;
  return lst;
}

void ShopView::emitChangedSignal()
{
  emit changedSignal();
}

bool ShopView::printCount() const
{
  return m_cbPrintCount->isChecked();
}
