#include "jitemex.h"
#include "user.h"
#include "note.h"
#include "product.h"
#include "reminder.h"
#include "settings.h"
#include "category.h"
#include "image.h"
#include "form.h"
#include "phone.h"
#include "address.h"
#include "shoppinglist.h"
#include "reservation.h"
#include "discount.h"
#include "store.h"
#include "supplier.h"
#include "employee.h"
#include "jmodel.h"
#include "activeuser.h"

JItemSQL* JItemEx::create(const QString& tableName)
{
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return new Image;
  if (tableName == FORM_SQL_TABLE_NAME)
    return new Form;
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return new Category;
  if (tableName == STORE_SQL_TABLE_NAME)
    return new Store;
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return new Product;
  if (tableName == PRODUCT_CODE_ITEMS_SQL_TABLE_NAME)
    return new ProductCode;
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
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return new Employee;
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return new Supplier;
  if (tableName == ACTIVE_USERS_SQL_TABLE_NAME)
    return new ActiveUser;
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return new Address;
  if (tableName == PHONE_SQL_TABLE_NAME)
    return new Phone;
  return nullptr;
}

JItemSQL* JItemEx::create(const QString& tableName, Id id)
{
  auto pt = create(tableName);
  if (pt != nullptr)
    pt->m_id = id;
  return pt;
}

bool JItemEx::authenticationToInsertUpdate(const QString& tableName)
{
  if (tableName == NOTE_SQL_TABLE_NAME)
    return true;
  return false;
}

bool JItemEx::authenticationToRemove(const QString& tableName)
{
  if (tableName == NOTE_SQL_TABLE_NAME)
    return true;
  return false;
}

QString JItemEx::text(const QString& tableName)
{
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return "Imagem";
  if (tableName == FORM_SQL_TABLE_NAME)
    return "Perfil";
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return "Categoria";
  if (tableName == STORE_SQL_TABLE_NAME)
    return "Loja";
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return "Produto";
  if (tableName == PRODUCT_CODE_ITEMS_SQL_TABLE_NAME)
    return "Código";
  if (tableName == NOTE_SQL_TABLE_NAME)
    return "Vale";
  if (tableName == USER_SQL_TABLE_NAME)
    return "Usuário";
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return "Lembrete";
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return "Lista de Compras";
  if (tableName == RESERVATION_SQL_TABLE_NAME)
    return "Reserva";
  if (tableName == DISCOUNT_SQL_TABLE_NAME)
    return "Desconto";
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return "Funcionário";
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return "Fornecedor";
  if (tableName == ACTIVE_USERS_SQL_TABLE_NAME)
    return "Usuários Ativos";
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return "Endereço";
  if (tableName == PHONE_SQL_TABLE_NAME)
    return "Telefone";
  return "ERRO: Tabela não encontrada.";
}

QString JItemEx::icon(const QString& tableName)
{
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return ":/icons/res/icon.png";
  if (tableName == FORM_SQL_TABLE_NAME)
    return ":/icons/res/resume.png";
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return ":/icons/res/category.png";
  if (tableName == STORE_SQL_TABLE_NAME)
    return ":/icons/res/store.png";
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return ":/icons/res/item.png";
  if (tableName == PRODUCT_CODE_ITEMS_SQL_TABLE_NAME)
    return ":/icons/res/barcode.png";
  if (tableName == NOTE_SQL_TABLE_NAME)
    return ":/icons/res/note.png";
  if (tableName == USER_SQL_TABLE_NAME)
    return ":/icons/res/user.png";
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return ":/icons/res/postit.png";
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return ":/icons/res/shopmgt.png";
  if (tableName == RESERVATION_SQL_TABLE_NAME)
    return ":/icons/res/reservation.png";
  if (tableName == DISCOUNT_SQL_TABLE_NAME)
    return ":/icons/res/discount.png";
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return ":/icons/res/employee.png";
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return ":/icons/res/supplier.png";
  if (tableName == ACTIVE_USERS_SQL_TABLE_NAME)
    return ":/icons/res/supplier.png";
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return ":/icons/res/address.png";
  if (tableName == PHONE_SQL_TABLE_NAME)
    return ":/icons/res/phone.png";
  return "";
}

JModel* JItemEx::model(const QString& tableName, QObject* parent)
{
  JModel* m = nullptr;
  auto p = JItemEx::create(tableName);
  if (p != nullptr)
  {
    m = p->SQL_table_model(parent);
    delete p;
  }
  return m;
}
