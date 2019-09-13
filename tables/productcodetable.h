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
  void addRow();
  void getCodes(QVector& v) const;
  void setCodes(const QVector& v);
};

#endif // PRODUCTCODETABLE_H
