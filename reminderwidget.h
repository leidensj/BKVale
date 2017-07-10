#ifndef REMINDERWIDGET_H
#define REMINDERWIDGET_H

#include <QFrame>
#include "reminder.h"

namespace Ui {
class ReminderWidget;
}

class ReminderWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ReminderWidget(QWidget *parent = 0);
  ~ReminderWidget();
  Reminder reminder() const;
  void clear();
  bool isValid() const;

private:
  Ui::ReminderWidget *ui;
};

#endif // REMINDERWIDGET_H
