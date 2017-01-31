#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QDateTime>
#include "calendardlg.h"

#define ESC              "\x1b"
#define ESC_ALIGN_CENTER "\x1b\x61\x31"
#define ESC_ALIGN_LEFT   "\x1b\x61\x30"
#define ESC_FULL_CUT     "\x1b\x77"
#define ESC_PARTIAL_CUT  "\x1b\x6d"
#define ESC_STRESS_ON    "\x1b\x45"
#define ESC_STRESS_OFF   "\x1b\x46"
#define ESC_LF           "\n"
#define ESC_INIT         "\x1b\x40"
#define ESC_DOUBLE_FONT  "\x1b\x0e\x1b\x56" //ESC SO + ESC V
#define ESC_PORTUGUESE   "\x1b\x74\x08"

#define HEADER_COMPANY1   "BAITAKÃO"
#define HEADER_COMPANY2   "RESTAURANTE E LANCHERIA"
#define HEADER_TELEPHONE  "3228 1666"

#define DATE_TIME_FORMAT  "Data dd/MM/yyyy  Hora HH:mm:ss"

#define MAX_WIDTH        50
#define TABLE_TOP        "╔════════╦══╦══════════════════════╦══════╦══════╗"
#define SOME_TEXT        "║........║FD║asddsdsdsa@dfdfdfd    ║R$4,00║R$3,00║"
#define TABLE_MIDDLE     "╠════════╬══╬══════════════════════╬══════╬══════╣"

#define TABLE_BOTTOM     "╚════════╩══╩══════════════════════╩══════╩══════╝"

namespace
{
  bool printerPrint(QSerialPort& printer, const QString& msg, QString& error)
  {
    error.clear();
    QByteArray data(msg.toUtf8());
    bool bRet = false;
    auto nBytes = printer.write(data);

    if (nBytes != data.size())
    {
      error = QObject::tr("Erro '%1' ao imprimir: %2").arg(
                QString::number(printer.error()),
                printer.errorString());
    }
    else if (!printer.waitForBytesWritten(5000))
    {
      error = QObject::tr("Falha de timeout.");
    }
    else
    {
      bRet = true;
    }

    return bRet;
  }

  bool printerInit(QSerialPort& printer, QString& error)
  {
    error.clear();
    QString msg = QString(ESC_INIT) +
                  ESC_PORTUGUESE;
    return printerPrint(printer, msg, error);
  }

  bool printerPrintHeader(QSerialPort& printer, QString& error)
  {
    error.clear();
    QString msg = QString(ESC_ALIGN_CENTER) +
                  ESC_DOUBLE_FONT +
                  HEADER_COMPANY1 +
                  ESC_LF +
                  ESC_DOUBLE_FONT +
                  HEADER_COMPANY2 +
                  ESC_LF +
                  ESC_DOUBLE_FONT +
                  HEADER_TELEPHONE +
                  ESC_LF +
                  ESC_ALIGN_LEFT +
                  TABLE_TOP +
                  ESC_LF +
                  SOME_TEXT +
                  ESC_LF +
                  TABLE_BOTTOM +
                  ESC_LF +
                  "Data: " + ESC_STRESS_ON +
                  QDate::currentDate().toString("dd/MM/yyyy") +
                  ESC_STRESS_OFF +
                  ESC_LF +
                  "Hora: " +
                  ESC_STRESS_ON +
                  QTime::currentTime().toString("hh:mm:ss") +
                  ESC_STRESS_OFF
                  ESC_LF +
                  ESC_FULL_CUT;

    return printerPrint(printer, msg, error);
  }
}

BKVale::BKVale(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BKVale),
  m_availablePorts(nullptr)
{
  ui->setupUi(this);

  QObject::connect(ui->actionAdd,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(createNewItem()));

  QObject::connect(ui->tableWidget,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(evaluateCellContent(int, int)));

  QObject::connect(ui->actionRefresh,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(refreshAvailablePorts()));

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

  QObject::connect(ui->actionCalendar,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showCalendar()));

  m_availablePorts = new QComboBox;
  ui->mainToolBar->insertWidget(ui->actionRefresh, m_availablePorts);
  refreshAvailablePorts();
  updateUI();
}

BKVale::~BKVale()
{
  delete ui;
}

void BKVale::refreshAvailablePorts()
{
  if (m_availablePorts != nullptr)
  {
    m_availablePorts->clear();
    const auto info = QSerialPortInfo::availablePorts();
    for (const auto& it : info)
      m_availablePorts->addItem(it.portName());
  }
}

void BKVale::createNewItem()
{
  ui->tableWidget->insertRow(ui->tableWidget->rowCount());
  QComboBox* cb = new QComboBox();
  QStringList list;
  list << tr("UN") << tr("KG") << tr("FD");
  cb->insertItems(0, list);
  ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1, 1, cb);
}

void BKVale::evaluateCellContent(int row, int column)
{
  switch (column)
  {
    case Ammount:
    case UnitValue:
    case TotalValue:
    {
      int error = 0;
      auto exp = ui->tableWidget->item(row, column)->text().toStdString();
      double res = te_interp(exp.c_str(), &error);
      if (error == 0)
        ui->tableWidget->item(row, column)->setText(QString::number(res));
    }
    default:
      break;
  }
}

void BKVale::connect()
{
  if (m_printer.isOpen())
  {
    if (m_printer.portName() != m_availablePorts->currentText())
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

  if (!m_availablePorts->currentText().isEmpty())
  {
    m_printer.setPortName(m_availablePorts->currentText());
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
      if (!printerInit(m_printer, error))
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

  updateUI();
}

void BKVale::updateUI()
{
  const bool bIsOpen = m_printer.isOpen();
  ui->actionRefresh->setEnabled(!bIsOpen);
  ui->actionConnect->setEnabled(!bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionPrint->setEnabled(bIsOpen);
  m_availablePorts->setEnabled(!bIsOpen);
}

void BKVale::disconnect()
{
  if (m_printer.isOpen())
      m_printer.close();
  updateUI();
}

void BKVale::print()
{
  QString error;
  if (!printerPrintHeader(m_printer, error))
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
}

void BKVale::showCalendar()
{
  CalendarDlg dlg;
  dlg.exec();
}
