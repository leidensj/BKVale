#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include <QRegExp>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlDatabase>

class Id
{
private:
  qlonglong m_id;

public:
  bool isValid() const { return m_id != INVALID_ID; }
  void clear() { m_id = INVALID_ID; }
  void set(qlonglong id) { m_id = id > INVALID_ID ? id: INVALID_ID; }
  QString str() const { return QString::number(m_id); }
  qlonglong get() const { return m_id; }
  QVariant getIdNull() const { return isValid() ? m_id : QVariant(QVariant::LongLong); }
  Id() { clear(); }
  Id(qlonglong id) : m_id(id) { }
  bool operator ==(const Id& other) const { return m_id == other.m_id; }
  bool operator !=(const Id& other) const { return !(*this == other); }
};

struct JItem
{
  enum class DataType
  {
    Integer,
    Money,
    Ammount,
    Fmt,
    Percentage
  };

  mutable Id m_id;
  virtual ~JItem() { }
  virtual bool isValid() const = 0;
  virtual void clear() = 0;
  virtual bool operator ==(const JItem& other) const = 0;
  virtual bool operator !=(const JItem& other) const = 0;

  static QString st_strMoney(double value) { return "R$" + QString::number(value, 'f', 2); }
  static QString st_strAmmount(double value) { return QString::number(value, 'f', 3); }
  static QString st_strFmt(double value) { return QString::number(value, 'f').remove(QRegExp("\\.?0*$")); }
  static QString st_strInt(double value) { return QString::number((int)value); }
  static QString st_strPercentage(double value) { return QString::number(value, 'f', 2) + "%"; }

  static QString st_str(double value, DataType type)
  {
    switch (type)
    {
      case DataType::Ammount:
        return st_strAmmount(value);
      case DataType::Fmt:
        return st_strFmt(value);
      case DataType::Integer:
        return st_strInt(value);
      case DataType::Money:
        return st_strMoney(value);
      case DataType::Percentage:
        return st_strPercentage(value);
      default:
        return QString::number(value);
    }
  }
};


struct SQL_JItem : public JItem
{
  virtual ~SQL_JItem() { }

  virtual QString SQL_tableName() const = 0;
  virtual bool SQL_insert(QString& error) = 0;
  virtual bool SQL_update(QString& error) = 0;
  virtual bool SQL_select(QString& error) = 0;
  virtual bool SQL_remove(QString& error) = 0;

  virtual bool SQL_insert_proc(QSqlQuery& query) = 0;
  virtual bool SQL_update_proc(QSqlQuery& query) = 0;
  virtual bool SQL_select_proc(QSqlQuery& query, QString& error) = 0;
  virtual bool SQL_remove_proc(QSqlQuery& query) = 0;

  static bool SQL_isOpen(QString& error)
  {
    QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
    error.clear();
    if (!db.isOpen())
       error = "Banco de dados não foi aberto.";
    return db.isOpen();
  }

  static bool SQL_finish(QSqlDatabase db, const QSqlQuery& query, bool bExecSelectResult, QString& error)
  {
    if (!bExecSelectResult)
    {
      if (error.isEmpty())
      {
        error = query.lastError().text();
        if (error.isEmpty())
          error = db.lastError().text();
      }
      db.rollback();
      return false;
    }
    else
      bExecSelectResult = db.commit();

    if (!bExecSelectResult && error.isEmpty())
    {
      error = query.lastError().text();
      if (error.isEmpty())
        error = db.lastError().text();
    }

    return bExecSelectResult;
  }
};

#endif // JITEM_H
