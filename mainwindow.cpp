#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "note/purchaseview.h"
#include "printutils.h"
#include "pincodeview.h"
#include "productview.h"
#include "categoryview.h"
#include "note/purchaseview.h"
#include "employeeview.h"
#include "supplierview.h"
#include "reminderview.h"
#include "calculatorwidget.h"
#include "userview.h"
#include "imageview.h"
#include "logindialog.h"
#include "shoppinglistview.h"
#include "reservationview.h"
#include "discountview.h"
#include "shopview.h"
#include "widgets/jdatabase.h"
#include "timecard.h"
#include "storeview.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>
#include <QLabel>
#include <QCloseEvent>
#include <QInputDialog>
#include <QMdiSubWindow>

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

Tipi::Tipi(const ActiveUser& login, QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Tipi)
  , m_login(login)
  , m_purchase(nullptr)
  , m_reminder(nullptr)
  , m_consumption(nullptr)
  , m_calculator(nullptr)
  , m_shop(nullptr)
  , m_reservation(nullptr)
  , m_discount(nullptr)
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
  , m_purchaseWindow(nullptr)
  , m_reminderWindow(nullptr)
  , m_calculatorWindow(nullptr)
  , m_shopWindow(nullptr)
  , m_reservationWindow(nullptr)
  , m_discountWindow(nullptr)
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
  m_reservation = new ReservationView;
  m_discount = new DiscountView;

  m_purchaseWindow = new JMdiSubWindow(this);
  m_purchaseWindow->setWindowTitle(tr("Vales"));
  m_purchaseWindow->setWindowIcon(QIcon(":/icons/res/note.png"));
  m_purchaseWindow->setWidget(m_purchase);
  m_mdi->addSubWindow(m_purchaseWindow);
  m_reminderWindow = new JMdiSubWindow(this);
  m_reminderWindow->setWindowTitle(tr("Lembretes"));
  m_reminderWindow->setWindowIcon(QIcon(":/icons/res/postit.png"));
  m_reminderWindow->setWidget(m_reminder);
  m_mdi->addSubWindow(m_reminderWindow);
  m_calculatorWindow = new JMdiSubWindow(this);
  m_calculatorWindow->setWindowTitle(tr("Calculadora"));
  m_calculatorWindow->setWindowIcon(QIcon(":/icons/res/calculator.png"));
  m_calculatorWindow->setWidget(m_calculator);
  m_mdi->addSubWindow(m_calculatorWindow);
  m_shopWindow = new JMdiSubWindow(this);
  m_shopWindow->setWindowTitle(tr("Compras"));
  m_shopWindow->setWindowIcon(QIcon(":/icons/res/shop.png"));
  m_shopWindow->setWidget(m_shop);
  m_mdi->addSubWindow(m_shopWindow);
  m_reservationWindow = new JMdiSubWindow(this);
  m_reservationWindow->setWindowTitle(tr("Reservas"));
  m_reservationWindow->setWindowIcon(QIcon(":/icons/res/reservation.png"));
  m_reservationWindow->setWidget(m_reservation);
  m_mdi->addSubWindow(m_reservationWindow);
  m_discountWindow = new JMdiSubWindow(this);
  m_discountWindow->setWindowTitle(tr("Descontos"));
  m_discountWindow->setWindowIcon(QIcon(":/icons/res/discount.png"));
  m_discountWindow->setWidget(m_discount);
  m_mdi->addSubWindow(m_discountWindow);

  m_statusDatabasePath = new QLabel();
  m_statusDatabasePath->setAlignment(Qt::AlignRight);
  m_statusDatabasePath->setTextFormat(Qt::RichText);
  m_statusUserName = new QLabel();
  m_statusUserName->setAlignment(Qt::AlignCenter);
  m_statusUserName->setTextFormat(Qt::RichText);
  statusBar()->addWidget(m_statusDatabasePath);
  statusBar()->addWidget(m_statusUserName);

  connect(ui->actionPrint, SIGNAL(triggered(bool)), this, SLOT(print()));
  connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettingsDialog()));
  connect(ui->actionInfo, SIGNAL(triggered(bool)), this, SLOT(showInfo()));
  connect(m_purchase, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_reminder, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_calculator, SIGNAL(lineSignal(const QString&)), this, SLOT(print(const QString&)));
  connect(ui->actionLogin, SIGNAL(triggered(bool)), this, SLOT(openLoginDialog()));
  connect(m_shop, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(ui->actionNotes, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReminders, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionCalculator, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionShoppingList, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReservations, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionDiscount, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(m_discount, SIGNAL(redeemSignal(const QString&)), this, SLOT(print(const QString&)));
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

  activateWindow();
  m_settings.load();
  updateControls();
  updateStatusBar();
}

Tipi::~Tipi()
{
  delete ui;
}

bool Tipi::connectPrinter()
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
    m_printerSerial.clear();
    if (bSuccess)
      bSuccess = Printer::printString(&m_printerSerial,
                                      m_settings.m_bIsPrinterEthernet,
                                      Printer::strCmdInit(),
                                      error);
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

void Tipi::disconnectPrinter()
{
  if (m_printerSerial.isOpen())
      m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();
}

Functionality Tipi::getCurrentFunctionality() const
{
  QMdiSubWindow* activeWindow = m_mdi->activeSubWindow();
  if (activeWindow == m_purchaseWindow)
    return Functionality::Note;
  else if (activeWindow == m_reminderWindow)
    return Functionality::Reminder;
  else if (activeWindow == m_calculatorWindow)
    return Functionality::Calculator;
  else if (activeWindow == m_shopWindow)
    return Functionality::Shop;
  else if (activeWindow == m_reservationWindow)
    return Functionality::Reservation;
  else if (activeWindow == m_discountWindow)
    return Functionality::Discount;
  return Functionality::None;
}

void Tipi::print()
{
  switch (getCurrentFunctionality())
  {
    case Functionality::Note:
    {
      Note o;
      m_purchase->getItem(o);
      if (o.m_date != QDate::currentDate() && !o.m_id.isValid())
      {
        int ret = QMessageBox::question(
                    this,
                    tr("Data"),
                    tr("A data informada é diferente da data de hoje.\nDeseja usar a data de hoje?"),
                    QMessageBox::Apply | QMessageBox::Ignore | QMessageBox::Cancel,
                    QMessageBox::Apply);
        switch (ret)
        {
          case QMessageBox::Apply:
            m_purchase->setDate(QDate::currentDate());
            break;
          case QMessageBox::Ignore:
            break;
          case QMessageBox::Cancel:
          default:
            return;
        }
      }
      o.clear(true);
      if (m_purchase->save(o.m_id))
      {
        QString error;
        if (o.SQL_select(error))
          print(NotePrinter::build(o));
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
    case Functionality::Reservation:
    {
      Reservation res = m_reservation->save();
      if (res.m_id.isValid())
        print(ReservationPrinter::build(res));
    } break;
    case Functionality::Discount:
    {
      Discount o = m_discount->save();
      if (o.m_id.isValid())
        print(DiscountPrinter::build(o));
    } break;
    case Functionality::None:
    default:
      break;
  }
}

bool Tipi::print(const QString& text)
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

void Tipi::openSettingsDialog()
{
  SettingsDlg dlg(m_settings);
  if (dlg.exec() == QDialog::Accepted)
  {
    m_settings = dlg.getSettings();
    m_settings.save();
  }
}

void Tipi::updateStatusBar()
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

void Tipi::updateControls()
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

  ui->actionNotes->setEnabled(bIsSQLOk && m_login.getUser().m_bNote);
  ui->actionReminders->setEnabled(bIsSQLOk && m_login.getUser().m_bReminder);
  ui->actionCalculator->setEnabled(bIsSQLOk && m_login.getUser().m_bCalculator);
  ui->actionShoppingList->setEnabled(bIsSQLOk && m_login.getUser().m_bShop);
  ui->actionReservations->setEnabled(bIsSQLOk && m_login.getUser().m_bReservation);
  ui->actionDiscount->setEnabled(bIsSQLOk && m_login.getUser().m_bDiscount);

  switch (getCurrentFunctionality())
  {
    case Functionality::Note:
    {
      Note o;
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
    case Functionality::Reservation:
      ui->actionPrint->setEnabled(m_reservation->getReservation().isValid());
      break;
    case Functionality::Discount:
      ui->actionPrint->setEnabled(m_discount->getDiscount().isValid() &&
                                  !m_discount->getDiscount().m_bRedeemed);
      break;
    default:
      break;
  }
}

void Tipi::showInfo()
{

}

void Tipi::openJItemSQLDialog()
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

void Tipi::openLoginDialog()
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


void Tipi::openActiveUsersDialog()
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

void Tipi::closeEvent(QCloseEvent* event)
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

void Tipi::activateWindow()
{
  m_purchaseWindow->hide();
  m_reminderWindow->hide();
  m_calculatorWindow->hide();
  m_shopWindow->hide();
  m_reservationWindow->hide();
  m_discountWindow->hide();
  if (sender() == ui->actionNotes)
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
  else if (sender() == ui->actionReservations)
  {
    m_reservationWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_reservationWindow);
  }
  else if (sender() == ui->actionDiscount)
  {
    m_discountWindow->showMaximized();
    m_mdi->setActiveSubWindow(m_discountWindow);
  }
  updateControls();
}

void Tipi::testTimeAccess()
{
  TimeCard c(this);
  c.exec();
}
