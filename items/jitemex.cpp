#include "jitemex.h"
#include "user.h"
#include "purchase.h"
#include "product.h"
#include "reminder.h"
#include "settings.h"
#include "category.h"
#include "image.h"
#include "form.h"
#include "phone.h"
#include "address.h"
#include "shoppinglist.h"
#include "store.h"
#include "supplier.h"
#include "employee.h"
#include "activeuser.h"

#include <QMessageBox>
#include "controls/pincodedialog.h"

#include "models/activeusermodel.h"
#include "models/addressmodel.h"
#include "models/categorymodel.h"
#include "models/employeemodel.h"
#include "models/imagemodel.h"
#include "models/phonemodel.h"
#include "models/productcodemodel.h"
#include "models/productmodel.h"
#include "models/purchasemodel.h"
#include "models/remindermodel.h"
#include "models/shoppinglistmodel.h"
#include "models/storemodel.h"
#include "models/suppliermodel.h"
#include "models/usermodel.h"

#include "views/categoryview.h"
#include "views/employeeview.h"
#include "views/imageview.h"
#include "views/productview.h"
#include "views/purchaseview.h"
#include "views/reminderview.h"
#include "views/shoppinglistview.h"
#include "views/storeview.h"
#include "views/supplierview.h"
#include "views/userview.h"

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
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return new Purchase;
  if (tableName == USER_SQL_TABLE_NAME)
    return new User;
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return new Reminder;
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return new ShoppingList;
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
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return true;
  return false;
}

bool JItemEx::authenticationToRemove(const QString& tableName)
{
  if (tableName == PURCHASE_SQL_TABLE_NAME)
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
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return "Compra";
  if (tableName == USER_SQL_TABLE_NAME)
    return "Usuário";
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return "Lembrete";
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return "Lista de Compras";
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
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return ":/icons/res/purchase.png";
  if (tableName == USER_SQL_TABLE_NAME)
    return ":/icons/res/user.png";
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return ":/icons/res/postit.png";
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return ":/icons/res/shopmgt.png";
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
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return new ImageModel(parent);
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return new CategoryModel(parent);
  if (tableName == STORE_SQL_TABLE_NAME)
    return new StoreModel(parent);
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return new ProductModel(parent);
  if (tableName == PRODUCT_CODE_ITEMS_SQL_TABLE_NAME)
    return new ProductCodeModel(parent);
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return new PurchaseModel(parent);
  if (tableName == USER_SQL_TABLE_NAME)
    return new UserModel(parent);
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return new ReminderModel(parent);
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return new ShoppingListModel(parent);
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return new EmployeeModel(parent);
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return new SupplierModel(parent);
  if (tableName == ACTIVE_USERS_SQL_TABLE_NAME)
    return new ActiveUserModel(parent);
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return new AddressModel(parent);
  if (tableName == PHONE_SQL_TABLE_NAME)
    return new PhoneModel(parent);
  return nullptr;
}

JItemView* JItemEx::view(const QString& tableName)
{
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return new CategoryView;
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return new EmployeeView;
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return new ImageView;
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return new ProductView;
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return new PurchaseView;
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return new ReminderView;
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return new ShoppingListView;
  if (tableName == STORE_SQL_TABLE_NAME)
    return new StoreView;
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return new SupplierView;
  return nullptr;
}

bool JItemEx::select(JItemSQL& o, QWidget* parent)
{
  if (!o.m_id.isValid())
    return false;

  QString error;
  bool bSuccess = o.SQL_select(error);
  if (!bSuccess)
  {
    QMessageBox::critical(parent,
                          QObject::tr("Erro"),
                          QObject::tr("O seguinte erro ocorreu ao selecionar o item com ID "
                             "'%1':\n'%2'").arg(o.m_id.str(), error));
  }
  return bSuccess;
}

void JItemEx::remove(const Ids& ids, const QString& tableName, QWidget* parent)
{
  if (ids.size() == 0)
    return;

  if (QMessageBox::question(parent,
                            QObject::tr("Remover itens"),
                            QObject::tr("Tem certeza que deseja remover os itens selecionados?"),
                            QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
    return;

  if (JItemEx::authenticationToRemove(tableName))
  {
    PinCodeDialog w(parent);
    if (!w.exec())
      return;

    QString error;
    Employee e = w.getEmployee();
    if (!e.m_id.isValid())
      error = QObject::tr("Pincode informado não encontrado.");
    else if (!e.hasPermissionToRemove(tableName))
      error = QObject::tr("Funcionário não possui permissão.");
    if (!error.isEmpty())
    {
      QMessageBox::warning(parent, QObject::tr("Erro"), error, QMessageBox::Ok);
      return;
    }
  }

  for (auto id : ids)
  {
    auto p = JItemEx::create(tableName, id);
    if (p != nullptr)
    {
      QString error;
      if (!p->SQL_remove(error))
        QMessageBox::warning(parent,
                             QObject::tr("Aviso"),
                             QObject::tr("Erro ao remover item com id '%1': %2").arg(id.str(), error), QMessageBox::Ok);
      delete p;
    }
  }
}

bool JItemEx::save(const JItemSQL& o, const QString& tableName, QWidget* parent)
{
  bool bSuccess = false;
  QString error;
  if (JItemEx::authenticationToInsertUpdate(tableName))
  {
    PinCodeDialog w(parent);
    if (!w.exec())
      return false;

    Employee e = w.getEmployee();
    if (!e.m_id.isValid())
      error = QObject::tr("Pincode informado não encontrado.");
    else if (!e.hasPermissionToEdit(tableName))
      error = QObject::tr("Funcionário não possui permissão.");

    bSuccess = error.isEmpty();
    o.setEmployee(e);
  }

  if (bSuccess)
    bSuccess = o.SQL_insert_update(error);

  if (!bSuccess)
    QMessageBox::critical(parent, QObject::tr("Erro"), QObject::tr("Erro '%1' ao salvar o item.").arg(error), QMessageBox::Ok);

  return bSuccess;
}
