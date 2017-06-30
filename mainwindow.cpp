#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notewidget.h"
#include "printutils.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>

BaitaAssistant::BaitaAssistant(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaitaAssistant),
    m_bReady(false)
{
  ui->setupUi(this);
  ui->tabNotes->layout()->addWidget(&m_noteWidget);

  QObject::connect(ui->actionNoteAdd,
                   SIGNAL(triggered(bool)),
                   &m_noteWidget,
                   SLOT(addItem()));

  QObject::connect(ui->actionNoteRemove,
                   SIGNAL(triggered(bool)),
                   &m_noteWidget,
                   SLOT(removeItem()));

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

  QObject::connect(ui->actionNoteNew,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(noteCreate()));

  QObject::connect(ui->actionNoteSearch,
                   SIGNAL(triggered(bool)),
                   &m_noteWidget,
                   SLOT(showHistory()));

  QObject::connect(this,
                   SIGNAL(initSignal()),
                   this,
                   SLOT(init()));

  QObject::connect(ui->tabWidget,
                   SIGNAL(currentChanged(int)),
                   this,
                   SLOT(enableControls()));

  m_noteWidget.clear();
  emit initSignal();
  enableControls();
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
      if (!PrintUtils::initPrinter(m_printer, error))
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
  Note note = m_noteWidget.getNote();
  QString str(PrintUtils::buildNote(note));
  QString error;
  if (!PrintUtils::print(m_printer, str, error))
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
  else if (!m_noteWidget.isHistoryMode())
  {
    if (!m_db.insert(note,
                     error))
    {
      QMessageBox msgBox(QMessageBox::Warning,
                         tr("Erro ao salvar vale"),
                         error,
                         QMessageBox::Ok);
      msgBox.exec();
    }
    else
    {
      m_db.insertDescriptions(m_noteWidget.getItemDescriptions());
      noteCreate();
    }
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
      if (!m_db.insertSettings(m_settings,
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
    case Functionality::FNotes:
    {
      ui->actionPrint->setEnabled(m_noteWidget.isValid() && bIsOpen && m_bReady);
      ui->actionNoteRemove->setEnabled(m_noteWidget.isItemSelected());
      ui->toolNotes->setHidden(false);
      ui->toolNotes->setEnabled(true);
      ui->toolPostits->setHidden(true);
      ui->toolPostits->setEnabled(false);
    } break;
    case Functionality::FPostits:
    {
      ui->toolNotes->setHidden(true);
      ui->toolNotes->setEnabled(false);
      ui->toolPostits->setHidden(false);
      ui->toolPostits->setEnabled(true);
    } break;
    case Functionality::FShop:
    default:
      break;
  }
}

void BaitaAssistant::init()
{
  QString error;
  bool bSuccess = m_db.open(qApp->applicationDirPath() +
                            QDir::separator() +
                            "setup.db",
                            error);
  if (bSuccess)
    bSuccess = m_db.init(error);
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
    m_db.selectSettings(m_settings);
    if (!m_settings.port.isEmpty())
      connect();
    m_noteWidget.setHistoryDatabase(m_db.getSqlDatabase());
    noteCreate();
  }

  enableControls();
}

void BaitaAssistant::noteCreate()
{
  if (m_bReady)
  {
    m_noteWidget.create(m_db.number(),
                        m_db.selectSuppliers(),
                        m_db.selectDescriptions());
    enableControls();
  }
}

void BaitaAssistant::showInfo()
{

}
