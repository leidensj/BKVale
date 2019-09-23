#include "timeitem.h"

TimeItem::TimeItem(const QTime& defaultTime)
  : m_defaultTime(defaultTime)
{
  setToolTip(QObject::tr("Hora e minutos hh:mm"));
  setValue(m_defaultTime);
}

void TimeItem::setValue(const QVariant& v)
{
  QTime t = v.toTime();
  setData(Qt::UserRole, t);
  setText(t.toString("HH:mm"));
}

void TimeItem::evaluate()
{
  QTime t = QTime::fromString(text(), "HH:mm");
  if (!t.isValid())
    t = QTime::fromString(text(), "HHmm");

  if (t.isValid())
    setValue(t);
  else
    setValue(getValue());
}

void TimeItem::erase()
{
  setValue(m_defaultTime);
}

void TimeItem::activate()
{

}
