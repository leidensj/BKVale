#ifndef JTABLEWIDGETITEM_H
#define JTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <jitem.h>
#include <QVector>
#include "product.h"
#include "person.h"
#include "store.h"

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

class PackageTableWidgetItem : public QTableWidgetItem
{
public:
  explicit PackageTableWidgetItem();
  void setItem(const Package& o, const QString& productUnity);
  const Package& getItem() const;
  void selectItem(const QString& productUnity);
private:
  Package m_package;
};

class JItemTableWidgetItem : public QTableWidgetItem
{
public:
  explicit JItemTableWidgetItem();
  virtual void setItem(const JItem& o) = 0;
  virtual const JItem& getItem() const = 0;
  virtual void selectItem(const QString& fixedFilter) = 0;
};

class ProductTableWidgetItem : public JItemTableWidgetItem
{
public:
  void setItem(const JItem& o);
  const JItem& getItem() const;
  void selectItem(const QString& fixedFilter);
  void selectItemByBarcode(const QString& fixedFilter);

private:
  Product m_product;
};

class PersonTableWidgetItem : public JItemTableWidgetItem
{
public:
  void setItem(const JItem& o);
  const JItem& getItem() const;
  void selectItem(const QString& fixedFilter);

private:
  Person m_person;
};

class TimeIntervalsTableWidgetItem : public QTableWidgetItem
{
public:
  TimeIntervalsTableWidgetItem();
  void setItems(const QVector<TimeInterval>& v);
  const QVector<TimeInterval>& getItem() const;
  void selectItem();

private:
  QVector<TimeInterval> m_timeIntervals;
};

#endif // JTABLEWIDGETITEM_H
