#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "jtablecolumn.h"
#include "defines.h"

struct Image
{
  mutable qlonglong m_id;
  QString m_name;
  QByteArray m_image;

  Image()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_name.clear();
    m_image.clear();
  }

  bool operator !=(const Image& img) const
  {
    return
        m_name != img.m_name ||
        m_image != img.m_image;
  }

  bool isValidId() const { return IS_VALID_ID(m_id); }

  static bool st_isValid(const Image& img) { return !img.m_name.isEmpty() && !img.m_image.isEmpty(); }
  bool isValid() const { return st_isValid(*this); }

  enum class Column
  {
    Id,
    Name,
    Image
  };

  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(IMAGE_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(IMAGE_SQL_COL01, QObject::tr("Imagem"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(IMAGE_SQL_COL02, QObject::tr("Imagem")));
    return c;
  }
};

#endif // IMAGE_H
