#include "image.h"

Image::Image()
{
  clear();
}

void Image::clear()
{
  m_id.clear();
  m_name.clear();
  m_image.clear();
}

bool Image::operator !=(const JItem& other) const
{
  const Image& another = dynamic_cast<const Image&>(other);
  return
      m_name != another.m_name ||
      m_image != another.m_image;
}

bool Image::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Image::isValid() const
{
  return !m_name.isEmpty() && !m_image.isEmpty();
}

QString Image::strTableName() const
{
  return IMAGE_SQL_TABLE_NAME;
}
