#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "printutils.h"
#include "pincode.h"
#include "views/productview.h"
#include "views/categoryview.h"
#include "views/purchaseview.h"
#include "views/employeeview.h"
#include "views/supplierview.h"
#include "views/reminderview.h"
#include "views/shoppinglistview.h"
#include "views/storeview.h"
#include "calculatorwidget.h"
#include "views/userview.h"
#include "views/imageview.h"
#include "logindialog.h"
#include "shopview.h"
#include "widgets/jdatabase.h"
#include "timecard.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>
#include <QLabel>
#include <QCloseEvent>
#include <QInputDialog>
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

Goiabo::Goiabo(const ActiveUser& login, QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Goiabo)
  , m_login(login)
  , m_purchase(nullptr)
  , m_reminder(nullptr)
  , m_consumption(nullptr)
  , m_calculator(nullptr)
  , m_shop(nullptr)
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
  , m_purchaseWindow(nullptr)
  , m_reminderWindow(nullptr)
  , m_calculatorWindow(nullptr)
  , m_shopWindow(nullptr)
{
  ui->setupUi(this);

  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());

  m_mdi = new JMdiArea;
  setCentralWidget(m_mdi);

  m_purchase = new PurchaseView;
  m_reminder = new ReminderView;
  m_calculator = new CalculatorWidget;
  m_shop = new ShopView;

  m_purchaseWindow = new JMdiSubWindow(this);
  m_purchaseWindow->setWindowTitle(ui->actionPurchases->text());
  m_purchaseWindow->setWindowIcon(ui->actionPurchases->icon());
  m_purchaseWindow->setWidget(m_purchase);
  m_mdi->addSubWindow(m_purchaseWindow);
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

  m_statusDatabasePath = new QLabel();
  m_statusDatabasePath->setAlignment(Qt::AlignRight);
  m_statusDatabasePath->setTextFormat(Qt::RichText);
  m_statusUserName = new QLabel;
  m_statusUserName->setAlignment(Qt::AlignCenter);
  m_statusUserName->setTextFormat(Qt::RichText);
  m_statusTime = new QLabel;
  m_statusTime->setAlignment(Qt::AlignCenter);
  statusBar()->addWidget(m_statusDatabasePath);
  statusBar()->addWidget(m_statusUserName);
  statusBar()->addPermanentWidget(m_statusTime);

  connect(ui->actionPrint, SIGNAL(triggered(bool)), this, SLOT(print()));
  connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettingsDialog()));
  connect(ui->actionInfo, SIGNAL(triggered(bool)), this, SLOT(showInfo()));
  connect(m_purchase, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_reminder, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_calculator, SIGNAL(lineSignal(const QString&)), this, SLOT(print(const QString&)));
  connect(ui->actionLogin, SIGNAL(triggered(bool)), this, SLOT(openLoginDialog()));
  connect(m_shop, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(ui->actionPurchases, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReminders, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionCalculator, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionShoppingList, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionTimeCard, SIGNAL(triggered(bool)), this, SLOT(testTimeAccess()));

  connect(ui->actionEmployees, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionShoppingListMgt, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionStores, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionProducts, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionCategories, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionUsers, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionImages, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));
  connect(ui->actionSuppliers, SIGNAL(triggered(bool)), this, SLOT(openJItemSQLDialog()));

  connect(ui->actionActiveUsers, SIGNAL(triggered(bool)), this, SLOT(openActiveUsersDialog()));

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  timer->start(60000);

  activateWindow();
  m_settings.load();
  updateTime();
  updateControls();
  updateStatusBar();
}

Goiabo::~Goiabo()
{
  delete ui;
}

