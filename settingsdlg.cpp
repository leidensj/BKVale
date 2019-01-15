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
