#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class SettingsDlg;
}

struct Settings
{
  QString port;
  QSerialPort::BaudRate baudRate;
  QSerialPort::DataBits dataBits;
  QSerialPort::FlowControl flowControl;
  QSerialPort::Parity parity;
  QSerialPort::StopBits stopBits;

  void clear();
};

class SettingsDlg : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDlg(QWidget *parent = 0);
  ~SettingsDlg();
  const Settings& getSettings() const { return m_settings; }

private:
  Ui::SettingsDlg *ui;
  Settings m_settings;
  void init();

private slots:
  void refreshAvailablePorts();
  void updateSettings();
};

#endif // SETTINGSDLG_H
