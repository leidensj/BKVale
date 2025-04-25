#ifndef MULTITOGGLEITEM_H
#define MULTITOGGLEITEM_H

#include "jtableitem.h"
#include "common.h"

class MultiToggleItem : public JTableItem
{
public:
  MultiToggleItem(const QStringList& ls, const QColor& foreground = Qt::black, const QColor&  = Qt::white);
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  const QStringList m_sl;
  const QColor m_foreground;
  const QColor m_background;
};

#endif // MULTITOGGLEITEM_H
