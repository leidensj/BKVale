#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include "views/productview.h"
#include "views/categoryview.h"
#include "views/purchaseview.h"
#include "views/employeeview.h"
#include "views/supplierview.h"
#include "views/reminderview.h"
#include "views/shoppinglistview.h"
#include "views/storeview.h"
#include "views/userview.h"
#include "views/imageview.h"
#include "views/couponview.h"
#include "views/inventoryview.h"
#include "views/sectorview.h"
#include "views/coinview.h"
#include "views/cashview.h"
#include "views/cashclosingview.h"

#include "controls/report.h"
#include "controls/calculatorwidget.h"
#include "controls/shopwidget.h"
#include "controls/pincodedialog.h"
#include "controls/logindialog.h"
#include "controls/databaseviewer.h"
#include "controls/timecarddialog.h"
#include "controls/timecardcontroldialog.h"
#include "controls/couponredeemer.h"

#include "widgets/jstatusprogressbarinstance.h"
#include "widgets/jstatusmessageinstance.h"

#include "items/jitemhelper.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>
#include <QLabel>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QTimer>

class JMdiSubWindow : public QMdiSubWindow
{
public:
  JMdiSubWindow(QWidget* parent = nullptr)
    : QMdiSubWindow(parent)
  {

  }

protected:
  void closeEvent(QCloseEvent *closeEvent)
  {
    closeEvent->ignore();
    hide();
  }
};

