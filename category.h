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
  qlonglong m_imageId;

  Category()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_name.clear();
    m_imageId = INVALID_ID;
  }

  bool operator != (const Category& other)
  {
    return m_name != other.m_name ||
           m_imageId != other.m_imageId;
  }

  static bool st_isValidId(qlonglong id) { return id != INVALID_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
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

struct FullCategory
{
  Category m_category;
  Image m_image;
  void clear()
  {
    m_category.clear();
    m_image.clear();
  }
  FullCategory()
  {
    clear();
  }
};

#endif // CATEGORY_H
