#ifndef INVENTORYEDITOR_H
#define INVENTORYEDITOR_H

#include <QWidget>

class InventoryEditor : public QWidget
{
  Q_OBJECT
public:
  explicit InventoryEditor(QWidget *parent = nullptr);

public slots:
  void refresh();

};

#endif // INVENTORYEDITOR_H
