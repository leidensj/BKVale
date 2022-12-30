#include "toggleitem.h"

ToggleItem::ToggleItem(Color color,
                       const QString& text)
  : m_color(color)
  , m_text(text)
{
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  ToggleItem::setValue(false);
}

void ToggleItem::setValue(const QVariant& v)
{
  bool b = v.toBool();
  setData(Qt::UserRole, b);
  setText(b ? m_text : "");

  switch (m_color)
  {
    case Color::Background:
      setBackground(QBrush(QColor(b ? QColor(200, 200, 255) : QColor(Qt::white))));
      break;
    case Color::Foreground:
      setForeground(QBrush(QColor(b ? Qt::blue : Qt::white)));
      break;
    case Color::None:
    default:
      break;
  }
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
