#ifndef ADDRESS_H
#define ADDRESS_H

#include "defines.h"
#include "jitem.h"
#include <QObject>
#include <QString>

struct Address : public SQL_JItem
{
  enum class EBRState : int
  {
    AC, AL, AP, AM, BA, CE, DF,
    ES, GO, MA, MT, MS, MG, PA,
    PB, PR, PE, PI, RJ, RN, RS,
    RO, RR, SC, SP, SE, TO
  };

  struct BRState
  {
    QString m_abv;
    QString m_name;
    QString m_mask;
  };

  Address();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  QString getFormattedAddress() const;
  QString getFormattedAddress2() const;
  QString getFormattedAddress3() const;

  QString m_cep;
  QString m_neighborhood;
  QString m_street;
  int m_number;
  QString m_city;
  EBRState m_state;
  QString m_complement;
  QString m_reference;

  static EBRState st_getEBRState(const QString& abv);
  static BRState st_getBRState(EBRState s);
  BRState getBRState() const;
};

#endif // ADDRESS_H
