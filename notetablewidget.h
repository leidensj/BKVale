#ifndef NOTETABLEWIDGET_H
#define NOTETABLEWIDGET_H

#include "jitemtable.h"
#include "note.h"

class NoteTableWidget : public JTable
{
  Q_OBJECT

public:
  explicit NoteTableWidget(QWidget* parent = nullptr);
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

#endif // NOTETABLEWIDGET_H
