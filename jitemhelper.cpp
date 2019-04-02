#include "jitemhelper.h"
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

JItemSQL* JItemHelper::create(const QString& tableName)
{
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return new Image;
  if (tableName == PERSON_SQL_TABLE_NAME)
    return new Person;
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return new Category;
  if (tableName == STORE_SQL_TABLE_NAME)
    return new Store;
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return new Product;
  if (tableName == NOTE_SQL_TABLE_NAME)
    return new Note;
  if (tableName == USER_SQL_TABLE_NAME)
    return new User;
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return new Reminder;
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return new ShoppingList;
  if (tableName == RESERVATION_SQL_TABLE_NAME)
    return new Reservation;
  if (tableName == DISCOUNT_SQL_TABLE_NAME)
    return new Discount;
  return nullptr;
}

JItemSQL* JItemHelper::create(const QString& tableName, Id id)
{
  auto pt = create(tableName);
  if (pt != nullptr)
    pt->m_id = id;
  return pt;
}

bool JItemHelper::authenticationToInsertUpdate(const QString& tableName)
{
  auto p = create(tableName);
  bool b = p == nullptr ? false : p->SQL_authentication_insert_update();
  if (p != nullptr)
    delete p;
  return b;
}

bool JItemHelper::authenticationToRemove(const QString& tableName)
{
  auto p = create(tableName);
  bool b = p == nullptr ? false : p->SQL_authentication_remove();
  if (p != nullptr)
    delete p;
  return b;
}

QString JItemHelper::text(const QString& tableName)
{
  auto p = JItemHelper::create(tableName);
  QString text;
  if (p != nullptr)
  {
    text = p->text();
    delete p;
  }
  return text;
}

QString JItemHelper::icon(const QString& tableName)
{
  auto p = JItemHelper::create(tableName);
  QString icon;
  if (p != nullptr)
  {
    icon = p->icon();
    delete p;
  }
  return icon;
}
