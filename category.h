#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "image.h"
#include "defines.h"
#include "jitem.h"

struct Category : public JItem
{
  QString m_name;
  Image m_image;

  void clear()
  {
    m_id.clear();
    m_name.clear();
    m_image.clear();
  }

  Category()
  {
    clear();
  }

  bool operator != (const JItem& other) const
  {
    const Category& another = dynamic_cast<const Category&>(other);
    return m_name != another.m_name ||
           m_image.m_id != another.m_image.m_id;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return !m_name.isEmpty();
  }

  QString strTableName() const
  {
    return CATEGORY_SQL_TABLE_NAME;
  }
};

#endif // CATEGORY_H
