#ifndef TIMECARD_H
#define TIMECARD_H

#include <QDialog>

class DatabasePicker;
class QDateEdit;
class QDialogButtonBox;
class QCheckBox;
class JSpinBox;
class DayOffTable;
class QLabel;

class TimeCardDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TimeCardDialog(QWidget* parent = nullptr);

private slots:
  void saveAndAccept();
  void updateControls();
  void saveDayOff();
  void updateMessageAndSwapButton();
  void shuffle();

private:
  DatabasePicker* m_storePicker;
  QDateEdit* m_date;
  QDialogButtonBox* m_buttons;
  JSpinBox* m_spnExtraPages;
  QCheckBox* m_cbOpenFile;
  DayOffTable* m_dayOffTable;
  QPushButton* m_btnCSV;
  QPushButton* m_btnComplete;
  QPushButton* m_btnSwap;
  QLabel* m_lblMessage;
};

#endif // TIMECARD_H
