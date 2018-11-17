#ifndef TIMECARD_H
#define TIMECARD_H

#include <QDialog>

class JDatabasePicker;
class QDateEdit;
class QDialogButtonBox;

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
};

#endif // TIMECARD_H
