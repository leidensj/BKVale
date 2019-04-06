#include "user.h"
#include "jmodel.h"
#include <QCryptographicHash>
#include <QObject>

class UserModel : public JModel
{
public:
  UserModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     USER_SQL_COL01
                     " FROM "
                     USER_SQL_TABLE_NAME);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    if (header != nullptr && header->count() == 2)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    }
  }
};

User::User()
{
  clear();
}

void User::clear()
{
  m_id = INVALID_ID;
  m_strUser.clear();
  m_bAccessNote = false;
  m_bAccessReminder = false;
  m_bAccessCalculator = false;
  m_bAccessShop = false;
  m_bAccessUser = false;
  m_bAccessProduct = false;
  m_bAccessSettings = false;
  m_bAccessForm = false;
  m_bAccessEmployee = false;
  m_bAccessSupplier = false;
  m_bAccessCategory = false;
  m_bAccessImage = false;
  m_bAccessReservation = false;
  m_bAccessShoppingList = false;
  m_password.clear();
}

QString User::st_strEncryptedPassword(const QString& strPassword)
{
  return QString(QCryptographicHash::hash(strPassword.toUtf8(),
                                          QCryptographicHash::Md5));
}

QString User::strEncryptedPassword() const
{
  return st_strEncryptedPassword(m_password);
}

bool User::operator != (const JItem& other) const
{
  const User& another = dynamic_cast<const User&>(other);
  return
      m_strUser != another.m_strUser ||
      m_bAccessNote != another.m_bAccessNote ||
      m_bAccessReminder != another.m_bAccessReminder ||
      m_bAccessCalculator != another.m_bAccessCalculator ||
      m_bAccessShop != another.m_bAccessCalculator ||
      m_bAccessForm != another.m_bAccessForm ||
      m_bAccessEmployee != another.m_bAccessEmployee ||
      m_bAccessSupplier != another.m_bAccessSupplier ||
      m_bAccessUser != another.m_bAccessUser ||
      m_bAccessProduct != another.m_bAccessProduct ||
      m_bAccessSettings != another.m_bAccessSettings ||
      m_bAccessReservation != another.m_bAccessReservation ||
      m_bAccessCategory != another.m_bAccessCategory ||
      m_bAccessImage != another.m_bAccessImage ||
      m_bAccessShoppingList != another.m_bAccessShoppingList;
}

bool User::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool User::isValid() const
{
  return m_strUser.length() > 4;
}

QString User::SQL_tableName() const
{
  return USER_SQL_TABLE_NAME;
}

