#include "note.h"
#include <QStringList>

NoteItem::NoteItem()
{
  clear();
}

void NoteItem::clear()
{
  m_id = INVALID_NOTE_ID;
  m_noteId = INVALID_NOTE_ID;
  m_ammount = 0.0;
  m_price = 0.0;
  m_unity.clear();
  m_description.clear();
}

void NoteItem::subtotal() const
{
  return m_ammount * m_price;
}

QString NoteItem::strSubtotal() const
{
  return QString::number(subtotal(), 'f', 2);
}

QString NoteItem::strAmmount() const
{
  return QString::number(m_ammount, 'f', 3);
}

QString NoteItem::strPrice() const
{
  return QString::number(m_price, 'f', 2);
}

Note::Note()
{
  clear();
}

Note::Note(int id,
           int number,
           qint64 date,
           QString supplier,
           QString items,
           double total)
        : m_id(id)
        , m_number(number)
        , m_date(date)
        , m_supplier(supplier)
        , m_items(items)
        , m_total(total)
{

}

void Note::clear()
{
  m_id = INVALID_NOTE_ID;
  m_number = 0;
  m_date = 0;
  m_supplier.clear();
  m_items.clear();
  m_total = 0.0;
}

QString Note::format(const QString& str, bool b3places /*= false*/)
{
  return format(str.toDouble(), b3places);
}

QString Note::format(double d, bool b3places /*= false*/)
{
  return QString::number(d, 'f', b3places ? 3 : 2);
}

NoteItems::NoteItems(const QString &items)
  : m_items(items.isEmpty() ? QStringList() : items.split(";"))
  , m_size(m_items.size() / NUMBER_OF_COLUMNS)
{
  Q_ASSERT(m_items.size() % NUMBER_OF_COLUMNS == 0);
}

QString NoteItems::at(int row, NoteColumn column) const
{
  QString str;
  const int idx = (row * NUMBER_OF_COLUMNS) + (int)column;
  if (idx < m_items.size())
    str = m_items.at(idx);
  return str;
}
