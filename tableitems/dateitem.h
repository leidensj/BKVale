#ifndef DATEITEM_H
#define DATEITEM_H

#include "jtableitem.h"

class DateItem : public JTableItem
{
public:
  enum class Color
  {
    None,
    DateBeforeDefault
  };

  DateItem(const QDate& defaultDate, Color color = Color::None);
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  QDate m_defaultDate;
  Color m_color;
};
#endif // DATEITEM_H
