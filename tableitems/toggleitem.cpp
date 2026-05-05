#include "toggleitem.h"

ToggleItem::ToggleItem(const QString& text, const QColor& foreground, const QColor& background)
  : m_text(text)
  , m_foreground(foreground)
  , m_background(background)
{
  setForeground(QBrush(QColor(m_foreground)));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  ToggleItem::setValue(false);
}

void ToggleItem::setValue(const QVariant& v)
{
  bool b = v.toBool();
  setData(Qt::UserRole, b);
  setText(b ? m_text : "");
  setBackground(b ? QBrush(m_background) : QBrush());
}

void ToggleItem::evaluate()
{

}

void ToggleItem::activate()
{
  bool b = data(Qt::UserRole).toBool();
  setValue(!b);
}

void ToggleItem::erase()
{
  setValue(false);
}
