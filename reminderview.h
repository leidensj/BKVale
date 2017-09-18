#ifndef REMINDERVIEW_H
#define REMINDERVIEW_H

#include <QFrame>
#include "reminder.h"

namespace Ui {
class ReminderView;
}

class ReminderView : public QFrame
{
  Q_OBJECT

public:
  explicit ReminderView(QWidget *parent = 0);
  ~ReminderView();
  Reminder reminder() const;

  void clear();
  bool isValid() const;

private slots:
  void emitChangedSignal();
  void setCapitalization(int state);

public slots:
  void setReminder(const Reminder r);

signals:
  changedSignal();

private:
  Ui::ReminderView *ui;
};

#endif // REMINDERVIEW_H
