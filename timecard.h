#ifndef TIMECARD_H
#define TIMECARD_H

#include <QDialog>

class JDatabasePicker;
class QDateEdit;

class TimeCard : public QDialog
{
  Q_OBJECT

public:
  explicit TimeCard(QWidget* parent = nullptr);

private slots:
  void saveAndAccept();

private:
  JDatabasePicker* m_storePicker;
  QDateEdit* m_date;
};

#endif // TIMECARD_H
