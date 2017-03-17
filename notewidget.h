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
  bool isValid() const;
  bool isDirty() const;

private:
  static const QChar st_separator;
  Ui::NoteWidget *ui;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column) const;
  QString text(int row, int column) const ;
  QString serializeItems() const;
  bool m_bDirty;

private slots:
  void updateTable(int row, int column);
  void changed();

public slots:
  void addItem();
  void removeItem();
  void clear(int number);
  void setEnabled(bool bEnable);

signals:
  void changedSignal();

};

#endif // BKFRAME_H
