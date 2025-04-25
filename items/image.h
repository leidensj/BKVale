#ifndef IMAGE_H
#define IMAGE_H

#include <QByteArray>
#include <QIcon>
#include "defines.h"
#include "jitemsql.h"

struct Image : public JItemSQL
{
  Image();
  void clear(bool bClearId = true);
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  QString m_name;
  QByteArray m_image;

  void copy(const JItemSQL& other);
  QString name() const { return m_name; }
  QByteArray image() const { return m_image; }
  QIcon toIcon() const;
};

#endif // IMAGE_H
