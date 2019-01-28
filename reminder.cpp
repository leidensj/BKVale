#include "reminder.h"

Reminder::Reminder()
{
  clear();
}

void Reminder::clear()
{
  m_id.clear();
  m_title.clear();
  m_message.clear();
  m_bFavorite = false;
  m_size = Size::Large;
  m_capitalization = Capitalization::AllUppercase;
  m_bBarcodeHRI = true;
  m_barcode.clear();
}

bool Reminder::operator != (const JItem& other) const
{
  const Reminder& another = dynamic_cast<const Reminder&>(other);
  return m_title != another.m_title ||
         m_message != another.m_message ||
         m_bFavorite != another.m_bFavorite ||
         m_size != another.m_size ||
         m_capitalization != another.m_capitalization ||
         m_bBarcodeHRI != another.m_bBarcodeHRI ||
         m_barcode != another.m_barcode;
}

bool Reminder::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Reminder::isValid() const
{
  return !m_title.isEmpty() || !m_message.isEmpty() || !m_barcode.isEmpty();
}

QString Reminder::strTableName() const
{
  return REMINDER_SQL_TABLE_NAME;
}
