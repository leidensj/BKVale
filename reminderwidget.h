#ifndef REMINDERWIDGET_H
#define REMINDERWIDGET_H

#include <QFrame>
#include "reminder.h"
#include "reminderdatabase.h"
#include "reminderview.h"

class ReminderWidget : public QFrame
{
  Q_OBJECT

private:
  ReminderView m_view;
  ReminderDatabase m_db;

public:
  ReminderWidget(QWidget *parent = 0);
  Reminder reminder() const { return m_view.reminder(); }
  bool isValid() const { return m_view.isValid(); }

private slots:
  void emitChangedSignal();

signals:
  changedSignal();
};

#endif // REMINDERWIDGET_H
