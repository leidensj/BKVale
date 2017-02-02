#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include <QSerialPortInfo>

void Settings::clear()
{
  port.clear();
  baudRate = QSerialPort::Baud9600;
  dataBits = QSerialPort::Data8;
  flowControl = QSerialPort::NoFlowControl;
  parity = QSerialPort::NoParity;
  stopBits = QSerialPort::OneStop;
}

SettingsDlg::SettingsDlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDlg)
{
  ui->setupUi(this);

  QObject::connect(this,
                   SIGNAL(accepted()),
                   this,
                   SLOT(updateSettings()));

  ui->cbBaudrate->addItem("1200", QSerialPort::Baud1200);
  ui->cbBaudrate->addItem("2400", QSerialPort::Baud2400);
  ui->cbBaudrate->addItem("4800", QSerialPort::Baud4800);
  ui->cbBaudrate->addItem("9600", QSerialPort::Baud9600);
  ui->cbBaudrate->addItem("19200", QSerialPort::Baud19200);
  ui->cbBaudrate->addItem("38400", QSerialPort::Baud38400);
  ui->cbBaudrate->addItem("57600", QSerialPort::Baud57600);
  ui->cbBaudrate->addItem("115200", QSerialPort::Baud115200);

  ui->cbFlow->addItem("No flow control", QSerialPort::NoFlowControl);
  ui->cbFlow->addItem("Hardware flow control (RTS/CTS)", QSerialPort::HardwareControl);
  ui->cbFlow->addItem("Software flow control (XON/XOFF)", QSerialPort::SoftwareControl);

  ui->cbDataBits->addItem("5", QSerialPort::Data5);
  ui->cbDataBits->addItem("6", QSerialPort::Data6);
  ui->cbDataBits->addItem("7", QSerialPort::Data7);
  ui->cbDataBits->addItem("8", QSerialPort::Data8);

  ui->cbStopBits->addItem("1 stop bit", QSerialPort::OneStop);
  ui->cbStopBits->addItem("1.5 stop bits", QSerialPort::OneAndHalfStop);
  ui->cbStopBits->addItem("2 stop bits", QSerialPort::TwoStop);

  ui->cbParity->addItem("No parity", QSerialPort::NoParity);
  ui->cbParity->addItem("Even parity", QSerialPort::EvenParity);
  ui->cbParity->addItem("Odd parity", QSerialPort::OddParity);
  ui->cbParity->addItem("Space parity", QSerialPort::SpaceParity);
  ui->cbParity->addItem("Mark parity", QSerialPort::MarkParity);

  refreshAvailablePorts();

  init();
}

SettingsDlg::~SettingsDlg()
{
  delete ui;
}

void SettingsDlg::init()
{
  m_settings.clear();
  ui->cbBaudrate->setCurrentIndex(ui->cbBaudrate->findData(m_settings.baudRate));
  ui->cbFlow->setCurrentIndex(ui->cbFlow->findData(m_settings.flowControl));
  ui->cbDataBits->setCurrentIndex(ui->cbDataBits->findData(m_settings.dataBits));
  ui->cbStopBits->setCurrentIndex(ui->cbStopBits->findData(m_settings.stopBits));
  ui->cbParity->setCurrentIndex(ui->cbParity->findData(m_settings.parity));
}

void SettingsDlg::refreshAvailablePorts()
{
  ui->cbPort->clear();
  const auto info = QSerialPortInfo::availablePorts();
  for (const auto& it : info)
    ui->cbPort->addItem(it.portName());
}

void SettingsDlg::updateSettings()
{
  m_settings.port = ui->cbPort->currentText();
  m_settings.baudRate = (QSerialPort::BaudRate)ui->cbBaudrate->currentData().toUInt();
  m_settings.dataBits = (QSerialPort::DataBits)ui->cbDataBits->currentData().toUInt();
  m_settings.flowControl = (QSerialPort::FlowControl)ui->cbFlow->currentData().toUInt();
  m_settings.parity = (QSerialPort::Parity)ui->cbParity->currentData().toUInt();
  m_settings.stopBits = (QSerialPort::StopBits)ui->cbStopBits->currentData().toUInt();
}
