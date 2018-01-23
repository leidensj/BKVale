#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "noteview.h"
#include "printutils.h"
#include "itemwidget.h"
#include "notewidget.h"
#include "reminderwidget.h"
#include "consumptionwidget.h"
#include "calculatorwidget.h"
#include "usermgtwidget.h"
#include "logindialog.h"
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
  , m_statusFilePath(nullptr)
  , m_statusUserName(nullptr)
{
  ui->setupUi(this);
  m_note = new NoteWidget();
  m_reminder = new ReminderWidget();
  m_consumption = new ConsumptionWidget();
  m_calculator = new CalculatorWidget();
  ui->tabNotes->layout()->addWidget(m_note);
  ui->tabReminder->layout()->addWidget(m_reminder);
  ui->tabConsumption->layout()->addWidget(m_consumption);
  ui->tabCalculator->layout()->addWidget(m_calculator);

  m_statusFilePath = new QLabel();
  m_statusFilePath->setAlignment(Qt::AlignRight);
  m_statusUserName = new QLabel();
  m_statusUserName->setAlignment(Qt::AlignCenter);
  statusBar()->addWidget(m_statusFilePath);
  statusBar()->addWidget(m_statusUserName);

  QObject::connect(ui->actionConnect,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(connect()));

  QObject::connect(ui->actionDisconnect,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(disconnect()));

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

  QObject::connect(ui->actionItems,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openItemsDialog()));

  QObject::connect(ui->actionUsers,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openUsersDialog()));

  QObject::connect(m_calculator,
                   SIGNAL(printSignal(const QString&)),
                   this,
                   SLOT(print(const QString&)));

  QObject::connect(m_calculator,
                   SIGNAL(printFullCutSignal()),
                   this,
                   SLOT(printFullCut()));

  QObject::connect(&m_printerTCP,
                   SIGNAL(connected()),
                   this,
                   SLOT(connectedTCP()));

  QObject::connect(ui->actionLogin,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openLoginDialog()));

  m_settings.load();
  if (!m_settings.m_serialPort.isEmpty())
    connect();
  m_note->setDatabase(m_userLogin.getDatabase());
  m_note->create();
  m_consumption->setDatabase(m_userLogin.getDatabase());
  m_reminder->setDatabase(m_userLogin.getDatabase());
  updateControls();
  updateStatusBar();
}

BaitaAssistant::~BaitaAssistant()
{
  delete ui;
}

void BaitaAssistant::connect()
{
  switch (m_settings.m_interfaceType)
  {
    case InterfaceType::Serial:
      connectSerial();
      break;
    case InterfaceType::Ethernet:
      connectedTCP();
      break;
  }
}

void BaitaAssistant::connectSerial()
{
  if (m_printer.isOpen())
  {
    if (m_printer.portName() != m_settings.m_serialPort)
    {
      m_printer.close();
    }
    else
    {
      QMessageBox msgBox(QMessageBox::Information,
                         tr("Aviso"),
                         tr("A impressora já está conectada."),
                         QMessageBox::Ok);
      msgBox.exec();
      return;
    }
  }

  if (!m_settings.m_serialPort.isEmpty())
  {
    m_printer.setPortName(m_settings.m_serialPort);
    if (!m_printer.open((QIODevice::ReadWrite)))
    {
      QMessageBox msgBox(QMessageBox::Critical,
                         tr("Erro"),
                         tr("O seguinte erro ocorreu ('%1') ao conectar à impressora "
                            "(porta '%2'): '%3'.").arg(
                           QString::number(m_printer.error()),
                           m_printer.portName(),
                           m_printer.errorString()),
                         QMessageBox::Ok);
      msgBox.exec();
    }
    else
    {
      QString error;
      if (!Printer::init(m_printer, error))
      {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("Aviso") + QString::number(m_printer.error()),
                           tr("Erro ao inicializar a impressora: '%1'.").arg(error),
                           QMessageBox::Ok);
        msgBox.exec();
        m_printer.close();
      }
    }
  }
  else
  {
    QMessageBox msgBox(QMessageBox::Warning,
                       tr("Atenção"),
                       tr("É necessário selecionar uma porta para se conectar à impressora."),
                       QMessageBox::Ok);
    msgBox.exec();
  }

  updateControls();
}

void BaitaAssistant::connectTCP()
{
  if (m_printerTCP.isOpen())
  {
    else
    {
      QMessageBox msgBox(QMessageBox::Information,
                         tr("Aviso"),
                         tr("A impressora já está conectada."),
                         QMessageBox::Ok);
      msgBox.exec();
      return;
    }
  }

  if (!m_settings.m_ethernetIP.isEmpty())
  {
    m_printerTCP.connectToHost(m_settings.m_ethernetIP,
                               (quint16)m_settings.m_ethernetPort);
    if (!m_printerTCP.waitForConnected())
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("O seguinte erro ocorreu ao conectar à impressora "
                               "(IP '%2', Porta: '%3'): '%4'.").arg(
                              m_settings.m_ethernetIP,
                              QString::number(m_settings.m_ethernetPort),
                              m_printerTCP.errorString()),
                            QMessageBox::Ok);
    }
    else
    {
      QString error;
      if (!Printer::init(m_printerTCP, error))
      {
        QMessageBox::warning(this,
                             tr("Aviso"),
                             tr("Erro ao inicializar a impressora: '%1'.").arg(error),
                             QMessageBox::Ok);
        m_printerTCP.close();
      }
    }
  }
  else
  {
    QMessageBox msgBox(QMessageBox::Warning,
                       tr("Atenção"),
                       tr("É necessário selecionar um IP para se conectar à impressora."),
                       QMessageBox::Ok);
    msgBox.exec();
  }

  updateControls();
}

