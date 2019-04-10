#ifndef REMINDER_H
#define REMINDER_H

#include <QString>
#include <QObject>
#include "jitemsql.h"

struct Reminder : public JItemSQL
{
  enum class Capitalization : int
  {
    Normal,
    AllUppercase,
    AllLowercase
  };

  enum class Size : int
  {
    Normal,
    Large
  };

  Reminder();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject* parent) const;

  QString m_title;
  QString m_message;
  bool m_bFavorite;
  Size m_size;
  Capitalization m_capitalization;
  bool m_bBarcodeHRI;
  QString m_barcode;
};

#endif // REMINDER_H
