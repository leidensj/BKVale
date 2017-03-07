#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include "note.h"

namespace Ui {
class NoteWidget;
}



class NoteWidget : public QFrame
{
  Q_OBJECT

public:
  explicit NoteWidget(QWidget *parent = 0);
  ~NoteWidget();
  bool isValidSelection() const;
  void getNote(Note& note) const;
  void setNote(const Note& note);

private:
  static const QChar st_separator;
  Ui::PromissoryNoteWidget *ui;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column);

private slots:
  void updateTable(int row, int column);
  void tableSelectionChanged();

public slots:
  void addItem();
  void removeItem();

signals:
  void tableSelectionChangedSignal();

};

#endif // BKFRAME_H
