#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "promissorynotewidget.h"
#include "printutils.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>

BKVale::BKVale(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BKVale),
  m_promissoryNoteWidget(nullptr)
{
  ui->setupUi(this);
  m_promissoryNoteWidget = new PromissoryNoteWidget();
  ui->centralWidget->layout()->addWidget(m_promissoryNoteWidget);

  QObject::connect(ui->actionAdd,
                   SIGNAL(triggered(bool)),
                   m_promissoryNoteWidget,
                   SLOT(addItem()));

  QObject::connect(ui->actionRemove,
                   SIGNAL(triggered(bool)),
                   m_promissoryNoteWidget,
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

<<<<<<< HEAD
  QObject::connect(ui->actionInfo,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showInfo()));

  QObject::connect(m_promissoryNoteWidget,
=======
  QObject::connect(m_bkframe,
>>>>>>> parent of da3042d... criando banco de dados
                   SIGNAL(tableSelectionChangedSignal()),
                   this,
                   SLOT(enableControls()));

  enableControls();
}

BKVale::~BKVale()
{
  delete ui;
  ui = nullptr;
  m_promissoryNoteWidget = nullptr;
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
      return;
    }
  }

  if (!m_settings.port.isEmpty())
  {
    m_printer.setPortName(m_settings.port);
    if (!m_printer.open((QIODevice::ReadWrite)))
    {
      QMessageBox msgBox(QMessageBox::Critical,
                         tr("Erro número: ") + QString::number(m_printer.error()),
                         tr("O seguinte erro ocorreu ao conectar à impressora: '%1'.").arg(
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
<<<<<<< HEAD
  QString str = PrintUtils::buildHeader(m_promissoryNoteWidget->getDate());
  str += PrintUtils::buildBody(*m_promissoryNoteWidget);
  str += PrintUtils::buildFooter(m_promissoryNoteWidget->getTotal());
=======
  TableContent tableContent;
  QString total;
  m_bkframe->getContent(tableContent, total);

  QString str = PrintUtils::buildHeader(ui->date->date());
  str += PrintUtils::buildBody(tableContent);
  str += PrintUtils::buildFooter(total);
>>>>>>> parent of da3042d... criando banco de dados

  QString error;
  if (!PrintUtils::print(m_printer, str, error))
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
<<<<<<< HEAD
  else
  {
    if (!m_db.insert(*m_promissoryNoteWidget, error))
    {
      QMessageBox msgBox(QMessageBox::Warning,
                         tr("Erro ao salvar vale"),
                         error,
                         QMessageBox::Ok);
      msgBox.exec();
    }
  }
=======
>>>>>>> parent of da3042d... criando banco de dados
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
  Q_ASSERT(m_promissoryNoteWidget != nullptr);
  const bool bIsOpen = m_printer.isOpen();
  ui->actionConnect->setEnabled(!bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionPrint->setEnabled(bIsOpen);
  ui->actionSettings->setEnabled(!bIsOpen);
  ui->actionRemove->setEnabled(m_promissoryNoteWidget->isValidSelection());
}
