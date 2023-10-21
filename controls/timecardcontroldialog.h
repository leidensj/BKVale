#ifndef TIMECARDCONTROL_H
#define TIMECARDCONTROL_H

#include <QDialog>

class DatabasePicker;
class JDateInterval;
class QDialogButtonBox;
class QLabel;
class QComboBox;

class TimeCardControlDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TimeCardControlDialog(QWidget* parent = nullptr);

private slots:
  void saveAndAccept();
  void updateControls();

private:
  DatabasePicker* m_storePicker;
  QComboBox* m_cbEmployee;
  JDateInterval* m_date;
  QDialogButtonBox* m_buttons;
};

#endif // TIMECARDCONTROL_H
