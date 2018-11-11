#ifndef PERSONTABLEWIDGET_H
#define PERSONTABLEWIDGET_H

#include "jitemtable.h"
#include "note.h"

class PersonTableWidget : public JTable
{
  Q_OBJECT

public:
  explicit PersonTableWidget(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItemAndLoadPrices(Id supplierId);
  void addItemAndLoadPricesByBarcode(Id supplierId);
  double computeTotal() const;

private:
  mutable NoteItem m_ref;
  double computePrice(int row) const;
  double computeSubTotal(int row) const;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // PERSONTABLEWIDGET_H
