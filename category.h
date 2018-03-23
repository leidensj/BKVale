#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "image.h"
#include "jtablecolumn.h"
#include "defines.h"
#include "jitem.h"

struct Category : public JItem
{
  QString m_name;
  Image m_image;

  void clear()
  {
    m_id = INVALID_ID;
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

  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(CATEGORY_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(CATEGORY_SQL_COL01, QObject::tr("Id Imagem")));
    c.push_back(JTableColumn(CATEGORY_SQL_COL02, QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    return c;
  }
};

#endif // CATEGORY_H
