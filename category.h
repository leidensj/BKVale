#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "image.h"
#include "jtablecolumn.h"

#define SQL_CATEGORY_TABLE_NAME "_CATEGORIES"
#define SQL_CATEGORY_COL00 "_ID"
#define SQL_CATEGORY_COL01 "_IMAGEID"
#define SQL_CATEGORY_COL02 "_NAME"

#define INVALID_CATEGORY_ID   -1

#define MAX_CATEGORY_NAME_LENGTH 35

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
    m_id = INVALID_CATEGORY_ID;
    m_name.clear();
    m_imageId = INVALID_IMAGE_ID;
  }

  bool operator != (const Category& other)
  {
    return m_name != other.m_name ||
           m_imageId != other.m_imageId;
  }

  static bool st_isValidId(qlonglong id) { return id != INVALID_CATEGORY_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static bool st_isValid(const Category& category) { return !category.m_name.isEmpty(); }
  bool isValid() const { return st_isValid(*this); }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(SQL_CATEGORY_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(SQL_CATEGORY_COL01, QObject::tr("Id Imagem")));
    c.push_back(JTableColumn(SQL_CATEGORY_COL02, QObject::tr("Nome"), false, true, JResizeMode::Stretch));
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
