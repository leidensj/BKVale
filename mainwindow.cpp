#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "noteview.h"
#include "printutils.h"
#include "pincodeview.h"
#include "productview.h"
#include "categoryview.h"
#include "noteview.h"
#include "reminderview.h"
#include "calculatorwidget.h"
#include "usermgtview.h"
#include "imageview.h"
#include "logindialog.h"
#include "personview.h"
#include "shoppinglistview.h"
#include "reservationview.h"
#include "shopview.h"
#include "jdatabase.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>
#include <QLabel>

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
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
{
  ui->setupUi(this);
  m_note = new NoteView;
  m_reminder = new ReminderView;
  m_calculator = new CalculatorWidget;
  m_shop = new ShopView;
  m_reservation = new ReservationView;
  ui->tabNotes->layout()->addWidget(m_note);
  ui->tabReminder->layout()->addWidget(m_reminder);
  ui->tabCalculator->layout()->addWidget(m_calculator);
  ui->tabShop->layout()->addWidget(m_shop);
  ui->tabReservation->layout()->addWidget(m_reservation);

  m_statusDatabasePath = new QLabel();
  m_statusDatabasePath->setAlignment(Qt::AlignRight);
  m_statusDatabasePath->setTextFormat(Qt::RichText);
  m_statusUserName = new QLabel();
  m_statusUserName->setAlignment(Qt::AlignCenter);
  m_statusUserName->setTextFormat(Qt::RichText);
  statusBar()->addWidget(m_statusDatabasePath);
  statusBar()->addWidget(m_statusUserName);


  QObject::connect(ui->actionPrint,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(print()));

  QObject::connect(ui->actionSettings,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openSettingsDialog()));

  QObject::connect(ui->actionInfo,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showInfo()));

  QObject::connect(m_note,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  QObject::connect(ui->tabWidget,
                   SIGNAL(currentChanged(int)),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_reminder,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  QObject::connect(ui->actionProducts,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openProductsDialog()));

  QObject::connect(ui->actionCategories,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openCategoriesDialog()));

  QObject::connect(ui->actionUsers,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openUsersDialog()));

  QObject::connect(ui->actionImages,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openImagesDialog()));

  QObject::connect(m_calculator,
                   SIGNAL(lineSignal(const QString&)),
                   this,
                   SLOT(print(const QString&)));

  QObject::connect(ui->actionLogin,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openLoginDialog()));

  QObject::connect(ui->actionPersons,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openPersonsDialog()));

  QObject::connect(ui->actionShoppingList,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openShoppingListDialog()));

  QObject::connect(m_shop,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  QObject::connect(ui->actionReconnectDatabase,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(reconnectDatabase()));

  QObject::connect(ui->actionActiveUsers,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openActiveUsersDialog()));

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

void BaitaAssistant::print()
{
  switch ((Functionality)ui->tabWidget->currentIndex())
  {
    case Functionality::NoteMode:
    {
      QString name;
      if (m_settings.m_notesPincodeRequired)
      {
        PinCodeView w(this);
        if (!w.exec() || !w.getCurrentPerson().isValidId())
          return;
        name = w.getCurrentPerson().strAliasName();
      }
      Note note = m_note->save();
      if (note.isValidId())
        print(NotePrinter::build(note, name));
    } break;
    case Functionality::ReminderMode:
    {
      Reminder r;
      if (m_reminder->isSave())
      {
        r = m_reminder->save();
        if (r.isValidId())
          print(ReminderPrinter::build(r));
      }
      else
      {
        r = m_reminder->getReminder();
        if (print(ReminderPrinter::build(r)))
          m_reminder->create();
      }
    } break;
    case Functionality::ConsumptionMode:
      break;
    case Functionality::CalculatorMode:
      print(m_calculator->getFullContent() + Printer::strCmdFullCut());
      break;
    case Functionality::ShopMode:
      print(ShoppingListPrinter::build(m_shop->getShoppingList()));
      break;
    case Functionality::ReservationMode:
    {
      Reservation res = m_reservation->save();
      if (res.isValidId())
        print(ReservationPrinter::build(res));
    } break;
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
  ui->actionPersons->setEnabled(bIsSQLOk && m_userLogin.hasAccessToPersons());
  ui->actionCategories->setEnabled(bIsSQLOk && m_userLogin.hasAccessToCategories());
  ui->actionImages->setEnabled(bIsSQLOk && m_userLogin.hasAccessToImages());
  ui->actionShoppingList->setEnabled(bIsSQLOk && m_userLogin.hasAccessToShoppingLists());

  ui->tabWidget->setTabEnabled((int)Functionality::NoteMode,
                               bIsSQLOk && m_userLogin.hasAccessToNote());
  ui->tabWidget->setTabEnabled((int)Functionality::ReminderMode,
                               bIsSQLOk && m_userLogin.hasAccessToReminder());
  ui->tabWidget->setTabEnabled((int)Functionality::CalculatorMode,
                               bIsSQLOk && m_userLogin.hasAccessToCalculator());
  ui->tabWidget->setTabEnabled((int)Functionality::ShopMode,
                               bIsSQLOk && m_userLogin.hasAccessToShop());
  ui->tabWidget->setTabEnabled((int)Functionality::ConsumptionMode,
                               bIsSQLOk && m_userLogin.hasAccessToConsumption());
  ui->tabWidget->setTabEnabled((int)Functionality::ReservationMode,
                               bIsSQLOk && m_userLogin.hasAccessToReservations());

  switch ((Functionality)ui->tabWidget->currentIndex())
  {
    case Functionality::NoteMode:
      ui->actionPrint->setEnabled(m_note->getNote().isValid());
      break;
    case Functionality::ReminderMode:
      ui->actionPrint->setEnabled(m_reminder->getReminder().isValid());
      break;
      case Functionality::CalculatorMode:
      ui->actionPrint->setEnabled(true);
      break;
    case Functionality::ConsumptionMode:
      break;
    case Functionality::ShopMode:
      ui->actionPrint->setEnabled(m_shop->getShoppingList().isValidId());
      break;
    case Functionality::ReservationMode:
      ui->actionPrint->setEnabled(m_reservation->getReservation().isValid());
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

void BaitaAssistant::openPersonsDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  PersonView* w = new PersonView(this);
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Pessoas"));
  dlg.setWindowIcon(QIcon(":/icons/res/person.png"));
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

void BaitaAssistant::reconnectDatabase()
{
  QString error;
  if (!BaitaSQL::init(m_settings.m_databaseHostName,
                      m_settings.m_databasePort,
                      error))
  {
    QMessageBox::critical(this,
                          tr("Erro ao reconectar"),
                          tr("O seguinte erro ocorreu ao reconectar ao banco de dados : '%1'").arg(error),
                          QMessageBox::Ok);
  }
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
