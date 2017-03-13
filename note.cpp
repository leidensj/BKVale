#include "note.h"
#include <QStringList>

Note::Note()
{
  clear();
}

void Note::clear()
{
  m_id = 0;
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
  : m_items(items.split(";"))
  , m_size(m_items.size() / NUMBER_OF_COLUMNS)
{
  Q_ASSERT(m_items.size() % NUMBER_OF_COLUMNS == 0);
}

QString NoteItems::at(int row, Column column) const
{
  QString str;
  const int idx = (row * NUMBER_OF_COLUMNS) + (int)column;
  if (idx < m_items.size())
    str = m_items.at(idx);
  return str;
}
