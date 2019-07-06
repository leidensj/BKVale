#ifndef DISCOUNTTABLE_H
#define DISCOUNTTABLE_H

#include <QTableWidget>
#include "items/discount.h"

class QKeyEvent;

class DiscountTable : public QTableWidget
{
  Q_OBJECT

public:
  explicit DiscountTable(QWidget* parent = 0);
  QVector<DiscountItem> getDiscountItems() const;
  void setDiscountItems(const QVector<DiscountItem>& v);
  void setProduct(const Product& o);
  void setDiscountItem(const DiscountItem& o);
  void addDiscountItem(const DiscountItem& o);
  double computeTotal() const;

  void removeCurrentItem() { removeRow(currentRow()); }
  void removeAllItems() { setRowCount(0); }
  bool hasItems() const { return rowCount() != 0; }

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  QString text(int row, int column) const;
  void setText(int row, int column, const QString& str);
  double evaluate(int row, int column) const;

private slots:
  void update(int row, int column);
  void emitChangedSignal();
  void emitEditSignal(int row, int column);

signals:
  void changedSignal();
  void productSignal(const Product& product);
};

#endif // DISCOUNTTABLE_H
