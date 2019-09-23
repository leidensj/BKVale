#include "doubleitem.h"
#include "tinyexpr.h"

DoubleItem::DoubleItem(Data::Type type,
                       Color color,
                       bool bCheckable,
                       bool bAcceptNegative,
                       const QString& prefix,
                       const QString& sufix)
  : m_type(type)
  , m_color(color)
  , m_bCheckable(bCheckable)
  , m_bAcceptNegative(bAcceptNegative)
  , m_prefix(prefix)
  , m_sufix(sufix)
{
  setValue(0.0);
  if (m_bCheckable)
  {
    setFlags(flags() | Qt::ItemIsUserCheckable);
    setCheckState(Qt::Checked);
  }
}

void DoubleItem::setValue(const QVariant& v)
{
  double val = v.toDouble();
  setData(Qt::UserRole, val);
  setText(m_prefix + Data::str(val, m_type) + m_sufix);

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

void DoubleItem::evaluate()
{
  auto stdExp = text().toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  bool bValid = !error;
  if (bValid)
  {
    if (!m_bAcceptNegative && val < 0)
      bValid = false;
    else
      setValue(val);
  }

  if (!bValid)
    setValue(getValue());
}

void DoubleItem::erase()
{
  setValue(0.0);
}

void DoubleItem::activate()
{

}