bool User::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " USER_SQL_TABLE_NAME " ("
                USER_SQL_COL01 ","
                USER_SQL_COL02 ","
                USER_SQL_COL03 ","
                USER_SQL_COL04 ","
                USER_SQL_COL05 ","
                USER_SQL_COL06 ","
                USER_SQL_COL07 ","
                USER_SQL_COL08 ","
                USER_SQL_COL09 ","
                USER_SQL_COL10 ","
                USER_SQL_COL11 ","
                USER_SQL_COL12 ","
                USER_SQL_COL13 ","
                USER_SQL_COL14 ","
                USER_SQL_COL15 ","
                USER_SQL_COL16 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07),"
                "(:_v08),"
                "(:_v09),"
                "(:_v10),"
                "(:_v11),"
                "(:_v12),"
                "(:_v13),"
                "(:_v14),"
                "(:_v15),"
                "(:_v16))");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", m_bAccessNote);
  query.bindValue(":_v04", m_bAccessReminder);
  query.bindValue(":_v05", m_bAccessCalculator);
  query.bindValue(":_v06", m_bAccessShop);
  query.bindValue(":_v07", m_bAccessUser);
  query.bindValue(":_v08", m_bAccessProduct);
  query.bindValue(":_v09", m_bAccessSettings);
  query.bindValue(":_v10", m_bAccessForm);
  query.bindValue(":_v11", m_bAccessCategory);
  query.bindValue(":_v12", m_bAccessImage);
  query.bindValue(":_v13", m_bAccessReservation);
  query.bindValue(":_v14", m_bAccessShoppingList);
  query.bindValue(":_v15", m_bAccessEmployee);
  query.bindValue(":_v16", m_bAccessSupplier);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool User::SQL_update_proc(QSqlQuery& query) const
{
  QString strQuery("UPDATE " USER_SQL_TABLE_NAME " SET "
                   USER_SQL_COL01 " = (:_v01),");
  if (!m_password.isEmpty())
    strQuery += USER_SQL_COL02 " = (:_v02),";
  strQuery += USER_SQL_COL03" = (:_v03),"
              USER_SQL_COL04" = (:_v04),"
              USER_SQL_COL05" = (:_v05),"
              USER_SQL_COL06" = (:_v06),"
              USER_SQL_COL07" = (:_v07),"
              USER_SQL_COL08" = (:_v08),"
              USER_SQL_COL09" = (:_v09),"
              USER_SQL_COL10" = (:_v10),"
              USER_SQL_COL11" = (:_v11),"
              USER_SQL_COL12" = (:_v12),"
              USER_SQL_COL13" = (:_v13),"
              USER_SQL_COL14" = (:_v14),"
              USER_SQL_COL15" = (:_v15),"
              USER_SQL_COL16" = (:_v16)"
              " WHERE " SQL_COLID " = (:_v00)";

  query.prepare(strQuery);
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_strUser);
  if (!m_password.isEmpty())
    query.bindValue(":_v02", strEncryptedPassword());
  query.bindValue(":_v03", m_bAccessNote);
  query.bindValue(":_v04", m_bAccessReminder);
  query.bindValue(":_v05", m_bAccessCalculator);
  query.bindValue(":_v06", m_bAccessShop);
  query.bindValue(":_v07", m_bAccessUser);
  query.bindValue(":_v08", m_bAccessProduct);
  query.bindValue(":_v09", m_bAccessSettings);
  query.bindValue(":_v10", m_bAccessForm);
  query.bindValue(":_v11", m_bAccessCategory);
  query.bindValue(":_v12", m_bAccessImage);
  query.bindValue(":_v13", m_bAccessReservation);
  query.bindValue(":_v14", m_bAccessShoppingList);
  query.bindValue(":_v15", m_bAccessEmployee);
  query.bindValue(":_v16", m_bAccessSupplier);

  return query.exec();
}

bool User::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                USER_SQL_COL01 ","
                USER_SQL_COL02 ","
                USER_SQL_COL03 ","
                USER_SQL_COL04 ","
                USER_SQL_COL05 ","
                USER_SQL_COL06 ","
                USER_SQL_COL07 ","
                USER_SQL_COL08 ","
                USER_SQL_COL09 ","
                USER_SQL_COL10 ","
                USER_SQL_COL11 ","
                USER_SQL_COL12 ","
                USER_SQL_COL13 ","
                USER_SQL_COL14 ","
                USER_SQL_COL15 ","
                USER_SQL_COL16
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_strUser = query.value(0).toString();
      query.value(1).toString(); // password nao precisamos
      m_bAccessNote = query.value(2).toBool();
      m_bAccessReminder = query.value(3).toBool();
      m_bAccessCalculator = query.value(4).toBool();
      m_bAccessShop = query.value(5).toBool();
      m_bAccessUser = query.value(6).toBool();
      m_bAccessProduct = query.value(7).toBool();
      m_bAccessSettings = query.value(8).toBool();
      m_bAccessForm = query.value(9).toBool();
      m_bAccessCategory = query.value(10).toBool();
      m_bAccessImage = query.value(11).toBool();
      m_bAccessReservation = query.value(12).toBool();
      m_bAccessShoppingList = query.value(13).toBool();
      m_bAccessEmployee = query.value(14).toBool();
      m_bAccessSupplier = query.value(15).toBool();
    }
    else
    {
      bSuccess = false;
      error = "Usuário não encontrado.";
    }
  }

  return bSuccess;
}

bool User::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " USER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

JModel* User::SQL_table_model(QObject* parent) const
{
  return new UserModel(parent);
}

bool User::SQL_select_password_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SQL_COLID
                " FROM " USER_SQL_TABLE_NAME
                " WHERE " USER_SQL_COL01 " = (:_v01) AND "
                USER_SQL_COL02 " = (:_v02) LIMIT 1");
  query.bindValue(":_v01", m_strUser);
  query.bindValue(":_v02", strEncryptedPassword());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
      bSuccess = SQL_select_proc(query, error);
    }
    else
    {
      error = "Usuário ou senha inválidos.";
      bSuccess = false;
    }
  }
  return bSuccess;
}
