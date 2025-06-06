#include "shopwidget.h"
#include "controls/databaseviewer.h"
#include "items/jitemhelper.h"
#include "widgets/jdatepicker.h"
#include "tables/shoppinglisttable.h"
#include "escposprinter.h"
#include <QLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>

ShopWidget::ShopWidget(QWidget* parent)
  : QWidget(parent)
  , m_viewer(nullptr)
  , m_dt(nullptr)
  , m_btnPrint(nullptr)
  , m_btnView(nullptr)
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
  m_btnView = new QPushButton;
  m_btnView->setFlat(true);
  m_btnView->setText("");
  m_btnView->setIconSize(QSize(24, 24));
  m_btnView->setIcon(QIcon(":/icons/res/view.png"));
  m_btnView->setToolTip(tr("Visualizar"));

  QHBoxLayout* headerLayout = new QHBoxLayout;
  headerLayout->setContentsMargins(0, 0, 0, 0);
  headerLayout->setAlignment(Qt::AlignLeft);
  headerLayout->addWidget(m_btnPrint);
  headerLayout->addWidget(m_btnView);
  headerLayout->addWidget(m_dt);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(headerLayout);
  mainLayout->addWidget(m_viewer);
  setLayout(mainLayout);

  connect(m_dt, SIGNAL(dateChangedSignal(const QDate&)), this, SLOT(updateControls()));
  connect(m_viewer, SIGNAL(currentRowChangedSignal(int)), this, SLOT(enableControls()));
  connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(print()));
  connect(m_btnView, SIGNAL(clicked(bool)), this, SLOT(view()));
  updateControls();
}

void ShopWidget::updateControls()
{
  QDate date = m_dt->getDate();
  QString filter(SHOPPING_LIST_SQL_COL_WEE " LIKE '%" SHOPPING_LIST_SEPARATOR +
                 QString::number(date.dayOfWeek()) + SHOPPING_LIST_SEPARATOR "%'"
                 " OR " SHOPPING_LIST_SQL_COL_MON " LIKE '%" SHOPPING_LIST_SEPARATOR +
                 QString::number(date.day()) + SHOPPING_LIST_SEPARATOR "%'");
  m_viewer->setFixedFilter(filter);
}

void ShopWidget::enableControls()
{
  Id id = m_viewer->getFirstSelectedId();
  m_btnPrint->setEnabled(id.isValid());
  m_btnView->setEnabled(id.isValid());
}

ShoppingList ShopWidget::getShoppingList()
{
  ShoppingList o(m_viewer->getFirstSelectedId());
  JItemHelper::select(o, this);
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
        EscPosPrinter printer;
      QString error;
        bool ok = printer.connectToPrinter(error);
      if (ok)
        ok = printer.printRawData(getShoppingList().printVersion(var), error);
      if (!ok)
        QMessageBox::warning(this, tr("Erro ao imprimir"), error, QMessageBox::Ok);
    } break;
    case QMessageBox::Cancel:
    default:
      return;
  }
}

void ShopWidget::view()
{
  ShoppingList o = getShoppingList();
  auto table = new ShoppingListTable(nullptr, this);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setSelectionMode(QAbstractItemView::NoSelection);
  table->setListElements(o.m_vItem);
  table->show();
  bool bShowSupplierColumn = !o.m_supplier.m_id.isValid();
  if (bShowSupplierColumn)
  {
    bool bReallyNeedToShowSupplierColumn = false;
    for (int i = 0; i != o.m_vItem.size(); ++i)
      if (o.m_vItem.at(i).m_supplier.m_id.isValid())
        bReallyNeedToShowSupplierColumn = true;
    bShowSupplierColumn = bReallyNeedToShowSupplierColumn;
  }
  table->showSupplierColumn(bShowSupplierColumn);

  QDialog dlg(this);
  QVBoxLayout *layout = new QVBoxLayout;
  dlg.setLayout(layout);
  if (o.m_supplier.m_id.isValid())
  {
    QLabel* lblSupplier = new QLabel(tr("Fornecedor: ") + o.m_supplier.m_form.m_name);
    layout->addWidget(lblSupplier);
  }
  layout->addWidget(table);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(JItemHelper::text(Functionality::Idx::Shop) + ": " + o.m_title);
  dlg.setWindowIcon(QIcon(JItemHelper::icon(Functionality::Idx::Shop)));
  dlg.setModal(true);
  dlg.resize(640, 480);
  dlg.exec();
}
