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
    ui->rdoSerial->setChecked(!m_settings.m_bIsPrinterEthernet);
    ui->rdoEthernet->setChecked(m_settings.m_bIsPrinterEthernet);
    ui->cbSerialPort->setCurrentIndex(ui->cbSerialPort->findText(m_settings.m_serialPort, Qt::MatchExactly));
    ui->edEthernetIP->setText(m_settings.m_ethernetIP);
    ui->sbEthernetPort->setValue(m_settings.m_ethernetPort);
    ui->spNotesDefaultNumber->setValue(m_settings.m_notesDefaultNumber);
    ui->cbNotePIN->setChecked(m_settings.m_notesPincodeRequired);
  }
  else
  {
    m_settings.m_databaseDir = ui->edDatabaseDir->text();
    m_settings.m_bIsPrinterEthernet = ui->rdoEthernet->isChecked();
    m_settings.m_serialPort = ui->cbSerialPort->currentText();
    m_settings.m_ethernetIP = ui->edEthernetIP->text();
    m_settings.m_ethernetPort = ui->sbEthernetPort->value();
    m_settings.m_notesDefaultNumber = ui->spNotesDefaultNumber->value();
    m_settings.m_notesPincodeRequired = ui->cbNotePIN->isChecked();
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
                                                      QObject::tr("Selecionar local do banco de dados"),
                                                      m_settings.m_databaseDir,
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
  if (!fileDir.isEmpty())
    ui->edDatabaseDir->setText(fileDir);
}
