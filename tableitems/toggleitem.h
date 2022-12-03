#ifndef TOGGLEITEM_H
#define TOGGLEITEM_H

#include "jtableitem.h"
#include "common.h"

class ToggleItem : public JTableItem
{
public:
  enum class Color
  {
    None,
    Background,
    Foreground
  };

  ToggleItem(Color color,
             const QString& text = "");
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  const Color m_color;
  const QString m_text;
};

#endif // TOGGLEITEM_H
