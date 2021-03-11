#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "printer.h"

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

#include "controls/report.h"
#include "controls/calculatorwidget.h"
#include "controls/shopwidget.h"
#include "controls/pincodedialog.h"
#include "controls/logindialog.h"
#include "controls/databaseviewer.h"
#include "controls/timecarddialog.h"
#include "controls/couponredeemer.h"

#include "widgets/jstatusprogressbarinstance.h"
#include "widgets/jstatusmessageinstance.h"

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
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
  , m_purchaseWindow(nullptr)
  , m_reportWindow(nullptr)
  , m_reminderWindow(nullptr)
  , m_calculatorWindow(nullptr)
  , m_shopWindow(nullptr)
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

  connect(ui->actionPrint, SIGNAL(triggered(bool)), this, SLOT(print()));
  connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettingsDialog()));
  connect(ui->actionInfo, SIGNAL(triggered(bool)), this, SLOT(showInfo()));
  connect(m_purchase, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_report, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_reminder, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_calculator, SIGNAL(lineSignal(const QString&)), this, SLOT(print(const QString&)));
  connect(ui->actionLogin, SIGNAL(triggered(bool)), this, SLOT(openLoginDialog()));
  connect(m_shop, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(ui->actionPurchases, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReports, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReminders, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionCalculator, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionShoppingList, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionTimeCard, SIGNAL(triggered(bool)), this, SLOT(testTimeAccess()));
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

  connect(ui->actionLogged, SIGNAL(triggered(bool)), this, SLOT(openLoggedDialog()));

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

Functionality Baita::getCurrentFunctionality() const
{
  QMdiSubWindow* activeWindow = m_mdi->activeSubWindow();
  if (activeWindow == m_purchaseWindow)
    return Functionality::Purchase;
  if (activeWindow == m_reportWindow)
    return Functionality::Report;
  else if (activeWindow == m_reminderWindow)
    return Functionality::Reminder;
  else if (activeWindow == m_calculatorWindow)
    return Functionality::Calculator;
  else if (activeWindow == m_shopWindow)
    return Functionality::Shop;
  return Functionality::None;
}

void Baita::print()
{
  bool ok = true;
  QString error;
  switch (getCurrentFunctionality())
  {
    case Functionality::Purchase:
    {
      Purchase o;
      m_purchase->getItem(o);
      if (o.m_date != QDate::currentDate() && !o.m_id.isValid())
      {
        if (QMessageBox::question(
              this,
              tr("Data"),
              tr("A data informada é diferente da data de hoje.\nDeseja usar a data de hoje?"),
              QMessageBox::Yes | QMessageBox::No,
              QMessageBox::Yes) == QMessageBox::Yes)
        {
           m_purchase->setDate(QDate::currentDate());
        }
      }
      o.clear(true);
      if (m_purchase->save(o.m_id))
      {
        if (o.SQL_select(error))
          ok = m_printer.print(o, error);
      }
    } break;
    case Functionality::Reminder:
    {
      ReminderPrintDialog dlg(this);
      if (!dlg.exec())
        break;
      Reminder o;
      m_reminder->getItem(o);
      if (dlg.getSave())
      {
        Id id;
        ok = m_reminder->save(id);
      }

      if (ok)
      {
        for (int i = 0; i != dlg.getCopies(); ++i)
        {
          ok = m_printer.print(o, error);
          if (ok)
            break;
        }
      }

      if (ok)
        m_reminder->clear();
    } break;
    case Functionality::Calculator:
    {
      ok = m_printer.print(m_calculator->getFullContent() + Printer::st_strFullCut(), error);
    } break;
    case Functionality::Shop:
    {
      ShopPrintDialog dlg;
      if (dlg.exec())
        ok = m_printer.print(m_shop->getShoppingList(), dlg.getCount(), error);
    } break;
    case Functionality::None:
    default:
      break;
  }
  if (!ok)
    QMessageBox::critical(this, ("Erro ao imprimir item"), error, QMessageBox::Ok);
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
  ui->actionSettings->setEnabled(login.getUser().m_bSettings);
  ui->actionUsers->setEnabled(login.getUser().m_bUser);
  ui->actionProducts->setEnabled(login.getUser().m_bProduct);
  ui->actionCategories->setEnabled(login.getUser().m_bCategory);
  ui->actionImages->setEnabled(login.getUser().m_bImage);
  ui->actionShoppingListMgt->setEnabled(login.getUser().m_bShoppingList);
  ui->actionEmployees->setEnabled(login.getUser().m_bEmployee);
  ui->actionSuppliers->setEnabled(login.getUser().m_bSupplier);
  ui->actionStores->setEnabled(login.getUser().m_bStore);
  ui->actionTimeCard->setEnabled(login.getUser().m_bTimeCard);
  ui->actionCoupons->setEnabled(true);

  ui->actionPurchases->setEnabled(login.getUser().m_bPurchase);
  ui->actionReminders->setEnabled(login.getUser().m_bReminder);
  ui->actionCalculator->setEnabled(login.getUser().m_bCalculator);
  ui->actionShoppingList->setEnabled(login.getUser().m_bShop);

  switch (getCurrentFunctionality())
  {
    case Functionality::Purchase:
    {
      Purchase o;
      m_purchase->getItem(o);
      ui->actionPrint->setEnabled(o.isValid());
    }  break;
    case Functionality::Report:
      ui->actionPrint->setEnabled(false);
      break;
    case Functionality::Reminder:
    {
      Reminder o;
      m_reminder->getItem(o);
      ui->actionPrint->setEnabled(o.isValid());
    } break;
    case Functionality::Calculator:
      ui->actionPrint->setEnabled(true);
      break;
    case Functionality::Shop:
      ui->actionPrint->setEnabled(m_shop->getShoppingList().m_id.isValid());
      break;
    default:
      break;
  }
}

void Baita::showInfo()
{

}

void Baita::openJItemSQLDialog()
{
  JItemView* view = nullptr;
  if (sender() == ui->actionCategories)
    view = new CategoryView;
  else if (sender() == ui->actionEmployees)
    view = new EmployeeView;
  else if (sender() == ui->actionImages)
    view = new ImageView;
  else if (sender() == ui->actionProducts)
    view = new ProductView;
  else if (sender() == ui->actionShoppingListMgt)
    view = new ShoppingListView;
  else if (sender() == ui->actionStores)
    view = new StoreView;
  else if (sender() == ui->actionSuppliers)
    view = new SupplierView;
  else if (sender() == ui->actionUsers)
    view = new UserView;
  else if (sender() == ui->actionCoupons)
    view = new CouponView;

  if (view != nullptr)
  {
    auto pt = static_cast<QAction*>(sender());
    QDialog dlg(this);
    QHBoxLayout *layout = new QHBoxLayout;
    dlg.setLayout(layout);
    layout->addWidget(view);
    dlg.setWindowFlags(Qt::Window);
    dlg.setWindowTitle(tr("Gerenciar ") + pt->text());
    dlg.setWindowIcon(pt->icon());
    dlg.setModal(true);
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
  updateControls();
}

void Baita::testTimeAccess()
{
  TimeCardDialog dlg(this);
  dlg.exec();
}
