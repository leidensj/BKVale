#ifndef TIMECARD_H
#define TIMECARD_H

#include <QDialog>

class DatabasePicker;
class QDateEdit;
class QDialogButtonBox;
class QCheckBox;
class JSpinBox;

class TimeCardDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TimeCardDialog(QWidget* parent = nullptr);

private slots:
  void saveAndAccept();
  void updateControls();

private:
  DatabasePicker* m_storePicker;
  QDateEdit* m_date;
  QDialogButtonBox* m_buttons;
  JSpinBox* m_spnExtraPages;
  QCheckBox* m_cbOpenFile;
};

#endif // TIMECARD_H
