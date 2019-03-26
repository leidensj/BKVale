#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <QVector>
#include "user.h"
#include "note.h"
#include "product.h"
#include "reminder.h"
#include "settings.h"
#include "category.h"
#include "image.h"
#include "person.h"
#include "phone.h"
#include "address.h"
#include "shoppinglist.h"
#include "reservation.h"
#include "discount.h"
#include "store.h"

class UserLoginSQL
{
public:
  UserLoginSQL();

  bool login(const QString& strUser,
             const QString& strPassword,
             QString& error);

private:
  User m_user;

public:
  Id getId() const { return m_user.m_id; }
  bool isValid() const { return m_user.m_id.isValid(); }
  QString strUser() const { return m_user.m_strUser; }
  bool hasAccessToNote() const { return isValid() && m_user.m_bAccessNote; }
  bool hasAccessToReminder() const { return isValid() && m_user.m_bAccessReminder; }
  bool hasAccessToCalculator() const { return isValid() && m_user.m_bAccessCalculator; }
  bool hasAccessToShop() const { return isValid() && m_user.m_bAccessShop; }
  bool hasAccessToUsers() const { return isValid() && m_user.m_bAccessUser; }
  bool hasAccessToProducts() const { return isValid() && m_user.m_bAccessProduct; }
  bool hasAccessToSettings() const { return isValid() && m_user.m_bAccessSettings; }
  bool hasAccessToPersons() const { return isValid() && m_user.m_bAccessPerson; }
  bool hasAccessToEmployee() const { return isValid() && m_user.m_bAccessEmployee; }
  bool hasAccessToSupplier() const { return isValid() && m_user.m_bAccessSupplier; }
  bool hasAccessToCategories() const { return isValid() && m_user.m_bAccessCategory; }
  bool hasAccessToImages() const { return isValid() && m_user.m_bAccessImage; }
  bool hasAccessToReservations() const { return isValid() && m_user.m_bAccessReservation; }
  bool hasAccessToShoppingLists() const { return isValid() && m_user.m_bAccessShoppingList; }
};

namespace BaitaSQL
{
  bool isOpen(QString& error);

  bool open(const QString& hostName,
            int port,
            QString& error);

  void close();

  bool createTables(QString& error);

  bool init(const QString& hostName,
            int port,
            QString& error);

  QDate getDate(bool dfltMax);
}

namespace UserSQL
{
  bool insert(const User& user,
              QString& error);

  bool update(const User& user,
              QString& error);

  bool select(User& user,
              QString& error);

  bool remove(Id id,
              QString& error);
}

namespace ShoppingListSQL
{
 bool execSelect(ShoppingList& shoppingList,
                 QString& error);

  bool select(ShoppingList& shoppingList,
              QString& error);

  bool insert(const ShoppingList& shoppingList,
              QString& error);

  bool update(const ShoppingList& shoppingList,
              QString& error);

  bool remove(Id id,
              QString& error);
}

namespace ActiveUserSQL
{
  bool refresh(QString& error);

  bool execRefresh(QSqlQuery& query,
                   QString& error);

  bool remove(QString& error);

  bool execRemove(QSqlQuery& query,
                  QString& error);
}


#endif // DATABASEUTILS_H
