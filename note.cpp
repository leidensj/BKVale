#include "note.h"

NoteItem::NoteItem()
{
  clear();
}

void NoteItem::clear()
{
  m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
  m_price = 0.0;
  m_package.clear();
}

bool NoteItem::isValid() const
{
  return true;
}

bool NoteItem::operator !=(const JItem& other) const
{
  const NoteItem& another = dynamic_cast<const NoteItem&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount ||
      m_price != another.m_price ||
      m_package != another.m_package;
}

bool NoteItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString NoteItem::strTableName() const
{
  return NOTE_ITEMS_SQL_TABLE_NAME;
}

double NoteItem::subtotal() const
{
  return m_ammount * m_price;
}

QString NoteItem::strSubtotal() const
{
  return st_strMoney(subtotal());
}

QString NoteItem::strAmmount() const
{
  return st_strAmmount(m_ammount);
}

QString NoteItem::strPrice() const
{
  return st_strMoney(m_price);
}

Note::Note()
{
  clear();
}

void Note::clear()
{
  m_id.clear();
  m_number = 0;
  m_supplier.clear();
  m_date = QDate::currentDate();
  m_bCash = false;
  m_vNoteItem.clear();
  m_observation.clear();
  m_disccount = 0.0;
}

bool Note::isValid() const
{
  return true;
}

bool Note::operator !=(const JItem& other) const
{
  const Note& another = dynamic_cast<const Note&>(other);
  return
      m_number != another.m_number ||
      m_date != another.m_date ||
      m_supplier.m_id != another.m_supplier.m_id ||
      m_bCash != another.m_bCash ||
      m_vNoteItem != another.m_vNoteItem ||
      m_disccount != another.m_disccount;
}

bool Note::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString Note::strTableName() const
{
  return NOTE_SQL_TABLE_NAME;
}

QString Note::strDate() const
{
  return m_date.toString("dd/MM/yyyy");
}

QString Note::strDayOfWeek() const
{
  return m_date.toString("dddd");
}

QString Note::strNumber() const
{
  return st_strInt(m_number);
}

QString Note::strDisccount() const
{
  return st_strMoney(m_disccount);
}

double Note::total() const
{
  double total = 0.0;
  for (int i = 0; i != m_vNoteItem.size(); ++i)
    total += m_vNoteItem.at(i).subtotal();
  return total + m_disccount;
}

double Note::subTotal() const
{
  double subTotal = 0.0;
  for (int i = 0; i != m_vNoteItem.size(); ++i)
    subTotal += m_vNoteItem.at(i).subtotal();
  return subTotal;
}

QString Note::strTotal() const
{
  return st_strMoney(total());
}

QString Note::strSubTotal() const
{
  return st_strMoney(subTotal());
}
