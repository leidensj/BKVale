#ifndef FORM_H
#define FORM_H

#include "image.h"
#include "phone.h"
#include "address.h"
#include "defines.h"
#include "jitemsql.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QDate>

struct Form : public JItemSQL
{
  Form();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strAliasName() const;

  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  JModel* SQL_table_model(QObject* parent) const;

  QString text() const { return "Perfil"; }
  QString icon() const { return ":/icons/res/resume.png"; }
  QString name() const { return strAliasName(); }
  QByteArray image() const { return m_image.m_image; }

  Image m_image;
  QString m_name;
  QString m_alias;
  QString m_email;
  QString m_CPF_CNPJ;
  QString m_RG_IE;
  QString m_details;
  bool m_bBirth;
  QDate m_dtBirth;
  QDate m_dtCreation;
  bool m_bCompany;
  QVector<Phone> m_vPhone;
  QVector<Address> m_vAddress;
};

#endif
