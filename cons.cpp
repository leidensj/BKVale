#include "cons.h"
#include <QStringList>

const QChar PromissoryNote::st_separator = ';';

PromissoryNote::PromissoryNote()
{
  clear();
}

void PromissoryNote::clear()
{
  m_id = -1;
  m_number = 0;
  m_supplier.clear();
  m_tableContent.clear();
  m_total.clear();
}

QString PromissoryNote::serializeItems() const
{
  QString str;
  for (size_t i = 0; i != m_tableContent.size(); ++i)
  {
    for (size_t j = 0; j != m_tableContent.size(); ++j)
    {
      str += m_tableContent.at(i).at(j);
      str += st_separator;
    }
  }
  return str;
}

void PromissoryNote::deserializeItems(const QString& str)
{
  m_tableContent.clear();
  QStringList list = str.split(st_separator);
  if (list.size() % NUMBER_OF_COLUMNS == 0)
  {
    m_tableContent.reserve(list.size() / NUMBER_OF_COLUMNS);
    for (int i = 0; i < list.size(); i += NUMBER_OF_COLUMNS)
    {
      std::vector<QString> v;
      v.reserve(NUMBER_OF_COLUMNS);
      v.push_back(list.at(i + (int)Column::Ammount));
      v.push_back(list.at(i + (int)Column::UnitValue));
      v.push_back(list.at(i + (int)Column::Unity));
      v.push_back(list.at(i + (int)Column::Description));
      v.push_back(list.at(i + (int)Column::SubTotal));
      m_tableContent.push_back(v);
    }
  }
}
