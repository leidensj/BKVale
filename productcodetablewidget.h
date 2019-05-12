#ifndef PRODUCTCODETABLEWIDGET_H
#define PRODUCTCODETABLEWIDGET_H

#include "jtable.h"
#include "items/product.h"

class ProductCodeTableWidget : public JItemTable
{
  Q_OBJECT

  enum class Column
  {
    Code
  };

public:
  explicit ProductCodeTableWidget(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();

private:
  mutable ProductCode m_ref;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // PRODUCTCODETABLEWIDGET_H
