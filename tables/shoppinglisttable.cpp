#include "shoppinglisttable.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>
#include <QMessageBox>
#include "tableitems/doubleitem.h"
#include "tableitems/packageitem.h"
#include "tableitems/sqlitem.h"

ShoppingListTable::ShoppingListTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(SHOPPING_LIST_NUMBER_OF_COLUMNS);
  QStringList headers;
  headers << "Unidade" << "Produto" << "Quantidade" << "Preço" << "Fornecedor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Package, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Product, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Price, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Supplier, QHeaderView::ResizeToContents);

  setHeaderIcon((int)Column::Package, QIcon(":/icons/res/unity.png"));
  setHeaderIcon((int)Column::Product, QIcon(":/icons/res/item.png"));
  setHeaderIcon((int)Column::Supplier, QIcon(":/icons/res/supplier.png"));
}

void ShoppingListTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itPackage = new PackageItem();
  auto itProduct = new SQLItem(PRODUCT_SQL_TABLE_NAME, PRODUCT_FILTER_BUY);
  auto itAmmount = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  auto itPrice = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  auto itSupplier = new SQLItem(SUPPLIER_SQL_TABLE_NAME);
  setItem(row, (int)Column::Package, itPackage);
  setItem(row, (int)Column::Product, itProduct);
  setItem(row, (int)Column::Ammount, itAmmount);
  setItem(row, (int)Column::Price, itPrice);
  setItem(row, (int)Column::Supplier, itSupplier);

  setCurrentItem(itAmmount);
  setFocus();
}

void ShoppingListTable::getListItems(QVector<ShoppingListItem>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    ShoppingListItem o;
    o.m_package = getItem(row, (int)Column::Code)->getValue().toString();
    o.m_product.m_id = getItem(row, (int)Column::Code)->getValue().toString();
    o.m_ammount = getItem(row, (int)Column::Code)->getValue().toString();
    o.m_price = getItem(row, (int)Column::Code)->getValue().toString();
    o.m_supplier = getItem(row, (int)Column::Code)->getValue().toString();
    v.push_back(o);
  }
}

void ShoppingListTable::getListItems(const QVector<ShoppingListItem>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Code)->setValue(v.at(i).m_code);
  }
}

const JItem& ShoppingListTable::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref.m_ammount = ((DoubleItem*)item(idx, (int)ShoppingListColumn::Ammount))->getValue();
    m_ref.m_price = ((DoubleItem*)item(idx, (int)ShoppingListColumn::Price))->getValue();
    m_ref.m_package = ((PackageTableWidgetItem*)item(idx, (int)ShoppingListColumn::Unity))->getItem();
    m_ref.m_product = dynamic_cast<const Product&>(((ProductTableWidgetItem*)item(idx, (int)ShoppingListColumn::Description))->getItem());
    m_ref.m_supplier = dynamic_cast<const Supplier&>(((SupplierTableWidgetItem*)item(idx, (int)ShoppingListColumn::Supplier))->getItem());
  }
  return m_ref;
}

void ShoppingListTable::addItem(const JItem& o)
{
  blockSignals(true);
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)ShoppingListColumn::Unity, new PackageTableWidgetItem);
  setItem(row, (int)ShoppingListColumn::Description, new ProductTableWidgetItem);
  setItem(row, (int)ShoppingListColumn::Ammount, new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background));
  setItem(row, (int)ShoppingListColumn::Price, new DoubleItem(Data::Type::Money, DoubleItem::Color::Background));
  setItem(row, (int)ShoppingListColumn::Supplier, new SupplierTableWidgetItem);

  setCurrentCell(row, (int)ShoppingListColumn::Ammount);

  const ShoppingListItem& _o = dynamic_cast<const ShoppingListItem&>(o);
  ((DoubleItem*)item(row, (int)ShoppingListColumn::Ammount))->setValue(_o.m_ammount);
  ((DoubleItem*)item(row, (int)ShoppingListColumn::Price))->setValue(_o.m_price);
  ((ProductTableWidgetItem*)item(row, (int)ShoppingListColumn::Description))->setItem(_o.m_product);
  ((PackageTableWidgetItem*)item(row, (int)ShoppingListColumn::Unity))->setItem(_o.m_package, _o.m_product.m_unity);
  ((SupplierTableWidgetItem*)item(row, (int)ShoppingListColumn::Supplier))->setItem(_o.m_supplier);
  blockSignals(false);
}

void ShoppingListTable::addItem()
{
  addItem(ShoppingListItem());
  itemActivate(rowCount() - 1, (int)ShoppingListColumn::Description);
  auto ptProductCell = (ProductTableWidgetItem*)item(rowCount() - 1, (int)ShoppingListColumn::Description);
  const Product& product = dynamic_cast<const Product&>(ptProductCell->getItem());
  if (!product.m_id.isValid())
    removeItem();
  else
  {
    for (int i = 0; i != rowCount() - 1; ++i)
    {
      getItem(i);
      if (m_ref.m_product.m_id != product.m_id)
        continue;

      if (QMessageBox::question(this,
                            tr("Produto duplicado"),
                            tr("O produto '%1' já consta na lista. Deseja adicioná-lo novamente?").arg(product.m_name),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
        removeItem();
      break;
    }
  }
  setFocus();
}

void ShoppingListTable::update(int row, int column)
{
  blockSignals(true);
  switch ((ShoppingListColumn)column)
  {
    case ShoppingListColumn::Ammount:
    case ShoppingListColumn::Price:
    {
      auto pt = static_cast<DoubleItem*>(item(row, column));
      pt->evaluate();
    } break;
    default:
      break;
  }

  blockSignals(false);
  emitChangedSignal();
}

void ShoppingListTable::itemActivate(int row, int column)
{
  if (column == (int)ShoppingListColumn::Description)
  {
    auto ptProduct = (ProductTableWidgetItem*)item(row, column);
    ptProduct->selectItem(PRODUCT_FILTER_BUY);
    auto ptPackage = (PackageTableWidgetItem*)item(row, (int)ShoppingListColumn::Unity);
    ptPackage->setItem(Package(), dynamic_cast<const Product&>(ptProduct->getItem()).m_unity);
  }
  else if (column == (int)ShoppingListColumn::Unity)
  {
    auto ptProduct = (ProductTableWidgetItem*)item(row, (int)ShoppingListColumn::Description);
    auto ptPackage = (PackageTableWidgetItem*)item(row, column);
    ptPackage->selectItem(dynamic_cast<const Product&>(ptProduct->getItem()).m_unity);
  }
  else if (column == (int)ShoppingListColumn::Supplier)
  {
    auto ptSupplier = (SupplierTableWidgetItem*)item(row, column);
    ptSupplier->selectItem("");
  }
}

void ShoppingListTable::itemDelete(int row, int column)
{
  if (column == (int)ShoppingListColumn::Supplier)
  {
    auto ptSupplier = (SupplierTableWidgetItem*)item(row, column);
    ptSupplier->setItem(Supplier());
  }
}

void ShoppingListTable::showSupplierColumn(bool b)
{
  b ? showColumn((int)ShoppingListColumn::Supplier)
    : hideColumn((int)ShoppingListColumn::Supplier);
}
