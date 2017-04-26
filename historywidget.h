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

class HistoryWidget : public QFrame
{
  Q_OBJECT

public:
  explicit HistoryWidget(QWidget *parent = 0);
  ~HistoryWidget();
  void set(const QSqlDatabase& sqldb);
  Note at(int idx) const;

private:
  Ui::HistoryWidget *ui;
  QSqlTableModel* m_model;

private slots:
  void noteSelected(const QModelIndex& idx);

signals:
  void noteSelectedSignal(int idx);
};

#endif // HISTORYWIDGET_H
