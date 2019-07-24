#ifndef REMINDER_H
#define REMINDER_H

#include <QString>
#include <QObject>
#include <QDate>
#include <QTime>
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
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  static QStringList SQL_select_subjects();

  QString m_title;
  QString m_message;
  bool m_bFavorite;
  Size m_size;
  Capitalization m_capitalization;
  bool m_bBarcodeHRI;
  QString m_barcode;
  QDate m_date;
  bool m_bDate;
  QTime m_time;
  bool m_bTime;
  QString m_subject;
};

#endif // REMINDER_H
