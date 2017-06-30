#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QFrame>
#include <note.h>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>

class QTableWidgetItem;

namespace Ui {
class HistoryWidget;
}

class HistoryTableModel : public QSqlTableModel
{
  Q_OBJECT

public:
  HistoryTableModel(QObject *parent, QSqlDatabase db);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class HistoryWidget : public QFrame
{
  Q_OBJECT

public:
  explicit HistoryWidget(QWidget *parent = 0);
  ~HistoryWidget();
  void setDatabase(const QSqlDatabase& sqldb);
  Note at(int idx) const;

public slots:
  void refresh();

private:
  Ui::HistoryWidget *ui;
  HistoryTableModel* m_model;

private slots:
  void noteSelected(const QModelIndex& idx);
  void noteSelected();

signals:
  void noteSelectedSignal(const Note& note);
};

#endif // HISTORYWIDGET_H
