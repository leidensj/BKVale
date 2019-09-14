#ifndef PRODUCTCODETABLE_H
#define PRODUCTCODETABLE_H

#include "widgets/jtable.h"
#include "items/product.h"

class ProductCodeTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Code
  };

public:
  explicit ProductCodeTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getCodes(QVector<ProductCode>& v) const;
  void setCodes(const QVector<ProductCode>& v);

public slots:
  void addRow();
};

#endif // PRODUCTCODETABLE_H
