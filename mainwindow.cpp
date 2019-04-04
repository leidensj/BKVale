#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "noteview.h"
#include "printutils.h"
#include "pincodeview.h"
#include "productview.h"
#include "categoryview.h"
#include "noteview.h"
#include "employeeview.h"
#include "reminderview.h"
#include "calculatorwidget.h"
#include "usermgtview.h"
#include "imageview.h"
#include "logindialog.h"
#include "formview.h"
#include "shoppinglistview.h"
#include "reservationview.h"
#include "discountview.h"
#include "shopview.h"
#include "jdatabase.h"
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

BaitaAssistant::BaitaAssistant(const UserLoginSQL& userLogin, QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::BaitaAssistant)
  , m_userLogin(userLogin)
  , m_note(nullptr)
  , m_reminder(nullptr)
  , m_consumption(nullptr)
  , m_calculator(nullptr)
  , m_shop(nullptr)
  , m_reservation(nullptr)
  , m_discount(nullptr)
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
  , m_noteWindow(nullptr)
  , m_reminderWindow(nullptr)
  , m_calculatorWindow(nullptr)
  , m_shopWindow(nullptr)
  , m_reservationWindow(nullptr)
  , m_discountWindow(nullptr)
{
  ui->setupUi(this);

  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());

  m_note = new NoteView;
  m_reminder = new ReminderView;
  m_calculator = new CalculatorWidget;
  m_shop = new ShopView;
  m_reservation = new ReservationView;
  m_discount = new DiscountView;

  m_noteWindow = new JMdiSubWindow(this);
  m_noteWindow->setWindowTitle(tr("Vales"));
  m_noteWindow->setWindowIcon(QIcon(":/icons/res/note.png"));
  m_noteWindow->setWidget(m_note);
  ui->mdi->addSubWindow(m_noteWindow);
  m_reminderWindow = new JMdiSubWindow(this);
  m_reminderWindow->setWindowTitle(tr("Lembretes"));
  m_reminderWindow->setWindowIcon(QIcon(":/icons/res/postit.png"));
  m_reminderWindow->setWidget(m_reminder);
  ui->mdi->addSubWindow(m_reminderWindow);
  m_calculatorWindow = new JMdiSubWindow(this);
  m_calculatorWindow->setWindowTitle(tr("Calculadora"));
  m_calculatorWindow->setWindowIcon(QIcon(":/icons/res/calculator.png"));
  m_calculatorWindow->setWidget(m_calculator);
  ui->mdi->addSubWindow(m_calculatorWindow);
  m_shopWindow = new JMdiSubWindow(this);
  m_shopWindow->setWindowTitle(tr("Compras"));
  m_shopWindow->setWindowIcon(QIcon(":/icons/res/shop.png"));
  m_shopWindow->setWidget(m_shop);
  ui->mdi->addSubWindow(m_shopWindow);
  m_reservationWindow = new JMdiSubWindow(this);
  m_reservationWindow->setWindowTitle(tr("Reservas"));
  m_reservationWindow->setWindowIcon(QIcon(":/icons/res/reservation.png"));
  m_reservationWindow->setWidget(m_reservation);
  ui->mdi->addSubWindow(m_reservationWindow);
  m_discountWindow = new JMdiSubWindow(this);
  m_discountWindow->setWindowTitle(tr("Descontos"));
  m_discountWindow->setWindowIcon(QIcon(":/icons/res/discount.png"));
  m_discountWindow->setWidget(m_discount);
  ui->mdi->addSubWindow(m_discountWindow);

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
  connect(m_note, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_reminder, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(ui->actionProducts, SIGNAL(triggered(bool)), this, SLOT(openProductsDialog()));
  connect(ui->actionCategories, SIGNAL(triggered(bool)), this, SLOT(openCategoriesDialog()));
  connect(ui->actionUsers, SIGNAL(triggered(bool)), this, SLOT(openUsersDialog()));
  connect(ui->actionImages, SIGNAL(triggered(bool)), this, SLOT(openImagesDialog()));
  connect(m_calculator, SIGNAL(lineSignal(const QString&)), this, SLOT(print(const QString&)));
  connect(ui->actionLogin, SIGNAL(triggered(bool)), this, SLOT(openLoginDialog()));
  connect(ui->actionForms, SIGNAL(triggered(bool)), this, SLOT(openFormsDialog()));
  connect(ui->actionShoppingList, SIGNAL(triggered(bool)), this, SLOT(openShoppingListDialog()));
  connect(m_shop, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(ui->actionActiveUsers, SIGNAL(triggered(bool)), this, SLOT(openActiveUsersDialog()));
  connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(ui->actionNotes, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReminders, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionCalculator, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionShop, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionReservations, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(ui->actionDiscount, SIGNAL(triggered(bool)), this, SLOT(activateWindow()));
  connect(m_discount, SIGNAL(redeemSignal(const QString&)), this, SLOT(print(const QString&)));
  connect(ui->actionTimeCard, SIGNAL(triggered(bool)), this, SLOT(testTimeAccess()));
  connect(ui->actionStores, SIGNAL(triggered(bool)), this, SLOT(openStoreDialog()));

  activateWindow();
  m_settings.load();
  updateControls();
  updateStatusBar();
}

BaitaAssistant::~BaitaAssistant()
{
  QString error;
  ActiveUserSQL::refresh(error);
  delete ui;
}

bool BaitaAssistant::connectPrinter()
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

void BaitaAssistant::disconnectPrinter()
{
  if (m_printerSerial.isOpen())
      m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();
}

Functionality BaitaAssistant::getCurrentFunctionality() const
{
  QMdiSubWindow* activeWindow = ui->mdi->activeSubWindow();
  if (activeWindow == m_noteWindow)
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

void BaitaAssistant::print()
{
  switch (getCurrentFunctionality())
  {
    case Functionality::Note:
    {
      Note note = m_note->getNote();
      if (note.m_date != QDate::currentDate() && !note.m_id.isValid())
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
            note.m_date = QDate::currentDate();
            m_note->setNote(note);
            break;
          case QMessageBox::Ignore:
            break;
          case QMessageBox::Cancel:
          default:
            return;
        }
      }
      Employee e;
      note = m_note->save(e);
      if (note.m_id.isValid())
        print(NotePrinter::build(note, e.m_form.strAliasName()));
    } break;
    case Functionality::Reminder:
    {
      ReminderPrintDialog dlg(this);
      if (dlg.exec())
      {
        Reminder r = m_reminder->getReminder();
        QString str = ReminderPrinter::build(r);
        bool bSuccess = false;
        for (int i = 0; i != dlg.getCopies(); ++i)
        {
          bSuccess = print(str);
          if (!bSuccess)
            break;
        }

        if (bSuccess && dlg.getSave())
          bSuccess = m_reminder->save();

        if (bSuccess)
          m_reminder->create();
      }
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

bool BaitaAssistant::print(const QString& text)
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

void BaitaAssistant::openSettingsDialog()
{
  SettingsDlg dlg(m_settings);
  if (dlg.exec() == QDialog::Accepted)
  {
    m_settings = dlg.getSettings();
    m_settings.save();
  }
}

void BaitaAssistant::updateStatusBar()
{
  // para adicionar um ícone:
  // "<img src=':/icons/res/16user.png'> " + ...

  m_statusUserName->setText(tr("Usuário: ") + m_userLogin.strUser());
  m_statusDatabasePath->setText(tr("Banco de dados: ") +
                                (m_settings.m_databaseHostName.isEmpty() ? "localhost"
                                                                        : m_settings.m_databaseHostName) +
                                ":" +
                                QString::number(m_settings.m_databasePort));
}

void BaitaAssistant::updateControls()
{
  const bool bIsSQLOk = QSqlDatabase::database(POSTGRE_CONNECTION_NAME).isValid() &&
                        QSqlDatabase::database(POSTGRE_CONNECTION_NAME).isOpen();
  ui->actionSettings->setEnabled(bIsSQLOk && m_userLogin.hasAccessToSettings());
  ui->actionLogin->setEnabled(bIsSQLOk);
  ui->actionUsers->setEnabled(bIsSQLOk && m_userLogin.hasAccessToUsers());
  ui->actionProducts->setEnabled(bIsSQLOk && m_userLogin.hasAccessToProducts());
  ui->actionForms->setEnabled(bIsSQLOk && m_userLogin.hasAccessToForms());
  ui->actionCategories->setEnabled(bIsSQLOk && m_userLogin.hasAccessToCategories());
  ui->actionImages->setEnabled(bIsSQLOk && m_userLogin.hasAccessToImages());
  ui->actionShoppingList->setEnabled(bIsSQLOk && m_userLogin.hasAccessToShoppingLists());
  ui->actionEmployees->setEnabled(bIsSQLOk && m_userLogin.hasAccessToEmployee());
  ui->actionSuppliers->setEnabled(bIsSQLOk && m_userLogin.hasAccessToSupplier());

  ui->actionNotes->setEnabled(bIsSQLOk && m_userLogin.hasAccessToNote());
  ui->actionReminders->setEnabled(bIsSQLOk && m_userLogin.hasAccessToReminder());
  ui->actionCalculator->setEnabled(bIsSQLOk && m_userLogin.hasAccessToCalculator());
  ui->actionShop->setEnabled(bIsSQLOk && m_userLogin.hasAccessToShop());
  ui->actionReservations->setEnabled(bIsSQLOk && m_userLogin.hasAccessToReservations());

  switch (getCurrentFunctionality())
  {
    case Functionality::Note:
      ui->actionPrint->setEnabled(m_note->getNote().isValid());
      break;
    case Functionality::Reminder:
      ui->actionPrint->setEnabled(m_reminder->getReminder().isValid());
      break;
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

void BaitaAssistant::showInfo()
{

}

void BaitaAssistant::openProductsDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  ProductView* w = new ProductView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Produtos"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openCategoriesDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  CategoryView* w = new CategoryView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Categorias"));
  dlg.setWindowIcon(QIcon(":/icons/res/category.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openUsersDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  UserMgtView* w = new UserMgtView(m_userLogin.getId(), this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Usuários"));
  dlg.setWindowIcon(QIcon(":/icons/res/user.png"));
  dlg.setModal(true);
  dlg.exec();

  if (w->hasLoggedUserChanged())
    openLoginDialog();
}

void BaitaAssistant::openImagesDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  ImageView* w = new ImageView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Imagens"));
  dlg.setWindowIcon(QIcon(":/icons/res/icon.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openLoginDialog()
{
  hide();
  LoginDialog l(m_userLogin);
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

void BaitaAssistant::openFormsDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  FormView* w = new FormView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Cadastros"));
  dlg.setWindowIcon(QIcon(":/icons/res/resume.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openEmployeesDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  EmployeeView* w = new EmployeeView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Funcionários"));
  dlg.setWindowIcon(QIcon(":/icons/res/employee.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openShoppingListDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  ShoppingListView* w = new ShoppingListView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Listas de Compras"));
  dlg.setWindowIcon(QIcon(":/icons/res/shopmgt.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openActiveUsersDialog()
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

void BaitaAssistant::openStoreDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  StoreView* w = new StoreView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Lojas"));
  dlg.setWindowIcon(QIcon(":/icons/res/store.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::closeEvent(QCloseEvent* event)
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

void BaitaAssistant::activateWindow()
{
  m_noteWindow->hide();
  m_reminderWindow->hide();
  m_calculatorWindow->hide();
  m_shopWindow->hide();
  m_reservationWindow->hide();
  m_discountWindow->hide();
  if (sender() == ui->actionNotes)
  {
    m_noteWindow->showMaximized();
    ui->mdi->setActiveSubWindow(m_noteWindow);
  }
  else if (sender() == ui->actionReminders)
  {
    m_reminderWindow->showMaximized();
    ui->mdi->setActiveSubWindow(m_reminderWindow);
  }
  else if (sender() == ui->actionCalculator)
  {
    m_calculatorWindow->showMaximized();
    ui->mdi->setActiveSubWindow(m_calculatorWindow);
  }
  else if (sender() == ui->actionShop)
  {
    m_shopWindow->showMaximized();
    ui->mdi->setActiveSubWindow(m_shopWindow);
  }
  else if (sender() == ui->actionReservations)
  {
    m_reservationWindow->showMaximized();
    ui->mdi->setActiveSubWindow(m_reservationWindow);
  }
  else if (sender() == ui->actionDiscount)
  {
    m_discountWindow->showMaximized();
    ui->mdi->setActiveSubWindow(m_discountWindow);
  }
  updateControls();
}

void BaitaAssistant::testTimeAccess()
{
  TimeCard c(this);
  c.exec();
}
