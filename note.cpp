#include "note.h"
#include <QDate>

NoteItem::NoteItem()
{
  clear();
}

void NoteItem::clear()
{
  m_id = INVALID_NOTE_ID;
  m_ammount = 0.0;
  m_price = 0.0;
  m_unity.clear();
  m_description.clear();
}

Note::Note()
{
  clear();
}

void Note::clear()
{
  m_id = INVALID_NOTE_ID;
  m_date = 0;
  m_supplier.clear();
  m_items.clear();
  m_total = 0.0;
  m_bCash = false;
}

QString Note::strDate() const
{
  return QDate::fromJulianDay(m_date).toString("dd/MM/yyyy");
}

QString Note::strDayOfWeek() const
{
  return QDate::fromJulianDay(m_date).toString("dddd");
}
