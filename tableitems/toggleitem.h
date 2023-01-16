#ifndef TOGGLEITEM_H
#define TOGGLEITEM_H

#include "jtableitem.h"
#include "common.h"

class ToggleItem : public JTableItem
{
public:
  ToggleItem(const QString& text = "", const QColor& foreground = Qt::black, const QColor& background = QColor(200, 200, 255));
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  const QString m_text;
  const QColor m_foreground;
  const QColor m_background;
};

#endif // TOGGLEITEM_H
