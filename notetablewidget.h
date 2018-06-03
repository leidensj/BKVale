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
  QVector<NoteItem> getNoteItems() const;
  void setNoteItems(const QVector<NoteItem>& vNoteItem);
  void setProduct(const Product& product);
  void setPackage(const Package& package);
  void setNoteItem(const NoteItem& noteItem);
  void addNoteItem(const NoteItem& noteItem);
  double computeTotal() const;

  void removeCurrentItem() { removeRow(currentRow()); }
  void removeAllItems() { setRowCount(0); }
  bool hasItems() const { return rowCount() != 0; }

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  QString text(int row, int column) const;
  void setText(int row, int column, const QString& str);
  double computePrice(int row) const;
  double computeSubTotal(int row) const;
  double evaluate(int row, int column) const;

private slots:
  void update(int row, int column);
  void emitChangedSignal();
  void emitEditSignal(int row, int column);

signals:
  void changedSignal();
  void packageSignal(const Package& package,
                     const QString& productUnity);
  void productSignal(const Product& product);
};

#endif // NOTETABLEWIDGET_H
