#ifndef REMINDERDATABASE_H
#define REMINDERDATABASE_H

#include <QFrame>
#include <QTableView>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>
#include "reminder.h"

class ReminderTableModel : public QSqlTableModel
{
  Q_OBJECT

public:
  ReminderTableModel(QObject *parent, QSqlDatabase db);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class ReminderDatabase : public QFrame
{
  Q_OBJECT

public:
  ReminderDatabase(QWidget *parent = 0);
  void setDatabase(QSqlDatabase db);

private:
  QTableView* m_table;
};

#endif // REMINDERDATABASE_H