Baita::Baita(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Baita)
  , m_purchase(nullptr)
  , m_report(nullptr)
  , m_reminder(nullptr)
  , m_consumption(nullptr)
  , m_calculator(nullptr)
  , m_shop(nullptr)
  , m_inventory(nullptr)
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
  , m_purchaseWindow(nullptr)
  , m_reportWindow(nullptr)
  , m_reminderWindow(nullptr)
  , m_calculatorWindow(nullptr)
  , m_shopWindow(nullptr)
  , m_inventoryWindow(nullptr)
  , m_redeemer(nullptr)
{
  ui->setupUi(this);

  m_mdi = new JMdiArea;
  setCentralWidget(m_mdi);

  m_purchase = new PurchaseView;
  m_report = new Report;
  m_reminder = new ReminderView;
  m_calculator = new CalculatorWidget;
  m_shop = new ShopWidget;
  m_inventory = new InventoryView;

  m_purchaseWindow = new JMdiSubWindow(this);
  m_purchaseWindow->setWindowTitle(ui->actionPurchases->text());
  m_purchaseWindow->setWindowIcon(ui->actionPurchases->icon());
  m_purchaseWindow->setWidget(m_purchase);
  m_mdi->addSubWindow(m_purchaseWindow);
  m_reportWindow = new JMdiSubWindow(this);
  m_reportWindow->setWindowTitle(ui->actionReports->text());
  m_reportWindow->setWindowIcon(ui->actionReports->icon());
  m_reportWindow->setWidget(m_report);
  m_mdi->addSubWindow(m_reportWindow);
  m_reminderWindow = new JMdiSubWindow(this);
  m_reminderWindow->setWindowTitle(ui->actionReminders->text());
  m_reminderWindow->setWindowIcon(ui->actionReminders->icon());
  m_reminderWindow->setWidget(m_reminder);
  m_mdi->addSubWindow(m_reminderWindow);
  m_calculatorWindow = new JMdiSubWindow(this);
  m_calculatorWindow->setWindowTitle(ui->actionCalculator->text());
  m_calculatorWindow->setWindowIcon(ui->actionCalculator->icon());
  m_calculatorWindow->setWidget(m_calculator);
  m_mdi->addSubWindow(m_calculatorWindow);
  m_shopWindow = new JMdiSubWindow(this);
  m_shopWindow->setWindowTitle(ui->actionShoppingList->text());
  m_shopWindow->setWindowIcon(ui->actionShoppingList->icon());
  m_shopWindow->setWidget(m_shop);
  m_mdi->addSubWindow(m_shopWindow);
  m_inventoryWindow = new JMdiSubWindow(this);
  m_inventoryWindow->setWindowTitle(ui->actionInventory->text());
  m_inventoryWindow->setWindowIcon(ui->actionInventory->icon());
  m_inventoryWindow->setWidget(m_inventory);
  m_mdi->addSubWindow(m_inventoryWindow);
  m_redeemer = new CouponRedeemer(this);

  m_statusDatabasePath = new QLabel();
  m_statusDatabasePath->setAlignment(Qt::AlignRight);
  m_statusDatabasePath->setTextFormat(Qt::RichText);
  m_statusUserName = new QLabel;
  m_statusUserName->setAlignment(Qt::AlignCenter);
  m_statusUserName->setTextFormat(Qt::RichText);
  m_statusTime = new QLabel;
  m_statusTime->setAlignment(Qt::AlignCenter);
  auto p = JStatusProgressBarInstance::getInstance();
  auto l = JStatusMessageInstance::getInstance();
  if (p != nullptr)
  {
    p->setMaximumHeight(16);
    p->setRange(0, 0);
    p->hide();
    l->hide();
  }
  statusBar()->addWidget(m_statusDatabasePath);
  statusBar()->addWidget(m_statusUserName);
  statusBar()->addWidget(l);
  statusBar()->addWidget(p);
  statusBar()->addPermanentWidget(m_statusTime);

  m_actions[Functionality::Idx::Settings] = ui->actionSettings;
  m_actions[Functionality::Idx::User] = ui->actionUsers;
  m_actions[Functionality::Idx::Product] = ui->actionProducts;
  m_actions[Functionality::Idx::Category] = ui->actionCategories;
  m_actions[Functionality::Idx::Image] = ui->actionImages;
  m_actions[Functionality::Idx::ShoppingList] = ui->actionShoppingListMgt;
  m_actions[Functionality::Idx::Employee] = ui->actionEmployees;
  m_actions[Functionality::Idx::Supplier] = ui->actionSuppliers;
  m_actions[Functionality::Idx::Store] = ui->actionStores;
  m_actions[Functionality::Idx::TimeCard] = ui->actionTimeCard;
  m_actions[Functionality::Idx::TimeCardControl] = ui->actionTimeCardControl;
  m_actions[Functionality::Idx::Coupon] = ui->actionCoupons;
  m_actions[Functionality::Idx::Shop] = ui->actionShoppingList;
  m_actions[Functionality::Idx::Calculator] = ui->actionCalculator;
  m_actions[Functionality::Idx::Purchase] = ui->actionPurchases;
  m_actions[Functionality::Idx::Report] = ui->actionReports;
  m_actions[Functionality::Idx::Reminder] = ui->actionReminders;
  m_actions[Functionality::Idx::CouponRedemption] = ui->actionRedeem;
  m_actions[Functionality::Idx::Inventory] = ui->actionInventory;
  m_actions[Functionality::Idx::Sector] = ui->actionSectors;
  m_actions[Functionality::Idx::Coin] = ui->actionCoins;
  m_actions[Functionality::Idx::Cash] = ui->actionCash;
  m_actions[Functionality::Idx::CashClosing] = ui->actionCashClosing;

  connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettingsDialog()));
  connect(m_purchase, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(ui->actionLogin, SIGNAL(triggered(bool)), this, SLOT(openLoginDialog()));
  connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(ui->actionPurchases, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReports, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReminders, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionCalculator, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionShoppingList, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionInventory, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionTimeCard, SIGNAL(triggered(bool)), this, SLOT(openTimeCardDialog()));
  connect(ui->actionTimeCardControl, SIGNAL(triggered(bool)), this, SLOT(openTimeCardControlDialog()));
  connect(ui->actionRedeem, SIGNAL(triggered(bool)), m_redeemer, SLOT(exec()));

  connect(ui->actionEmployees, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionShoppingListMgt, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionStores, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionProducts, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionCategories, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionUsers, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionImages, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionSuppliers, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionCoupons, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionSectors, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionCoins, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionCash, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionCashClosing, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));

  connect(ui->actionLogged, SIGNAL(triggered(bool)), this, SLOT(openLoggedDialog()));
  connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));

  activateWindow();
  m_settings.load();
  updateControls();
  updateStatusBar();
  updateTime();
}

Baita::~Baita()
{
  delete ui;
}

Functionality::Idx Baita::getCurrentFunctionality() const
{
  QMdiSubWindow* activeWindow = m_mdi->activeSubWindow();
  if (activeWindow == m_purchaseWindow)
    return Functionality::Idx::Purchase;
  if (activeWindow == m_reportWindow)
    return Functionality::Idx::Report;
  else if (activeWindow == m_reminderWindow)
    return Functionality::Idx::Reminder;
  else if (activeWindow == m_calculatorWindow)
    return Functionality::Idx::Calculator;
  else if (activeWindow == m_shopWindow)
    return Functionality::Idx::Shop;
  else if (activeWindow == m_inventoryWindow)
    return Functionality::Idx::Inventory;
  return Functionality::Idx::_END;
}

