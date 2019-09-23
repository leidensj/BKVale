#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "jtableitem.h"
#include "common.h"

class TextItem : public JTableItem
{
public:
  TextItem(Text::Input input, bool toUpper = true);
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  Text::Input m_input;
  bool m_toUpper;
};

#endif // TEXTITEM_H
