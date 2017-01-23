#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <string>
#include <QMessageBox>
#include <QInputDialog>
#include <QSerialPortInfo>

namespace
{
    void fillComboBoxWithAvailablePorts(QComboBox* cb)
    {
        if (cb != nullptr)
        {
            const auto info = QSerialPortInfo::availablePorts();
            for (const auto& it : info)
                cb->addItem(it.portName());
        }
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

      QObject::connect(ui->actionTeste,
                       SIGNAL(triggered(bool)),
                       this,
                       SLOT(testPrint()));

      QComboBox* portList = new QComboBox;
      fillComboBoxWithAvailablePorts(portList);
      ui->mainToolBar->insertWidget(ui->actionConnect, portList);
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

void BKVale::testPrint()
{
    QString error;
    bool bRet = m_printer.connect(m_portName, error);

    if (bRet)
        bRet = m_printer.initialize(error);

    if (bRet)
    {
        QString text = QInputDialog::getText(this,
                                             tr("Enviar"),
                                             tr("User name:"));

        bRet = m_printer.write(text, error);
    }

    if (!bRet)
    {
        QMessageBox msg;
        msg.setText(error);
        msg.exec();
    }
}