bool Goiabo::connectPrinter()
{
  if (m_printerSerial.isOpen())
    m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();

  if (m_settings.m_serialPort.isEmpty() &&
      m_settings.m_ethernetIP.isEmpty())
  {
    QMessageBox::warning(this,
                       tr("Porta ou endereço não informado"),
                       tr("É necessário selecionar uma porta serial "
                          "ou endereço para se conectar à impressora."),
                       QMessageBox::Ok);
    return false;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  QString error;
  bool bSuccess = false;
  if (m_settings.m_bIsPrinterEthernet)
  {
    m_printerTCP.connectToHost(m_settings.m_ethernetIP,
                               (quint16)m_settings.m_ethernetPort);
    bSuccess = m_printerTCP.waitForConnected();
    if (bSuccess)
      bSuccess = Printer::printString(&m_printerTCP,
                                      m_settings.m_bIsPrinterEthernet,
                                      Printer::strCmdInit(),
                                      error);
    else
      error = m_printerTCP.errorString();
  }
  else
  {
    m_printerSerial.setPortName(m_settings.m_serialPort);
    bSuccess = m_printerSerial.open(QIODevice::ReadWrite);
    if (bSuccess)
    {
      m_printerSerial.clear();
      bSuccess = Printer::printString(&m_printerSerial,
                                      m_settings.m_bIsPrinterEthernet,
                                      Printer::strCmdInit(),
                                      error);
    }
    else
      error = m_printerSerial.errorString();
  }


  QApplication::restoreOverrideCursor();

  if (!bSuccess)
  {
    QMessageBox::critical(this,
                       tr("Erro"),
                       tr("O seguinte erro ocorreu ao conectar à impressora:\n"
                          "'%1'.").arg(error),
                       QMessageBox::Ok);
  }
  return bSuccess;
}

void Goiabo::disconnectPrinter()
{
  if (m_printerSerial.isOpen())
      m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();
}

Functionality Goiabo::getCurrentFunctionality() const
{
  QMdiSubWindow* activeWindow = m_mdi->activeSubWindow();
  if (activeWindow == m_purchaseWindow)
    return Functionality::Purchase;
  else if (activeWindow == m_reminderWindow)
    return Functionality::Reminder;
  else if (activeWindow == m_calculatorWindow)
    return Functionality::Calculator;
  else if (activeWindow == m_shopWindow)
    return Functionality::Shop;
  return Functionality::None;
}

void Goiabo::print()
{
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
        QString error;
        if (o.SQL_select(error))
          print(PurchasePrinter::build(o));
        else
          QMessageBox::critical(this, ("Erro ao selecionar item"), error, QMessageBox::Ok);
      }
    } break;
    case Functionality::Reminder:
    {
      ReminderPrintDialog dlg(this);
      if (!dlg.exec())
        break;

      bool bSuccess = true;
      Reminder o;
      m_reminder->getItem(o);
      if (dlg.getSave())
      {
        Id id;
        bSuccess = m_reminder->save(id);
      }

      if (bSuccess)
      {
        QString str = ReminderPrinter::build(o);
        for (int i = 0; i != dlg.getCopies(); ++i)
        {
          bSuccess = print(str);
          if (!bSuccess)
            break;
        }
      }

      if (bSuccess)
        m_reminder->create();

    } break;
    case Functionality::Calculator:
    {
      print(m_calculator->getFullContent() + Printer::strCmdFullCut());
    } break;
    case Functionality::Shop:
    {
      ShopPrintDialog dlg;
      if (dlg.exec())
        print(ShoppingListPrinter::build(m_shop->getShoppingList(), dlg.getCount()));
    } break;
    case Functionality::None:
    default:
      break;
  }
}

bool Goiabo::print(const QString& text)
{
  bool bSuccess = connectPrinter();
  if (bSuccess)
  {
    QIODevice* printer = m_settings.m_bIsPrinterEthernet
                         ? (QIODevice*)&m_printerTCP
                         : (QIODevice*)&m_printerSerial;

    QString error;
    bSuccess = Printer::printString(printer, m_settings.m_bIsPrinterEthernet, text, error);
    if (!bSuccess)
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao imprimir.").arg(error),
                            QMessageBox::Ok);
    }
    disconnectPrinter();
  }
  return bSuccess;
}

