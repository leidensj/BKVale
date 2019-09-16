#ifndef DOUBLEITEM_H
#define DOUBLEITEM_H

#include "jtableitem.h"
#include "common.h"

class DoubleItem : public JTableItem
{
public:
  enum class Color
  {
    None,
    Background,
    Foreground
  };

  DoubleItem(Data::Type type,
             Color color,
             bool bCheckable = false,
             bool bAcceptNegative = true,
             const QString& prefix = "",
             const QString& sufix = "");
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  const Data::Type m_type;
  const Color m_color;
  const bool m_bCheckable;
  const bool m_bAcceptNegative;
  const QString m_prefix;
  const QString m_sufix;
};

#endif // DOUBLEITEM_H