void BaitaAssistant::connectedTCP()
{
  setWindowTitle("CONECTOU");
}

void BaitaAssistant::disconnect()
{
  if (m_printer.isOpen())
      m_printer.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();
  updateControls();
}

void BaitaAssistant::print()
{
  switch ((Functionality)ui->tabWidget->currentIndex())
  {
    case Functionality::NoteMode:
    {
      m_note->saveAndPrint(m_printer, m_userLogin.strUser());
    } break;
    case Functionality::ReminderMode:
    {
      m_reminder->print(m_printer);
      m_reminder->save();
      m_reminder->clear();
    } break;
    case Functionality::ConsumptionMode:
    {
      print(m_consumption->printContent());
    } break;
    case Functionality::CalculatorMode:
    {
      print(m_calculator->text());
    } break;
    case Functionality::ShopMode:
      break;
  }
}

bool BaitaAssistant::print(const QString& text)
{
  bool bSuccess = m_printer.isOpen();
  if (bSuccess)
  {
    QString error;
    bSuccess = Printer::print(m_printer, text, error);
    if (!bSuccess)
    {
      QMessageBox msgBox(QMessageBox::Critical,
                         tr("Erro ao imprimir"),
                         tr("Erro '%1' ao imprimir.").arg(error),
                         QMessageBox::Ok);
      msgBox.exec();
    }
  }
  else
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro ao imprimir"),
                       "O sistema não conseguiu conectar-se à impressora.",
                       QMessageBox::Ok);
    msgBox.exec();
  }
  return bSuccess;
}

void BaitaAssistant::printFullCut()
{
  Printer::fullCut(m_printer);
}

void BaitaAssistant::openSettingsDialog()
{
  if (!m_printer.isOpen())
  {
    SettingsDlg dlg(m_settings);
    if (dlg.exec() == QDialog::Accepted)
    {
      m_settings = dlg.getSettings();
      m_settings.save();
    }
  }
  else
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro"),
                       tr("Desconecte a impressora primeiro."),
                       QMessageBox::Ok);
    msgBox.exec();
  }
}

void BaitaAssistant::updateStatusBar()
{
  m_statusUserName->setText(" Usuário: " + m_userLogin.strUser() + " ");
  m_statusFilePath->setText(" Arquivo: " + m_settings.filePath() + " ");
}

void BaitaAssistant::updateControls()
{
  const bool bIsSQLOk = m_userLogin.getDatabase().isOpen();
  const bool bIsOpen = m_printer.isOpen();
  ui->actionConnect->setEnabled(!bIsOpen && bIsSQLOk);
  ui->actionDisconnect->setEnabled(bIsOpen && bIsSQLOk);
  ui->actionSettings->setEnabled(bIsSQLOk && !bIsOpen && m_userLogin.hasAccessToSettings());
  ui->actionLogin->setEnabled(bIsSQLOk);
  ui->actionUsers->setEnabled(bIsSQLOk && m_userLogin.hasAccessToUsers());
  ui->actionItems->setEnabled(bIsSQLOk && m_userLogin.hasAccessToItems());

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

  switch ((Functionality)ui->tabWidget->currentIndex())
  {
    case Functionality::NoteMode:
    {
      ui->actionPrint->setEnabled(m_note->isValid() && bIsOpen);
    } break;
    case Functionality::ReminderMode:
    {
      ui->actionPrint->setEnabled(m_reminder->isValid() && bIsOpen);
    } break;
      case Functionality::CalculatorMode:
    {
      ui->actionPrint->setEnabled(bIsOpen);
    } break;
    case Functionality::ConsumptionMode:
    {
      ui->actionPrint->setEnabled(m_consumption->isValid() && bIsOpen);
    } break;
    case Functionality::ShopMode:
    default:
      break;
  }
}

void BaitaAssistant::showInfo()
{

}

void BaitaAssistant::openItemsDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ItemWidget* itemWidget = new ItemWidget(true);
  itemWidget->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(itemWidget);
  dlg.resize(640, 480);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Produtos"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  itemWidget->focusFilter();
  dlg.exec();
}

void BaitaAssistant::openUsersDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  UserMgtWidget* w = new UserMgtWidget(m_userLogin.getId());
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Usuários"));
  dlg.setWindowIcon(QIcon(":/icons/res/user.png"));
  dlg.setModal(true);
  dlg.exec();

  if (w->hasAnyUserChanged())
    openLoginDialog();
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
