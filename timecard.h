#ifndef TIMECARD_H
#define TIMECARD_H

#include <QDialog>

class JDatabasePicker;
class QDateEdit;
class QDialogButtonBox;
class QCheckBox;

class TimeCard : public QDialog
{
  Q_OBJECT

public:
  explicit TimeCard(QWidget* parent = nullptr);

private slots:
  void saveAndAccept();
  void updateControls();

private:
  JDatabasePicker* m_storePicker;
  QDateEdit* m_date;
  QDialogButtonBox* m_buttons;
  QCheckBox* m_cbOpenFile;
};

#endif // TIMECARD_H
