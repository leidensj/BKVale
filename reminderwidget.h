#ifndef REMINDER_H
#define REMINDER_H

#include <QFrame>

namespace Ui {
class ReminderWidget;
}

class ReminderWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ReminderWidget(QWidget *parent = 0);
  ~ReminderWidget();

private:
  Ui::ReminderWidget *ui;
};

#endif // REMINDER_H
