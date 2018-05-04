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

  QObject::connect(this,
                   SIGNAL(accepted()),
                   this,
                   SLOT(update()));

  QObject::connect(ui->btnDatabaseDir,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(selectFileDir()));

  ui->cbSerialBaudRate->addItem("1200", QSerialPort::Baud1200);
  ui->cbSerialBaudRate->addItem("2400", QSerialPort::Baud2400);
  ui->cbSerialBaudRate->addItem("4800", QSerialPort::Baud4800);
  ui->cbSerialBaudRate->addItem("9600", QSerialPort::Baud9600);
  ui->cbSerialBaudRate->addItem("19200", QSerialPort::Baud19200);
  ui->cbSerialBaudRate->addItem("38400", QSerialPort::Baud38400);
  ui->cbSerialBaudRate->addItem("57600", QSerialPort::Baud57600);
  ui->cbSerialBaudRate->addItem("115200", QSerialPort::Baud115200);

  ui->cbSerialFlowControl->addItem("No flow control", QSerialPort::NoFlowControl);
  ui->cbSerialFlowControl->addItem("Hardware flow control (RTS/CTS)", QSerialPort::HardwareControl);
  ui->cbSerialFlowControl->addItem("Software flow control (XON/XOFF)", QSerialPort::SoftwareControl);

  ui->cbSerialDataBits->addItem("5", QSerialPort::Data5);
  ui->cbSerialDataBits->addItem("6", QSerialPort::Data6);
  ui->cbSerialDataBits->addItem("7", QSerialPort::Data7);
  ui->cbSerialDataBits->addItem("8", QSerialPort::Data8);

  ui->cbSerialStopBits->addItem("1 stop bit", QSerialPort::OneStop);
  ui->cbSerialStopBits->addItem("1.5 stop bits", QSerialPort::OneAndHalfStop);
  ui->cbSerialStopBits->addItem("2 stop bits", QSerialPort::TwoStop);

  ui->cbSerialParity->addItem("No parity", QSerialPort::NoParity);
  ui->cbSerialParity->addItem("Even parity", QSerialPort::EvenParity);
  ui->cbSerialParity->addItem("Odd parity", QSerialPort::OddParity);
  ui->cbSerialParity->addItem("Space parity", QSerialPort::SpaceParity);
  ui->cbSerialParity->addItem("Mark parity", QSerialPort::MarkParity);

  ui->spNotesDefaultNumber->setValue(NOTE_DEFAULT_NUMBER);

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
    ui->edDatabaseDir->setText(m_settings.m_databaseDir);
    ui->rdoSerial->setChecked(m_settings.m_interfaceType == InterfaceType::Serial);
    ui->rdoEthernet->setChecked(m_settings.m_interfaceType == InterfaceType::Ethernet);
    ui->cbConnectStartup->setChecked(m_settings.m_bConnectOnStartup);
    ui->cbSerialPort->setCurrentIndex(ui->cbSerialPort->findText(m_settings.m_serialPort, Qt::MatchExactly));
    ui->cbSerialBaudRate->setCurrentIndex(ui->cbSerialBaudRate->findData(m_settings.m_serialBaudRate));
    ui->cbSerialFlowControl->setCurrentIndex(ui->cbSerialFlowControl->findData(m_settings.m_serialFlowControl));
    ui->cbSerialDataBits->setCurrentIndex(ui->cbSerialDataBits->findData(m_settings.m_serialDataBits));
    ui->cbSerialStopBits->setCurrentIndex(ui->cbSerialStopBits->findData(m_settings.m_serialStopBits));
    ui->cbSerialParity->setCurrentIndex(ui->cbSerialParity->findData(m_settings.m_serialParity));
    ui->edEthernetIP->setText(m_settings.m_ethernetIP);
    ui->sbEthernetPort->setValue(m_settings.m_ethernetPort);
    ui->spNotesDefaultNumber->setValue(m_settings.m_notesDefaultNumber);
  }
  else
  {
    m_settings.m_databaseDir = ui->edDatabaseDir->text();
    m_settings.m_interfaceType = ui->rdoSerial->isChecked() ? InterfaceType::Serial : InterfaceType::Ethernet;
    m_settings.m_bConnectOnStartup = ui->cbConnectStartup->isChecked();
    m_settings.m_serialPort = ui->cbSerialPort->currentText();
    m_settings.m_serialBaudRate = (QSerialPort::BaudRate)ui->cbSerialBaudRate->currentData().toUInt();
    m_settings.m_serialDataBits = (QSerialPort::DataBits)ui->cbSerialDataBits->currentData().toUInt();
    m_settings.m_serialFlowControl = (QSerialPort::FlowControl)ui->cbSerialFlowControl->currentData().toUInt();
    m_settings.m_serialParity = (QSerialPort::Parity)ui->cbSerialParity->currentData().toUInt();
    m_settings.m_serialStopBits = (QSerialPort::StopBits)ui->cbSerialStopBits->currentData().toUInt();
    m_settings.m_ethernetIP = ui->edEthernetIP->text();
    m_settings.m_ethernetPort = ui->sbEthernetPort->value();
    m_settings.m_notesDefaultNumber = ui->spNotesDefaultNumber->value();
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

void SettingsDlg::selectFileDir()
{
  QString fileDir = QFileDialog::getExistingDirectory(this,
                                                      QObject::tr("Selecionar local para salvar seus dados"),
                                                      m_settings.m_databaseDir,
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
  if (!fileDir.isEmpty())
    ui->edDatabaseDir->setText(fileDir);
}
