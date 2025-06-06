#include "jitemhelper.h"
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
#include "login.h"
#include "printer.h"
#include "escposprinter.h"
#include "inventory.h"
#include "sector.h"
#include "coin.h"
#include "cashclosing.h"

#include <QMessageBox>
#include "controls/pincodedialog.h"

#include "models/loginmodel.h"
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
#include "models/couponmodel.h"
#include "models/inventorymodel.h"
#include "models/sectormodel.h"
#include "models/coinmodel.h"
#include "models/cashmodel.h"
#include "models/cashclosingmodel.h"

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
#include "views/couponview.h"
#include "views/inventoryview.h"
#include "views/sectorview.h"
#include "views/coinview.h"
#include "views/cashview.h"
#include "views/cashclosingview.h"

JItemSQL* JItemHelper::create(const QString& tableName)
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
  if (tableName == LOGIN_SQL_TABLE_NAME)
    return new Login;
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return new Address;
  if (tableName == PHONE_SQL_TABLE_NAME)
    return new Phone;
  if (tableName == COUPON_SQL_TABLE_NAME)
    return new Coupon;
  if (tableName == INVENTORY_SQL_TABLE_NAME)
    return new Inventory;
  if (tableName == SECTOR_SQL_TABLE_NAME)
    return new Sector;
  if (tableName == COIN_SQL_TABLE_NAME)
    return new Coin;
  if (tableName == CASH_SQL_TABLE_NAME)
    return new Cash;
  if (tableName == CASH_CLOSING_SQL_TABLE_NAME)
    return new CashClosing;
  return nullptr;
}

JItemSQL* JItemHelper::create(const QString& tableName, Id id)
{
  auto pt = create(tableName);
  if (pt != nullptr)
    pt->m_id = id;
  return pt;
}

bool JItemHelper::authenticationToSave(const QString& tableName)
{
  return (tableName == PURCHASE_SQL_TABLE_NAME ||
          tableName == REMINDER_SQL_TABLE_NAME ||
          tableName == COUPON_SQL_TABLE_NAME);
}

bool JItemHelper::authenticationToRemove(const QString& tableName)
{
  return (tableName == PURCHASE_SQL_TABLE_NAME ||
          tableName == REMINDER_SQL_TABLE_NAME ||
          tableName == COUPON_SQL_TABLE_NAME);
}

QString JItemHelper::text(Functionality::Idx idx)
{
  switch (idx)
  {
    case Functionality::Idx::User:
      return "Usuário";
    case Functionality::Idx::Category:
      return "Categoria";
    case Functionality::Idx::Product:
      return "Produto";
    case Functionality::Idx::Image:
      return "Imagem";
    case Functionality::Idx::Employee:
      return "Funcionário";
    case Functionality::Idx::Supplier:
      return "Fornecedor";
    case Functionality::Idx::Store:
      return "Loja";
    case Functionality::Idx::Purchase:
      return "Compra";
    case Functionality::Idx::Calculator:
      return "Calculadora";
    case Functionality::Idx::Reminder:
      return "Lembrete";
    case Functionality::Idx::ShoppingList:
      return "Lista de Compras";
    case Functionality::Idx::Shop:
      return "Lista de Compras";
    case Functionality::Idx::TimeCard:
      return "Livro Ponto";
    case Functionality::Idx::Coupon:
      return "Cupom";
    case Functionality::Idx::Settings:
      return "Configurações";
    case Functionality::Idx::Login:
      return "Usuários Ativos";
    case Functionality::Idx::ProductCode:
      return "Código";
    case Functionality::Idx::Address:
      return "Endereço";
    case Functionality::Idx::Phone:
      return "Telefone";
    case Functionality::Idx::Report:
      return "Relatórios";
    case Functionality::Idx::CouponRedemption:
      return "Resgatar Cupons";
    case Functionality::Idx::Inventory:
      return "Estoque";
    case Functionality::Idx::Sector:
      return "Setor";
    case Functionality::Idx::Coin:
      return "Moeda";
    case Functionality::Idx::Cash:
      return "Caixa";
    case Functionality::Idx::CashClosing:
      return "Fechamento de Caixa";
    case Functionality::Idx::_END:
    default:
      return "ERRO! Item não encontrado";
  }
}

QString JItemHelper::text(const QString& tableName)
{
  return text(Functionality::tableNameToIdx(tableName));
}

QString JItemHelper::icon(Functionality::Idx idx)
{
  switch (idx)
  {
    case Functionality::Idx::User:
      return ":/icons/res/user.png";
    case Functionality::Idx::Category:
      return ":/icons/res/category.png";
    case Functionality::Idx::Product:
      return ":/icons/res/item.png";
    case Functionality::Idx::Image:
      return ":/icons/res/icon.png";
    case Functionality::Idx::Employee:
      return ":/icons/res/employee.png";
    case Functionality::Idx::Supplier:
      return ":/icons/res/supplier.png";
    case Functionality::Idx::Store:
      return ":/icons/res/store.png";
    case Functionality::Idx::Purchase:
      return ":/icons/res/purchase.png";
    case Functionality::Idx::Calculator:
      return ":/icons/res/calculator.png";
    case Functionality::Idx::Reminder:
      return ":/icons/res/postit.png";
    case Functionality::Idx::ShoppingList:
      return ":/icons/res/shoppinglistmgt.png";
    case Functionality::Idx::Shop:
      return ":/icons/res/shoppinglist.png";
    case Functionality::Idx::TimeCard:
      return ":/icons/res/timecard.png";
    case Functionality::Idx::Coupon:
      return ":/icons/res/coupon.png";
    case Functionality::Idx::Settings:
      return ":/icons/res/settings.png";
    case Functionality::Idx::Login:
      return ":/icons/res/users.png";
    case Functionality::Idx::ProductCode:
      return ":/icons/res/barcode.png";
    case Functionality::Idx::Address:
      return ":/icons/res/address.png";
    case Functionality::Idx::Phone:
      return ":/icons/res/phone.png";
    case Functionality::Idx::Report:
      return ":/icons/res/statistics.png";
    case Functionality::Idx::CouponRedemption:
      return ":/icons/res/redeem.png";
    case Functionality::Idx::Inventory:
      return ":/icons/res/inventory.png";
    case Functionality::Idx::Sector:
      return ":/icons/res/sector.png";
    case Functionality::Idx::Coin:
      return ":/icons/res/coin.png";
    case Functionality::Idx::Cash:
      return ":/icons/res/cashier.png";
    case Functionality::Idx::CashClosing:
      return ":/icons/res/cashier.png";
    case Functionality::Idx::_END:
    default:
      return "ERRO! Item não encontrado";
  }
}

