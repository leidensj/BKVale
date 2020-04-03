#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "defines.h"
#include <QSerialPortInfo>
#include <QPushButton>
#include <QFileDialog>

SettingsDialog::SettingsDialog(const Settings& settings, QWidget *parent)
  : QDialog(parent)
  , m_settings(settings)
  , ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);

  connect(this, SIGNAL(accepted()), this, SLOT(update()));
  refreshAvailablePorts();
  doDataExchange(true);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::doDataExchange(bool toUI)
{
  if (toUI)
  {
    ui->rdoSerial->setChecked(!m_settings.m_bIsPrinterEthernet);
    ui->rdoEthernet->setChecked(m_settings.m_bIsPrinterEthernet);
    ui->cbSerialPort->setCurrentIndex(ui->cbSerialPort->findText(m_settings.m_serialPort, Qt::MatchExactly));
    ui->edEthernetIP->setText(m_settings.m_ethernetIP);
    ui->sbEthernetPort->setValue(m_settings.m_ethernetPort);
  }
  else
  {
    m_settings.m_bIsPrinterEthernet = ui->rdoEthernet->isChecked();
    m_settings.m_serialPort = ui->cbSerialPort->currentText();
    m_settings.m_ethernetIP = ui->edEthernetIP->text();
    m_settings.m_ethernetPort = ui->sbEthernetPort->value();
  }
}

void SettingsDialog::refreshAvailablePorts()
{
  ui->cbSerialPort->clear();
  const auto info = QSerialPortInfo::availablePorts();
  for (const auto& it : info)
    ui->cbSerialPort->addItem(it.portName());
}

void SettingsDialog::update()
{
  doDataExchange(false);
}
