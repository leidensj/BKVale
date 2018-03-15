#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "image.h"
#include "jtablecolumn.h"
#include "defines.h"

struct Category
{
  mutable qlonglong m_id;
  QString m_name;
  Image m_image;

  Category()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_name.clear();
    m_image.clear();
  }

  bool operator != (const Category& other)
  {
    return m_name != other.m_name ||
           m_image != other.m_image;
  }

  bool isValidId() const { return IS_VALID_ID(m_id); }
  static bool st_isValid(const Category& category) { return !category.m_name.isEmpty(); }
  bool isValid() const { return st_isValid(*this); }
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
