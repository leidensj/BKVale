#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QFrame>
#include <note.h>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>

class QTableWidgetItem;

namespace Ui {
class NoteDatabaseWidget;
}

class NoteDatabaseTableModel : public QSqlTableModel
{
  Q_OBJECT

public:
  NoteDatabaseTableModel(QObject *parent, QSqlDatabase db);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class NoteDatabaseWidget : public QFrame
{
  Q_OBJECT

public:
  explicit NoteDatabaseWidget(QWidget *parent = 0);
  ~NoteDatabaseWidget();
  void setDatabase(QSqlDatabase db);
  QSqlDatabase getDatabase() const;

public slots:
  void refresh();
  void removeSelectedNote();
  void enableControls();

private:
  Ui::NoteDatabaseWidget *ui;
  NoteDatabaseTableModel* m_model;

private slots:
  void noteSelected(const QModelIndex& idx);
  void noteSelected();

signals:
  void noteSelectedSignal(const Note& note);
  void noteRemovedSignal(int id);
};

#endif // HISTORYWIDGET_H
