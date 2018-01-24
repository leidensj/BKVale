#ifndef REMINDERWIDGET_H
#define REMINDERWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "reminder.h"
#include "settings.h"

class QDockWidget;
class ReminderView;
class ReminderDatabase;
class QIODevice;

class ReminderWidget : public QFrame
{
  Q_OBJECT

public:
  ReminderWidget(QWidget *parent = 0);
  bool isValid() const;
  void setDatabase(QSqlDatabase db);
  void saveAndPrint(QIODevice* printer,
                    InterfaceType type,
                    int userId);

private slots:
  void emitChangedSignal();
  void showDock();
  void removed(int id);

signals:
  changedSignal();

private:
  ReminderView* m_view;
  ReminderDatabase* m_db;
  QDockWidget* m_dock;
  bool print(QIODevice* printer, InterfaceType type);
  bool save(int userId);
};

#endif // REMINDERWIDGET_H
