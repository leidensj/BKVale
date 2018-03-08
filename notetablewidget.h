#ifndef NOTETABLEWIDGET_H
#define NOTETABLEWIDGET_H

#include <QTableWidget>
#include "note.h"

class QKeyEvent;

class NoteTableWidget : public QTableWidget
{
  Q_OBJECT

public:
  explicit NoteTableWidget(QWidget* parent = 0);
  QVector<NoteItemProduct>& getItems() const;
  void setItems(const QVector<NoteItemProduct>& vItemProduct,);
  void addItem(const NoteItemProduct& itemProduct);
  QString computeTotal() const;

  void removeCurrentItem() { removeRow(currentRow()); }
  void removeAllItems() { setRowCount(0); }
  bool hasItems() const { return rowCount() != 0; }

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  QString text(int row, int column) const;
  void setText(int row, int column, const QString& str);
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  double evaluate(int row, int column) const;

private slots:
  void update(int row, int column);
  void emitChangedSignal();

signals:
  void changedSignal();
};

#endif // NOTETABLEWIDGET_H
