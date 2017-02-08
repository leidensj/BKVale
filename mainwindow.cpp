#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDateTime>

#define ESC              "\x1b"
#define ESC_ALIGN_CENTER "\x1b\x61\x31"
#define ESC_ALIGN_LEFT   "\x1b\x61\x30"
#define ESC_FULL_CUT     "\x1b\x77"
#define ESC_PARTIAL_CUT  "\x1b\x6d"
#define ESC_STRESS_ON    "\x1b\x45"
#define ESC_STRESS_OFF   "\x1b\x46"
#define ESC_LF           "\n"
#define ESC_INIT         "\x1b\x40"
#define ESC_DOUBLE_FONT  "\x1b\x0e\x1b\x56"
#define ESC_PORTUGUESE   "\x1b\x74\x08"
#define ESC_REVERSE_ON   "\x1b\x7d\x31"
#define ESC_REVERSE_OFF  "\x1b\x7d\x30"

#define TABLE_WIDTH           48
#define TABLE_MAX_VALUE       10000

namespace
{
  bool printerPrint(QSerialPort& printer,
                    const QString& msg,
                    QString& error)
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

  bool printerInit(QSerialPort& printer,
                   QString& error)
  {
    error.clear();
    QString msg = QString(ESC_INIT) + ESC_PORTUGUESE;
    return printerPrint(printer, msg, error);
  }

  QString formatNumber(double d, bool b3)
  {
    return QString::number(d, 'f', b3 ? 3 : 2);
  }

  QString formatNumber(const QString& number, bool b3)
  {
    return formatNumber(number.toDouble(), b3);
  }

  QString buildTop(const QDate& date)
  {
    QString str = QString(ESC_ALIGN_CENTER) +
                  ESC_DOUBLE_FONT +
                  "B.K. RESTAURANTE"+
                  ESC_LF +
                  ESC_DOUBLE_FONT +
                  "E LANCHERIA LTDA" +
                  ESC_LF +
                  ESC_DOUBLE_FONT +
                  "3228-1666" +
                  ESC_LF +
                  ESC_ALIGN_LEFT +
                  ESC_LF +
                  ESC_LF +
                  "Data de impressão: " + ESC_STRESS_ON +
                  QDate::currentDate().toString("dd/MM/yyyy") +
                  ESC_STRESS_OFF +
                  ESC_LF +
                  "Hora de impressão: " +
                  ESC_STRESS_ON +
                  QTime::currentTime().toString("hh:mm:ss") +
                  ESC_LF +
                  ESC_LF +
                  ESC_ALIGN_CENTER +
                  "Data do vale:" +
                  ESC_LF
                  ESC_STRESS_OFF +
                  ESC_DOUBLE_FONT +
                  date.toString("dd/MM/yyyy\n(dddd)") +
                  ESC_LF +
                  ESC_LF;
    return str;
  }

  QString buildBottom(double total)
  {
    QString str = QString(ESC_LF) +
                  ESC_ALIGN_CENTER +
                  ESC_DOUBLE_FONT +
                  "TOTAL R$" +
                  formatNumber(total, false) +
                  ESC_LF +
                  ESC_LF +
                  ESC_FULL_CUT;
    return str;
  }

  QString buildTable(const QTableWidget& table)
  {
    QString str(ESC_ALIGN_LEFT);
    for (int row = 0; row != table.rowCount(); ++row)
    {
      QComboBox* pcb = static_cast<QComboBox*>(table.cellWidget(row, TableColumnUnity));
      QString subTotal(formatNumber(table.item(row, TableColumnAmmount)->text(), true) +
                       pcb->currentText() + " x R$" +
                       formatNumber(table.item(row, TableColumnUnitValue)->text(), false));

      {
        const QString subTotalValue("R$"+formatNumber(table.item(row, TableColumnSubTotalValue)->text(), false));
        const int n = TABLE_WIDTH - (subTotal.length() + subTotalValue.length());
        for (int i = 0; i != n; ++i)
          subTotal += " ";
        subTotal += ESC_STRESS_ON + subTotalValue + ESC_STRESS_OFF;
      }

      str += table.item(row, TableColumnDescription)->text() + ESC_LF;
      str += subTotal + ESC_LF;
      str += QString("────────────────────────────────────────────────") + ESC_LF;
    }
    return str;
  }
}

BKVale::BKVale(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BKVale)
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

  ui->dateEdit->setDate(QDate::currentDate());
  updateUI();
}

BKVale::~BKVale()
{
  delete ui;
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
    case TableColumnAmmount:
    case TableColumnUnitValue:
    case TableColumnSubTotalValue:
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
  ui->actionConnect->setEnabled(!bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionDisconnect->setEnabled(bIsOpen);
  ui->actionPrint->setEnabled(bIsOpen);
  ui->actionSettings->setEnabled(!bIsOpen);
}

void BKVale::disconnect()
{
  if (m_printer.isOpen())
      m_printer.close();
  updateUI();
}

void BKVale::print()
{
  QString str = buildTop(ui->dateEdit->date());
  str += buildTable(*ui->tableWidget);
  str += buildBottom(total());
  QString error;
  if (!printerPrint(m_printer, str, error))
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       tr("Erro"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
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

double BKVale::total() const
{
  double total = 0.0;
  for (int i = 0; i != ui->tableWidget->rowCount(); ++i)
    total += ui->tableWidget->item(i, TableColumnSubTotalValue)->text().toDouble();
  return total;
}
