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
  explicit SettingsDlg(const Settings& settings, QWidget *parent = 0);
  ~SettingsDlg();
  const Settings& getSettings() const { return m_settings; }

private:
  Settings m_settings;
  Ui::SettingsDlg *ui;
  void doDataExchange(bool toUI);

private slots:
  void refreshAvailablePorts();
  void update();
};

#endif // SETTINGSDLG_H
