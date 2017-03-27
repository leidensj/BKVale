#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notewidget.h"
#include "printutils.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>

BKVale::BKVale(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BKVale),
  m_bReady(false)
{
  ui->setupUi(this);
  ui->centralWidget->layout()->addWidget(&m_noteWidget);
  ui->dockContents->layout()->addWidget(&m_historyWidget);
  ui->dock->close();

  QObject::connect(ui->actionAdd,
                   SIGNAL(triggered(bool)),
                   &m_noteWidget,
                   SLOT(addItem()));

  QObject::connect(ui->actionRemove,
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
                   SLOT(print()));

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

  QObject::connect(ui->actionNew,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(createNew()));

  QObject::connect(ui->actionSearch,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showSearch()));

  QObject::connect(&m_historyWidget,
                   SIGNAL(noteSelectedSignal(int)),
                   this,
                   SLOT(openNote(int)));

  m_noteWidget.clear();
  enableControls();
}

BKVale::~BKVale()
{
  delete ui;
}

void BKVale::connect()
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

void BKVale::disconnect()
{
  if (m_printer.isOpen())
      m_printer.close();
  enableControls();
}

void BKVale::print()
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
      createNew();
    }
  }
}

void BKVale::showSettings()
{
  if (!m_printer.isOpen())
  {
    SettingsDlg dlg;
    if (dlg.exec() == QDialog::Accepted)
      m_settings = dlg.getSettings();
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

void BKVale::enableControls()
{
  const bool bIsOpen = m_printer.isOpen();
  const bool bCanEdit = m_bReady && !m_noteWidget.isHistoryMode() && ui->dock->isHidden();
  ui->actionConnect->setEnabled(!bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionPrint->setEnabled(m_noteWidget.isValid() && bIsOpen && m_bReady);
  ui->actionSettings->setEnabled(!bIsOpen);
  ui->actionAdd->setEnabled(bCanEdit);
  ui->actionRemove->setEnabled(bCanEdit && m_noteWidget.isValidSelection());
  m_noteWidget.setEnabled(bCanEdit);
}

bool BKVale::initDatabase(QString& error)
{
  error.clear();
  bool bSuccess = m_db.open(qApp->applicationDirPath() +
                            QDir::separator() +
                            "setup.db",
                            error);
  if (bSuccess)
    bSuccess = m_db.init(error);
  else
    m_db.close();

  return bSuccess;
}

void BKVale::createNew()
{
  QString error;
  if (!initDatabase(error))
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
    m_noteWidget.createNew(m_db.number(),
                           m_db.selectSuppliers(),
                           m_db.selectDescriptions());
    if (!ui->dock->isHidden())
      ui->dock->close();
    enableControls();
  }
}

void BKVale::showInfo()
{

}

void BKVale::showSearch()
{
  if (ui->dock->isHidden())
  {
    QString error;
    if (!initDatabase(error))
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
      Notes notes;
      QString error;
      m_db.selectAll(notes, error);
      m_historyWidget.refresh(notes);
      m_noteWidget.clear();
      ui->dock->show();
      enableControls();
    }
  }
}

void BKVale::openNote(int idx)
{
  Note note(m_historyWidget.at(idx));
  m_noteWidget.setNote(note);
  enableControls();
}