QString JItemHelper::icon(const QString& tableName)
{
  return icon(Functionality::tableNameToIdx(tableName));
}

JModel* JItemHelper::model(const QString& tableName, QObject* parent)
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
  if (tableName == LOGIN_SQL_TABLE_NAME)
    return new LoginModel(parent);
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return new AddressModel(parent);
  if (tableName == PHONE_SQL_TABLE_NAME)
    return new PhoneModel(parent);
  if (tableName == COUPON_SQL_TABLE_NAME)
    return new CouponModel(parent);
  if (tableName == INVENTORY_SQL_TABLE_NAME)
    return new InventoryModel(parent);
  if (tableName == SECTOR_SQL_TABLE_NAME)
    return new SectorModel(parent);
  if (tableName == COIN_SQL_TABLE_NAME)
    return new CoinModel(parent);
  if (tableName == CASH_SQL_TABLE_NAME)
    return new CashModel(parent);
  if (tableName == CASH_CLOSING_SQL_TABLE_NAME)
    return new CashClosingModel(parent);
  return nullptr;
}

JItemView* JItemHelper::view(const QString& tableName)
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
  if (tableName == COUPON_SQL_TABLE_NAME)
    return new CouponView;
  if (tableName == USER_SQL_TABLE_NAME)
    return new UserView;
  if (tableName == INVENTORY_SQL_TABLE_NAME)
    return new InventoryView;
  if (tableName == SECTOR_SQL_TABLE_NAME)
    return new SectorView;
  if (tableName == COIN_SQL_TABLE_NAME)
    return new CoinView;
  if (tableName == CASH_SQL_TABLE_NAME)
    return new CashView;
  if (tableName == CASH_CLOSING_SQL_TABLE_NAME)
    return new CashClosingView;
  return nullptr;
}

bool JItemHelper::select(JItemSQL& o, QWidget* parent)
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

bool JItemHelper::authenticateRemove(const QString tableName, QWidget* parent)
{
  if (JItemHelper::authenticationToRemove(tableName))
  {
    PinCodeDialog w(parent);
    if (!w.exec())
      return false;

    QString error;
    Employee e = w.getEmployee();
    if (!e.m_id.isValid())
      error = QObject::tr("Pincode informado não encontrado.");
    else if (!e.hasPermissionToRemove(tableName))
      error = QObject::tr("Funcionário não possui permissão.");
    if (!error.isEmpty())
    {
      QMessageBox::warning(parent, QObject::tr("Erro"), error, QMessageBox::Ok);
      return false;
    }
  }
  return true;
}

bool JItemHelper::authenticateSave(const JItemSQL& o, QWidget* parent)
{
  QString error;
  if (JItemHelper::authenticationToSave(o.SQL_tableName()))
  {
    PinCodeDialog w(parent);
    if (!w.exec())
      return false;

    Employee e = w.getEmployee();
    if (!e.m_id.isValid())
      error = QObject::tr("Pincode informado não encontrado.");
    else if (!e.hasPermissionToCreateEdit(o.SQL_tableName()))
      error = QObject::tr("Funcionário não possui permissão.");
    if (!error.isEmpty())
    {
      QMessageBox::warning(parent, QObject::tr("Erro"), error, QMessageBox::Ok);
      return false;
    }
    o.setEmployee(e);
  }
  return true;
}

void JItemHelper::remove(const Ids& ids, const QString& tableName, QWidget* parent)
{
  if (ids.size() == 0)
    return;

  if (!authenticateRemove(tableName, parent))
    return;

  for (auto id : ids)
  {
    auto p = JItemHelper::create(tableName, id);
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

bool JItemHelper::save(const JItemSQL& o, QWidget* parent)
{
  if (!authenticateSave(o, parent))
    return false;

  QString error;
  bool bSuccess = o.SQL_insert_update(error);
  if (!bSuccess)
    QMessageBox::critical(parent, QObject::tr("Ops..."), QObject::tr("Erro '%1' ao salvar o item.").arg(error), QMessageBox::Ok);

  return bSuccess;
}

bool JItemHelper::print(const JItemSQL& o, const QVariant& arg, QWidget* parent)
{
  QString error;
  EscPosPrinter printer;
  bool ok = printer.connectToPrinter(error);
  if (ok)
    ok = printer.printRawData(o.printVersion(arg), error);
  if (!ok)
    QMessageBox::warning(parent, QObject::tr("Erro ao imprimir"), QObject::tr("O seguinte erro ocorreu ao imprimir %1:\n%2.").arg(text(o.SQL_tableName()), error), QMessageBox::Ok);
  return ok;
}
