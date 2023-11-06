#ifndef TIMECARDCONTROL_H
#define TIMECARDCONTROL_H

#include <QDialog>

class DatabasePicker;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QComboBox;
class TimeCardControlTable;

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
  QDateEdit* m_date;
  TimeCardControlTable* m_table;
  QDialogButtonBox* m_buttons;
};

#endif // TIMECARDCONTROL_H
