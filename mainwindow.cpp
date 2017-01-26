#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QSerialPortInfo>
#include <QByteArray>

namespace
{
    bool print(QSerialPort& printer, const QString& msg, QString error)
    {
        error.clear();
        QByteArray data(msg.toLocal8Bit() + "\n");
        bool bRet = false;
        auto nBytes = printer.write(data);
        if (nBytes == -1 || nBytes != data.size())
        {
            error = QObject::tr("Erro '%1' ao imprimir: %2").arg(
                        QString::number(printer.error()),
                        printer.errorString());
        }
        else if (!printer.waitForBytesWritten(5000))
        {
            error = QObject::tr("Erro ao imprimir: falha de timeout.");
        }
        else
        {
            bRet = true;
        }

        return bRet;
    }

    bool printHeader(QSerialPort& printer, QString& error)
    {
        error.clear();

    }

    bool printInit(QSerialPort& printer, QString& error)
    {
        error.clear();
        QString msg = "\n\x1b@";
        msg += "\nhaha";
        return print(printer, msg, error);
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
                               tr("Você já está conectado a está impressora."),
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
                               tr("O seguinte erro ocorreu ao conectar à impressora: '") +
                               m_printer.errorString() + "'.",
                               QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("Atenção"),
                           tr("É necessário selecionar uma porta para se conectar à impressora."),
                           QMessageBox::Ok);
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
