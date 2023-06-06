#ifndef INVENTORYVIEW_H
#define INVENTORYVIEW_H

#include "items/inventory.h"
#include "jitemview.h"

class DatabasePicker;
class JAddRemoveButtons;
class InventoryTable;

class InventoryView : public JItemView
{
  Q_OBJECT

public:
  explicit InventoryView(QWidget* parent = nullptr);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  DatabasePicker* m_storePicker;
  JAddRemoveButtons* m_btnAddRemove;
  InventoryTable* m_table;
};

#endif // INVENTORYVIEW_H
