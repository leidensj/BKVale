#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include <QRegExp>
#include <QVariant>

class Id
{
private:
  qlonglong m_id;

public:
  static bool st_isValid(qlonglong id) { return id != INVALID_ID; }
  static QString st_str(qlonglong id) { return QString::number(id); }
  bool isValid() const { return st_isValid(m_id); }
  void clear() { m_id = INVALID_ID; }
  void set(qlonglong id) { m_id = id > INVALID_ID ? id: INVALID_ID; }
  QString str() const { return st_str(m_id); }
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
  virtual QString strTableName() const = 0;
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

/*QString getItemName(const QString& tableName)
{
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return "Endereço";
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
    return "Categoria";
  else if (tableName == IMAGE_SQL_TABLE_NAME)
    return "Imagem";
  else if (tableName == NOTE_SQL_TABLE_NAME)
    return "Vale";
  else if (tableName == NOTE_ITEMS_SQL_TABLE_NAME)
    return "Item de Vale";
  else if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return "Funcionário";
  else if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return "Fornecedor";
  else if (tableName == PERSON_SQL_TABLE_NAME)
    return "Pessoa";
  else if (tableName == PHONE_SQL_TABLE_NAME)
    return "Telefone";
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
    return "Produto";
  else if (tableName == PRODUCT_CODE_ITEMS_SQL_TABLE_NAME)
    return "Código";
  else if (tableName == USER_SQL_TABLE_NAME)
    return "Usuário";
  else if (tableName == REMINDER_SQL_TABLE_NAME)
    return "Lembrete";
  else if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return "Lista de Compras";
  else if (tableName == SHOPPING_LIST_ITEMS_SQL_TABLE_NAME)
    return "Item de Lista de Compras";
  else if (tableName == RESERVATION_SQL_TABLE_NAME)
    return "Reserva";
  else if (tableName == ACTIVE_USERS_SQL_TABLE_NAME)
    return "Usuário Ativo";
  else if (tableName == DISCOUNT_SQL_TABLE_NAME)
    return "Desconto";
  else if (tableName == DISCOUNT_ITEMS_SQL_TABLE_NAME)
    return "Item de Desconto";
  else if (tableName == STORE_SQL_TABLE_NAME)
    return "Loja";
  else if (tableName == STORE_EMPLOYEES_SQL_TABLE_NAME)
    return "Funcionário da Loja";
  else if (tableName == STORE_EMPLOYEE_HOURS_SQL_TABLE_NAME)
    return "Horário de Funcionário da Loja";
  else
    return "";
}

QString getItemIconPath(const QString& tableName)
{
  return "";
}*/

#endif // JITEM_H
