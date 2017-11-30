#ifndef REMINDERDATABASE_H
#define REMINDERDATABASE_H

#include <QFrame>
#include <QTableView>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>
#include "reminder.h"

class QPushButton;

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

private slots:
  void enableControls();
  void emitSelectedSignal();

public slots:
  bool insertOrUpdate(const Reminder& r, QString& error);
  void remove();
  void refresh();
  void favorite(bool b);

signals:
  void selectedSignal(const Reminder& r);
  void removedSignal(int id);

private:
  QTableView* m_table;
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFavorite;
};

#endif // REMINDERDATABASE_H
