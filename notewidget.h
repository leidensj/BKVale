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
  Note getNote() const;
  void setNote(const Note& note);

private:
  static const QChar st_separator;
  Ui::NoteWidget *ui;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, Column column) const;
  QString text(int row, Column column)const ;
  QString serializeItems() const;

private slots:
  void updateTable(int row, Column column);
  void tableSelectionChanged();

public slots:
  void addItem();
  void removeItem();

signals:
  void tableSelectionChangedSignal();

};

#endif // BKFRAME_H
