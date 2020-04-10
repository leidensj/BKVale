#include "dateitem.h"

DateItem::DateItem(const QDate& defaultDate, Color color)
  : m_defaultDate(defaultDate)
  , m_color(color)
{
  setValue(m_defaultDate);
}

void DateItem::setValue(const QVariant& v)
{
  QDate dt = v.toDate();
  setData(Qt::UserRole, dt);
  setText(dt.toString("dd/MM/yyyy"));

  switch (m_color)
  {
    case Color::DateBeforeDefault:
      setBackground(QBrush(dt < m_defaultDate ? QColor(255, 200, 200) : QColor(Qt::white)));
      setToolTip(dt < m_defaultDate ? "A data é anterior a " + m_defaultDate.toString("dd/MM/yyyy") : "");
      break;
    case Color::None:
    default:
      break;
  }
}

void DateItem::evaluate()
{

  QDate dt = QDate::fromString(text(), "dd/MM/yyyy");
  if (!dt.isValid())
  {
    dt = QDate::fromString(text(), "dd/MM/yy");
    if (!dt.isValid())
    {
      dt = QDate::fromString(text(), "ddMMyyyy");
      if (!dt.isValid())
      {
        dt = QDate::fromString(text(), "ddMM");
        dt.setDate(QDate::currentDate().year(), dt.month(), dt.day());
        if (!dt.isValid())
        {
          dt = QDate::fromString(text(), "dd");
          dt.setDate(QDate::currentDate().year(), QDate::currentDate().month(), dt.day());
        }
      }
    }
  }

  if (dt.isValid())
    setValue(dt);
  else
    setValue(getValue());
}

void DateItem::erase()
{
  setValue(m_defaultDate);
}

void DateItem::activate()
{

}
