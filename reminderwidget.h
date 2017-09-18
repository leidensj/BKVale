#ifndef REMINDERWIDGET_H
#define REMINDERWIDGET_H

#include <QFrame>
#include "reminder.h"
#include "reminderdatabase.h"
#include "reminderview.h"
#include <printutils.h>

class ReminderWidget : public QFrame
{
  Q_OBJECT

private:
  ReminderView m_view;
  ReminderDatabase m_db;

public:
  ReminderWidget(QWidget *parent = 0);
  bool isValid() const { return m_view.isValid(); }
  bool print(QSerialPort& printer);
  bool save();
  void setDatabase(QSqlDatabase db);
  void clear();

private slots:
  void emitChangedSignal();

signals:
  changedSignal();
};

#endif // REMINDERWIDGET_H