void Goiabo::openSettingsDialog()
{
  SettingsDlg dlg(m_settings);
  if (dlg.exec() == QDialog::Accepted)
  {
    m_settings = dlg.getSettings();
    m_settings.save();
  }
}

void Goiabo::updateStatusBar()
{
  // para adicionar um ícone:
  // "<img src=':/icons/res/16user.png'> " + ...

  m_statusUserName->setText(tr("Usuário: ") + m_login.getUser().m_strUser);
  m_statusDatabasePath->setText(tr("Banco de dados: ") +
                                (m_settings.m_databaseHostName.isEmpty() ? "localhost"
                                                                        : m_settings.m_databaseHostName) +
                                ":" +
                                QString::number(m_settings.m_databasePort));
}

void Goiabo::updateTime()
{
  QDateTime dt = DateTime::server();
  m_statusTime->setText(dt.date().toString("dd/MM/yyyy ") + dt.time().toString("hh:mm"));
}

void Goiabo::updateControls()
{
  const bool bIsSQLOk = QSqlDatabase::database(POSTGRE_CONNECTION_NAME).isValid() &&
                        QSqlDatabase::database(POSTGRE_CONNECTION_NAME).isOpen();
  ui->actionSettings->setEnabled(bIsSQLOk && m_login.getUser().m_bSettings);
  ui->actionLogin->setEnabled(bIsSQLOk);
  ui->actionUsers->setEnabled(bIsSQLOk && m_login.getUser().m_bUser);
  ui->actionProducts->setEnabled(bIsSQLOk && m_login.getUser().m_bProduct);
  ui->actionCategories->setEnabled(bIsSQLOk && m_login.getUser().m_bCategory);
  ui->actionImages->setEnabled(bIsSQLOk && m_login.getUser().m_bImage);
  ui->actionShoppingListMgt->setEnabled(bIsSQLOk && m_login.getUser().m_bShoppingList);
  ui->actionEmployees->setEnabled(bIsSQLOk && m_login.getUser().m_bEmployee);
  ui->actionSuppliers->setEnabled(bIsSQLOk && m_login.getUser().m_bSupplier);
  ui->actionStores->setEnabled(bIsSQLOk && m_login.getUser().m_bStore);
  ui->actionTimeCard->setEnabled(bIsSQLOk && m_login.getUser().m_bTimeCard);

  ui->actionPurchases->setEnabled(bIsSQLOk && m_login.getUser().m_bPurchase);
  ui->actionReminders->setEnabled(bIsSQLOk && m_login.getUser().m_bReminder);
  ui->actionCalculator->setEnabled(bIsSQLOk && m_login.getUser().m_bCalculator);
  ui->actionShoppingList->setEnabled(bIsSQLOk && m_login.getUser().m_bShop);

  switch (getCurrentFunctionality())
  {
    case Functionality::Purchase:
    {
      Purchase o;
      m_purchase->getItem(o);
      ui->actionPrint->setEnabled(o.isValid());
    }  break;
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

void Goiabo::showInfo()
{

}

void Goiabo::openJItemSQLDialog()
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
    view = new UserView(m_login.getUser().m_id);

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

void Goiabo::openLoginDialog()
{
  hide();
  LoginDialog l(m_login);
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


void Goiabo::openActiveUsersDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  JDatabase* w = new JDatabase(ACTIVE_USERS_SQL_TABLE_NAME, JDatabase::Mode::ReadOnly);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Usuários Ativos"));
  dlg.setWindowIcon(QIcon(":/icons/res/users.png"));
  dlg.setModal(true);
  dlg.exec();
}

void Goiabo::closeEvent(QCloseEvent* event)
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

void Goiabo::activateWindow()
{
  m_purchaseWindow->hide();
  m_reminderWindow->hide();
  m_calculatorWindow->hide();
  m_shopWindow->hide();
  if (sender() == ui->actionPurchases)
  {
    m_purchaseWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_purchaseWindow);
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

void Goiabo::testTimeAccess()
{
  TimeCard c(this);
  c.exec();
}
