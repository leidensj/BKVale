#ifndef CALENDARDLG_H
#define CALENDARDLG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class CalendarDlg;
}

class CalendarDlg : public QDialog
{
  Q_OBJECT

public:
  explicit CalendarDlg(QWidget *parent = 0);
  ~CalendarDlg();
  const QDate& getDate();

private:
  Ui::CalendarDlg *ui;
  QDate m_date;

private slots:
  void updateDate(const QDate & date);
};

#endif // CALENDARDLG_H
