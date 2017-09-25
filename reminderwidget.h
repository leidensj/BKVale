#ifndef REMINDERWIDGET_H
#define REMINDERWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "reminder.h"
#include <printutils.h>

class QDockWidget;
class ReminderView;
class ReminderDatabase;

class ReminderWidget : public QFrame
{
  Q_OBJECT

public:
  ReminderWidget(QWidget *parent = 0);
  bool isValid() const;
  bool print(QSerialPort& printer);
  bool save();
  void setDatabase(QSqlDatabase db);
  void clear();

private slots:
  void emitChangedSignal();
  void showDock();

signals:
  changedSignal();

private:
  ReminderView* m_view;
  ReminderDatabase* m_db;
  QDockWidget* m_dock;

};

#endif // REMINDERWIDGET_H
