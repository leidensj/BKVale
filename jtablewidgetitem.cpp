#include "jtablewidgetitem.h"
#include "tinyexpr.h"
#include "product.h"

DoubleTableWidgetItem::DoubleTableWidgetItem(JItem::DataType type, Color color, bool bCheckable)
  : m_type(type)
  , m_color(color)
  , m_bCheckable(bCheckable)
{
  if (m_bCheckable)
    setFlags(flags() | Qt::ItemIsUserCheckable);
}

void DoubleTableWidgetItem::setValue(double val)
{
  setData(Qt::UserRole, val);
  setText(JItem::st_str(val, m_type));

  if (m_bCheckable)
  {
    if (checkState() == Qt::Unchecked)
      setFlags(flags() & ~Qt::ItemIsEditable);
    else
      setFlags(flags() | Qt::ItemIsEditable);
  }

  switch (m_color)
  {
    case Color::Background:
      setBackgroundColor(val == 0.0 ? QColor(255, 200, 200) : QColor(Qt::white));
      break;
    case Color::Foreground:
      setTextColor(QColor(val >= 0 ? Qt::red : Qt::darkGreen));
      break;
    case Color::None:
    default:
      break;
  }
}

double DoubleTableWidgetItem::getValue()
{
  return data(Qt::UserRole).toDouble();
}

bool DoubleTableWidgetItem::evaluate(const QString& exp)
{
  auto stdExp = exp.toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  if (!error)
    setValue(val);
  return !error;
}

void DoubleTableWidgetItem::evaluate()
{
  if (!evaluate(text()))
    setValue(getValue());
}

void ProductTableWidgetItem::setItem(const JItem& o)
{
  m_product = o;
  setText(m_product.m_name);
}

const JItem& ProductTableWidgetItem::getItem() const
{
  return m_product;
}
