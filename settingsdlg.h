#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include <QSerialPort>
#include "settings.h"

namespace Ui {
class SettingsDlg;
}

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
  void doDataExchange(bool toUI);

private slots:
  void refreshAvailablePorts();
  void update();
  void restoreDefaults();
};

#endif // SETTINGSDLG_H
