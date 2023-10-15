#ifndef TIMECARDCONTROL_H
#define TIMECARDCONTROL_H

#include <QDialog>

class DatabasePicker;
class JDateInterval;
class QDialogButtonBox;
class QLabel;

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
  DatabasePicker* m_employeePicker;
  JDateInterval* m_date;
  QDialogButtonBox* m_buttons;
};

#endif // TIMECARDCONTROL_H
