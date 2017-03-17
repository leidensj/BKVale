#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QFrame>
#include <note.h>

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

private:
  Ui::HistoryWidget *ui;
};

#endif // HISTORYWIDGET_H
