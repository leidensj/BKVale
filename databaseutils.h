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
#include "productbarcode.h"

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
  qlonglong getId() const { return m_user.m_id; }
  bool isValid() const { return IS_VALID_ID(m_user.m_id); }
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
}

namespace UserSQL
{
  bool insert(const User& user,
              QString& error);

  bool update(const User& user,
              QString& error);

  bool select(User& user,
              QString& error);

  bool remove(qlonglong id,
              QString& error);
}

namespace NoteSQL
{
  bool insert(const Note& note,
              QString& error);

  bool update(const Note& note,
              QString& error);

  bool select(Note& note,
              QString& error);

  bool remove(qlonglong id,
              QString& error);

  NoteItem selectLastItem(qlonglong supplierId,
                          qlonglong productId);
}

namespace ProductSQL
{
bool execSelect(QSqlQuery& query,
                Product& product,
                QString& error);

bool select(Product& product,
            QString& error);

bool insert(const Product& product,
            QString& error);

bool update(const Product& product,
            QString& error);

bool remove(qlonglong id,
            QString& error);
}

namespace ProductBarcodeSQL
{
bool execSelect(QSqlQuery& query,
                ProductBarcode& barcode,
                QString& error);

bool select(ProductBarcode& barcode,
            QString& error);

bool insert(const ProductBarcode& barcode,
            QString& error);

bool update(const ProductBarcode& barcode,
            QString& error);

bool remove(qlonglong id,
            QString& error);
}

namespace CategorySQL
{
 bool execSelect(QSqlQuery& query,
                 Category& category,
                 QString& error);

  bool select(Category& category,
              QString& error);

  bool insert(const Category& category,
              QString& error);

  bool update(const Category& category,
              QString& error);

  bool remove(qlonglong id,
              QString& error);
}

namespace ImageSQL
{
  bool execSelect(QSqlQuery& query,
                  Image& image,
                  QString& error);

  bool select(Image& image,
              QString& error);

  bool insert(const Image& image,
              QString& error);

  bool update(const Image& image,
              QString& error);

  bool remove(qlonglong id,
              QString& error);
}

namespace ReminderSQL
{
  bool execSelect(QSqlQuery& query,
                  Reminder& reminder,
                  QString& error);

  bool insert(const Reminder& reminder,
              QString& error);

  bool update(const Reminder& reminder,
              QString& error);

  bool select(Reminder& reminder,
              QString& error);

  bool remove(qlonglong id,
              QString& error);
}

namespace PersonSQL
{
  bool execSelect(QSqlQuery& query,
                  Person& person,
                  QString& error);

  bool execByPinCodeSelect(QSqlQuery& query,
                           const QString& pincode,
                           Person& person,
                           QString& error);

  bool select(Person& person,
              QString& error);

  bool insert(const Person& person,
              QString& error);

  bool update(const Person& person,
              QString& error);

  bool remove(qlonglong id,
              QString& error);

  bool isValidPinCode(const QString& pincode,
                      Person& person,
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

  bool remove(qlonglong id,
              QString& error);
}

namespace ReservationSQL
{
  bool execSelect(QSqlQuery& query,
                  Reservation& res,
                  QString& error);

  bool select(Reservation& res,
              QString& error);

  bool insert(const Reservation& res,
              QString& error);

  bool update(const Reservation& res,
              QString& error);

  bool remove(qlonglong id,
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
