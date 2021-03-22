#include "shopwidget.h"
#include "controls/databaseviewer.h"
#include "items/jitemex.h"
#include "widgets/jdatepicker.h"
#include <QLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>

ShopWidget::ShopWidget(QWidget* parent)
  : QWidget(parent)
  , m_viewer(nullptr)
  , m_dt(nullptr)
  , m_btnPrint(nullptr)
{
  m_viewer = new DatabaseViewer(SHOPPING_LIST_SQL_TABLE_NAME, DatabaseViewer::Mode::ReadOnly);
  m_viewer->layout()->setContentsMargins(0, 0, 0, 0);
  m_dt = new JDatePicker;
  m_dt->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
  m_btnPrint = new QPushButton;
  m_btnPrint->setFlat(true);
  m_btnPrint->setText("");
  m_btnPrint->setIconSize(QSize(24, 24));
  m_btnPrint->setIcon(QIcon(":/icons/res/printer.png"));
  m_btnPrint->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
  m_btnPrint->setToolTip(tr("Imprimir (Ctrl+P)"));

  QHBoxLayout* headerLayout = new QHBoxLayout;
  headerLayout->setContentsMargins(0, 0, 0, 0);
  headerLayout->setAlignment(Qt::AlignLeft);
  headerLayout->addWidget(m_btnPrint);
  headerLayout->addWidget(m_dt);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(headerLayout);
  mainLayout->addWidget(m_viewer);
  setLayout(mainLayout);

  connect(m_dt, SIGNAL(dateChangedSignal(const QDate&)), this, SLOT(updateControls()));
  connect(m_viewer, SIGNAL(currentRowChangedSignal(int)), this, SLOT(updateControls()));
  connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(print()));
  updateControls();
}

void ShopWidget::updateControls()
{
  if (sender() == m_dt)
  {
    QDate date = m_dt->getDate();
    QString filter(SHOPPING_LIST_SQL_COL_WEE " LIKE '%" SHOPPING_LIST_SEPARATOR +
                   QString::number(date.dayOfWeek()) + SHOPPING_LIST_SEPARATOR "%'"
                   " OR " SHOPPING_LIST_SQL_COL_MON " LIKE '%" SHOPPING_LIST_SEPARATOR +
                   QString::number(date.day()) + SHOPPING_LIST_SEPARATOR "%'");
    m_viewer->setFixedFilter(filter);
  }
  auto o = getShoppingList();
  m_btnPrint->setEnabled(o.m_id.isValid());
}

ShoppingList ShopWidget::getShoppingList()
{
  ShoppingList o(m_viewer->getFirstSelectedId());
  JItemEx::select(o, this);
  return o;
}

void ShopWidget::print()
{
  int ret = QMessageBox::question(this, tr("Imprimir Lista"), tr("Imprimir contagem?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
  switch (ret)
  {
    case QMessageBox::Yes:
    case QMessageBox::No:
    {
      QVariant var(ret == QMessageBox::Yes);
      JItemEx::print(getShoppingList(), &var, this);
    } break;
    case QMessageBox::Cancel:
    default:
      return;
  }
}
