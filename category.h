#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "image.h"
#include "jtablecolumn.h"

#define INVALID_CATEGORY_ID   -1

#define MAX_CATEGORY_NAME_LENGTH 35

struct Category
{
  mutable int m_id;
  QString m_name;
  int m_imageId;

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
    return m_name != other.m_name &&
           m_imageId != other.m_imageId;
  }

  static bool st_isValidId(int id) { return id != INVALID_CATEGORY_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static bool st_isValid(const Category& category) { return !category.m_name.isEmpty(); }
  bool isValid() const { return st_isValid(*this); }
  static QString getTableName() { return "_CATEGORIES"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_NAME", QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn("_IMAGEID", QObject::tr("Imagem")));
    return c;
  }
};

#endif // CATEGORY_H
