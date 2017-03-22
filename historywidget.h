#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QFrame>
#include <note.h>

class QTableWidgetItem;

namespace Ui {
class HistoryWidget;
}

enum class HistColumn : int
{
  Number,
  Date,
  Supplier,
  Total
};

class HistoryWidget : public QFrame
{
  Q_OBJECT

  std::vector<Note> m_notes;

public:
  explicit HistoryWidget(QWidget *parent = 0);
  ~HistoryWidget();
  void refresh(Notes& notes);
  Note at(int idx) const;

private:
  Ui::HistoryWidget *ui;

private slots:
  void noteSelected(QTableWidgetItem*);

signals:
  void noteSelectedSignal(int idx);
};

#endif // HISTORYWIDGET_H
