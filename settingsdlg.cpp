#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include "defines.h"
#include <QSerialPortInfo>
#include <QPushButton>
#include <QFileDialog>

SettingsDlg::SettingsDlg(const Settings& settings, QWidget *parent)
  : QDialog(parent)
  , m_settings(settings)
  , ui(new Ui::SettingsDlg)
{
  ui->setupUi(this);

  ui->cbBaudRate->addItem("1200", QSerialPort::Baud1200);
  ui->cbBaudRate->addItem("2400", QSerialPort::Baud2400);
  ui->cbBaudRate->addItem("4800", QSerialPort::Baud4800);
  ui->cbBaudRate->addItem("9600", QSerialPort::Baud9600);
  ui->cbBaudRate->addItem("19200", QSerialPort::Baud19200);
  ui->cbBaudRate->addItem("38400", QSerialPort::Baud38400);
  ui->cbBaudRate->addItem("57600", QSerialPort::Baud57600);
  ui->cbBaudRate->addItem("115200", QSerialPort::Baud115200);
  ui->cbBaudRate->setCurrentIndex(ui->cbBaudRate->findData(QSerialPort::Baud9600));

  ui->cbDataBits->addItem("Data5", QSerialPort::Data5);
  ui->cbDataBits->addItem("Data6", QSerialPort::Data6);
  ui->cbDataBits->addItem("Data7", QSerialPort::Data7);
  ui->cbDataBits->addItem("Data8", QSerialPort::Data8);
  ui->cbDataBits->setCurrentIndex(ui->cbDataBits->findData(QSerialPort::Data8));

  ui->cbFlowControl->addItem("NoFlowControl", QSerialPort::NoFlowControl);
  ui->cbFlowControl->addItem("HardwareControl (RTS/CTS)", QSerialPort::HardwareControl);
  ui->cbFlowControl->addItem("SoftwareControl (XON/XOFF)", QSerialPort::SoftwareControl);
  ui->cbFlowControl->setCurrentIndex(ui->cbFlowControl->findData(QSerialPort::NoFlowControl));

  ui->cbParity->addItem("NoParity", QSerialPort::NoParity);
  ui->cbParity->addItem("EvenParity", QSerialPort::EvenParity);
  ui->cbParity->addItem("OddParity", QSerialPort::OddParity);
  ui->cbParity->addItem("SpaceParity", QSerialPort::SpaceParity);
  ui->cbParity->addItem("MarkParity", QSerialPort::MarkParity);
  ui->cbParity->setCurrentIndex(ui->cbParity->findData(QSerialPort::NoParity));

  ui->cbStopBits->addItem("OneStop", QSerialPort::OneStop);
  ui->cbStopBits->addItem("OneAndHalfStop", QSerialPort::OneAndHalfStop);
  ui->cbStopBits->addItem("TwoStop", QSerialPort::TwoStop);
  ui->cbStopBits->setCurrentIndex(ui->cbStopBits->findData(QSerialPort::OneStop));

  connect(this, SIGNAL(accepted()), this, SLOT(update()));
  refreshAvailablePorts();
  doDataExchange(true);
}

SettingsDlg::~SettingsDlg()
{
  delete ui;
}

void SettingsDlg::doDataExchange(bool toUI)
{
  if (toUI)
  {
    ui->rdoSerial->setChecked(!m_settings.m_bIsPrinterEthernet);
    ui->rdoEthernet->setChecked(m_settings.m_bIsPrinterEthernet);
    ui->cbSerialPort->setCurrentIndex(ui->cbSerialPort->findText(m_settings.m_serialPort, Qt::MatchExactly));
    ui->cbBaudRate->setCurrentIndex(ui->cbBaudRate->findData(m_settings.m_serialBaudRate));
    ui->cbDataBits->setCurrentIndex(ui->cbDataBits->findData(m_settings.m_serialDataBits));
    ui->cbFlowControl->setCurrentIndex(ui->cbFlowControl->findData(m_settings.m_serialFlowControl));
    ui->cbParity->setCurrentIndex(ui->cbParity->findData(m_settings.m_serialParity));
    ui->cbStopBits->setCurrentIndex(ui->cbStopBits->findData(m_settings.m_serialStopBits));
    ui->edEthernetIP->setText(m_settings.m_ethernetIP);
    ui->sbEthernetPort->setValue(m_settings.m_ethernetPort);
  }
  else
  {
    m_settings.m_bIsPrinterEthernet = ui->rdoEthernet->isChecked();
    m_settings.m_serialPort = ui->cbSerialPort->currentText();
    m_settings.m_serialBaudRate = (QSerialPort::BaudRate)ui->cbBaudRate->currentData().toInt();
    m_settings.m_serialDataBits = (QSerialPort::DataBits)ui->cbDataBits->currentData().toInt();
    m_settings.m_serialFlowControl = (QSerialPort::FlowControl)ui->cbFlowControl->currentData().toInt();
    m_settings.m_serialParity = (QSerialPort::Parity)ui->cbParity->currentData().toInt();
    m_settings.m_serialStopBits = (QSerialPort::StopBits)ui->cbStopBits->currentData().toInt();
    m_settings.m_ethernetIP = ui->edEthernetIP->text();
    m_settings.m_ethernetPort = ui->sbEthernetPort->value();
  }
}

void SettingsDlg::refreshAvailablePorts()
{
  ui->cbSerialPort->clear();
  const auto info = QSerialPortInfo::availablePorts();
  for (const auto& it : info)
    ui->cbSerialPort->addItem(it.portName());
}

void SettingsDlg::update()
{
  doDataExchange(false);
}
