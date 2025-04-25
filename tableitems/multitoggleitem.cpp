#include "multitoggleitem.h"

MultiToggleItem::MultiToggleItem(const QStringList& ls, const QColor& foreground, const QColor& background)
  : m_sl(ls.size() == 0 ? QStringList() << "" : ls)
  , m_foreground(foreground)
  , m_background(background)
{
  setForeground(QBrush(QColor(m_foreground)));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  MultiToggleItem::setValue(false);
}

void MultiToggleItem::setValue(const QVariant& v)
{
  int i = v.toInt();
  if (i < 0)
    i = 0;
  setData(Qt::UserRole, i);
  setText(i < m_sl.size() ? m_sl.at(i) : "");
  setBackground(QBrush(QColor(m_background)));
}

void MultiToggleItem::evaluate()
{

}

void MultiToggleItem::activate()
{
  int i = data(Qt::UserRole).toInt() + 1;
  if (i >= m_sl.size())
    i = 0;
  setValue(i);
}

void MultiToggleItem::erase()
{
  setValue(0);
}
