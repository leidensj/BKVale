#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include "image.h"
#include "jitemsql.h"

struct Category : public JItemSQL
{
  Category();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  QString m_name;
  Image m_image;

  QString text() const { return "Categoria"; }
  QString icon() const { return ":/icons/res/category.png"; }
  QString name() const { return m_name; }
  QByteArray image() const { return m_image.m_image; }
};

#endif // CATEGORY_H
