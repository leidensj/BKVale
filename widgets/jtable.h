#ifndef JITEMTABLE_H
#define JITEMTABLE_H

#include <QTableWidget>
#include "jaddremovebuttons.h"
#include "tableitems/jtableitem.h"

class QKeyEvent;

class JTable : protected QTableWidget
{
  Q_OBJECT

public:
  explicit JTable(JAddRemoveButtons* btns, QWidget* parent);
  ~JTable();
  void setLargerSize(bool b);
  bool isValidRow(int row) const;
  bool hasItems() const;
  bool isValidCurrentRow() const;
  JTableItem* getItem(int row, int column) const;
  double sum(int column) const;

public slots:
  virtual void removeItem();
  virtual void removeAllItems();
  virtual void addRow() = 0;
  virtual void addRowAndActivate();
  void setEnabled(bool b);

protected slots:
  void emitChangedSignal();
  void activate(QTableWidgetItem* p);
  void erase(QTableWidgetItem* p);
  void evaluate(QTableWidgetItem* p);

protected:
  void keyPressEvent(QKeyEvent *event);
  void setHeaderIcon(int pos, const QIcon& icon);

signals:
  void changedSignal(bool bIsRowSelected);
  void changedSignal(int row, int column);
};

#endif // JITEMTABLE_H