void Baita::openSettingsDialog()
{
  SettingsDialog dlg(m_settings);
  if (dlg.exec() == QDialog::Accepted)
  {
    m_settings = dlg.getSettings();
    m_settings.save();
  }
}

void Baita::updateStatusBar()
{
  // para adicionar um ícone:
  // "<img src=':/icons/res/16user.png'> " + ...

  Login login(true);
  m_statusUserName->setText(tr("Usuário: ") + login.getUser().m_strUser);
  m_statusDatabasePath->setText(tr("Banco de dados: ") +
                                (m_settings.m_databaseHostName.isEmpty() ? "localhost"
                                                                        : m_settings.m_databaseHostName) +
                                ":" +
                                QString::number(m_settings.m_databasePort));
}

void Baita::updateTime()
{
  QDateTime dt = DateTime::server();
  m_statusTime->setText(dt.date().toString("dd/MM/yyyy ") + dt.time().toString("hh:mm"));
  QTimer::singleShot(10000, this, &Baita::updateTime);
}

void Baita::updateControls()
{
  Login login(true);
  QMapIterator<Functionality::Idx, QAction*> i(m_actions);
  while (i.hasNext())
  {
    i.next();
    i.value()->setEnabled(login.getUser().hasPermission(i.key()));
  }
}

void Baita::openJItemSQLDialog()
{
  JItemView* view = nullptr;
  QMapIterator<Functionality::Idx, QAction*> i(m_actions);
  while (i.hasNext())
  {
    i.next();
    if (sender() == i.value())
    {
      view = JItemHelper::view(Functionality::idxToTableName(i.key()));
      break;
    }
  }
  if (view != nullptr)
  {
    JItemViewDialog dlg(view, this);
    dlg.exec();
  }
}

void Baita::openLoginDialog()
{
  hide();
  Login login(true);
  QString error;
  login.SQL_logout(error);
  LoginDialog l;
  if (!l.exec())
  {
    QMessageBox::critical(this,
                          tr("Login não realizado"),
                          tr("A aplicação será encerrada."),
                          QMessageBox::Ok);
    close();
  }
  else
  {
    show();
    updateControls();
    updateStatusBar();
  }
}


void Baita::openLoggedDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  DatabaseViewer* w = new DatabaseViewer(LOGIN_SQL_TABLE_NAME, DatabaseViewer::Mode::ReadOnly);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Usuários Ativos"));
  dlg.setWindowIcon(QIcon(":/icons/res/users.png"));
  dlg.setModal(true);
  dlg.exec();
}

void Baita::closeEvent(QCloseEvent* event)
{
  if (QMessageBox::question(this,
                            tr("Sair"),
                            tr("Tem certeza que deseja sair?"),
                            QMessageBox::Yes,
                            QMessageBox::No) == QMessageBox::Yes)
    QMainWindow::closeEvent(event);
  else
    event->ignore();
}

void Baita::activateWindow()
{
  m_purchaseWindow->hide();
  m_reportWindow->hide();
  m_reminderWindow->hide();
  m_calculatorWindow->hide();
  m_shopWindow->hide();
  m_inventoryWindow->hide();
  if (sender() == ui->actionPurchases)
  {
    m_purchaseWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_purchaseWindow);
  }
  else if (sender() == ui->actionReports)
  {
    m_reportWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_reportWindow);
  }
  else if (sender() == ui->actionReminders)
  {
    m_reminderWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_reminderWindow);
  }
  else if (sender() == ui->actionCalculator)
  {
    m_calculatorWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_calculatorWindow);
  }
  else if (sender() == ui->actionShoppingList)
  {
    m_shopWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_shopWindow);
  }
  else if (sender() == ui->actionInventory)
  {
    m_inventoryWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_inventoryWindow);
  }
  updateControls();
}

void Baita::openTimeCardDialog()
{
  TimeCardDialog dlg(this);
  dlg.exec();
}

void Baita::openTimeCardControlDialog()
{
  TimeCardControlDialog dlg(this);
  dlg.exec();
}

void Baita::about()
{
  AboutDialog dlg;
  dlg.exec();
}
