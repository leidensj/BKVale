#include "category.h"

Category::Category()
{
  clear();
}

void Category::clear()
{
  m_id.clear();
  m_name.clear();
  m_image.clear();
}

bool Category::operator != (const JItem& other) const
{
  const Category& another = dynamic_cast<const Category&>(other);
  return m_name != another.m_name ||
         m_image.m_id != another.m_image.m_id;
}

bool Category::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Category::isValid() const
{
  return !m_name.isEmpty();
}

QString Category::strTableName() const
{
  return CATEGORY_SQL_TABLE_NAME;
}
