#ifndef JTABLEWIDGETITEM_H
#define JTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <jitem.h>
#include "product.h"

class DoubleTableWidgetItem : public QTableWidgetItem
{
public:
  enum class Color
  {
    None,
    Background,
    Foreground
  };

  DoubleTableWidgetItem(JItem::DataType type, Color color, bool bCheckable = false);
  bool evaluate(const QString& exp);
  void evaluate();
  void setValue(double val);
  double getValue();

private:
  const JItem::DataType m_type;
  const Color m_color;
  const bool m_bCheckable;
};

class JItemTableWidgetItem : protected QTableWidgetItem
{
public:
  virtual void setItem(const JItem& o) = 0;
  virtual const JItem& getItem() const = 0;
};

class ProductTableWidgetItem : public JItemTableWidgetItem
{
public:
  void setItem(const JItem& o);
  const JItem& getItem() const;
private:
  Product m_product;
};

#endif // JTABLEWIDGETITEM_H
