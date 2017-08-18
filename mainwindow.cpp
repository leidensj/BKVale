#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notewidget.h"
#include "printutils.h"
#include "itemwidget.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>

BaitaAssistant::BaitaAssistant(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaitaAssistant),
    m_bReady(false),
    m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
  ui->setupUi(this);
  ui->tabNotes->layout()->addWidget(&m_noteWidget);
  ui->tabReminder->layout()->addWidget(&m_reminder);
  ui->tabConsumption->layout()->addWidget(&m_consumptionDatabaseWidget);

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
                   SLOT(notePrint()));

  QObject::connect(ui->actionSettings,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showSettings()));

  QObject::connect(ui->actionInfo,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showInfo()));

  QObject::connect(&m_noteWidget,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(enableControls()));

  QObject::connect(this,
                   SIGNAL(initSignal()),
                   this,
                   SLOT(init()));

  QObject::connect(ui->tabWidget,
                   SIGNAL(currentChanged(int)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(&m_reminder,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(enableControls()));

  QObject::connect(ui->actionItems,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openItemsDialog()));

  m_noteWidget.clear();
  emit initSignal();
}

BaitaAssistant::~BaitaAssistant()
{
  delete ui;
}

void BaitaAssistant::connect()
{
  if (m_printer.isOpen())
  {
    if (m_printer.portName() != m_settings.port)
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

  if (!m_settings.port.isEmpty())
  {
    m_printer.setPortName(m_settings.port);
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

  enableControls();
}

void BaitaAssistant::disconnect()
{
  if (m_printer.isOpen())
      m_printer.close();
  enableControls();
}

void BaitaAssistant::notePrint()
{
  if (ui->tabWidget->currentIndex() == (int)Functionality::NoteMode)
  {
    Note note = m_noteWidget.getNote();
    QString str(NotePrinter::build(note));
    QString error;
    if (!Printer::print(m_printer, str, error))
    {
      QMessageBox msgBox(QMessageBox::Critical,
                         tr("Erro"),
                         error,
                         QMessageBox::Ok);
      msgBox.exec();
    }
    else
    {
      QString error;
      if (!m_noteWidget.save(error))
      {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("Erro ao salvar vale"),
                           error,
                           QMessageBox::Ok);
        msgBox.exec();
      }
      else
      {
        m_noteWidget.create();
      }
    }
  }
  else if (ui->tabWidget->currentIndex() == (int)Functionality::ReminderMode)
  {
    QString str(ReminderPrinter::build(m_reminder.reminder()));
    QString error;
    Printer::print(m_printer, str, error);
  }
}

void BaitaAssistant::showSettings()
{
  if (!m_printer.isOpen())
  {
    SettingsDlg dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
      m_settings = dlg.getSettings();
      QString error;
      if (!BaitaDatabase::insertSettings(m_db,
                                         m_settings,
                                         error))
      {
        QMessageBox msgBox(QMessageBox::Critical,
                           tr("Erro ao salvar a configuração"),
                           error,
                           QMessageBox::Ok);
        msgBox.exec();
      }
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

void BaitaAssistant::enableControls()
{
  const bool bIsOpen = m_printer.isOpen();
  ui->actionConnect->setEnabled(!bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionSettings->setEnabled(!bIsOpen);

  Functionality func = (Functionality)ui->tabWidget->currentIndex();
  switch (func)
  {
    case Functionality::NoteMode:
    {
      ui->actionPrint->setEnabled(m_noteWidget.isValid() && bIsOpen && m_bReady);
    } break;
    case Functionality::ReminderMode:
    {
      ui->actionPrint->setEnabled(m_reminder.isValid() && bIsOpen && m_bReady);
    } break;
    case Functionality::ShopMode:
    default:
      break;
  }
}

void BaitaAssistant::init()
{
  QString error;
  bool bSuccess = BaitaDatabase::open(m_db,
                                      qApp->applicationDirPath() +
                                      QDir::separator() +
                                      "setup.db",
                                      error);
  if (bSuccess)
    bSuccess = BaitaDatabase::init(m_db, error);
  else
    m_db.close();

  if (!bSuccess)
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro ao inicializar banco de dados"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
  else
  {
    m_bReady = true;
    BaitaDatabase::selectSettings(m_db, m_settings);
    if (!m_settings.port.isEmpty())
      connect();
    m_noteWidget.setHistoryDatabase(m_db);
    m_consumptionDatabaseWidget.setDatabase(m_db);
    m_noteWidget.create();
  }
  enableControls();
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
  itemWidget->setDatabase(m_db);
  layout->addWidget(itemWidget);
  dlg.resize(width(), height());
  dlg.setWindowTitle(tr("Gerenciar Produtos"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  itemWidget->focusFilter();
  dlg.exec();
}
