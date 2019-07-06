#ifndef PRODUCTCODETABLE_H
#define PRODUCTCODETABLE_H

#include "widgets/jtable.h"
#include "items/product.h"

class ProductCodeTable : public JItemTable
{
  Q_OBJECT

  enum class Column
  {
    Code
  };

public:
  explicit ProductCodeTable(QWidget* parent = nullptr);
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

#endif // PRODUCTCODETABLE_H
