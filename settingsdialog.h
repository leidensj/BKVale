#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(const Settings& settings, QWidget *parent = 0);
  ~SettingsDialog();
  const Settings& getSettings() const { return m_settings; }

private:
  Settings m_settings;
  Ui::SettingsDialog *ui;
  void doDataExchange(bool toUI);

private slots:
  void refreshAvailablePorts();
  void update();
};

#endif // SETTINGSDIALOG_H
