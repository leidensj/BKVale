#include "shoppinglisttable.h"
#include "tinyexpr.h"
#include "jtablewidgetitem.h"
#include <QHeaderView>
#include <QKeyEvent>

ShoppingListTable::ShoppingListTable(QWidget* parent)
  : JTable((int) Flags::NoFlags, parent)
{
  setColumnCount(SHOPPING_LIST_NUMBER_OF_COLUMNS);
  QStringList headers;
  headers << "Unidade" << "Produto" << "Quantidade" << "Preço" << "Fornecedor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Unity, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Description, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Price, QHeaderView::ResizeToContents);

  setHeaderIconSearchable((int)ShoppingListColumn::Description);
  setHeaderIconSearchable((int)ShoppingListColumn::Supplier);
}

const JItem& ShoppingListTable::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref.m_ammount = ((DoubleTableWidgetItem*)item(idx, (int)ShoppingListColumn::Ammount))->getValue();
    m_ref.m_bAmmount = ((DoubleTableWidgetItem*)item(idx, (int)ShoppingListColumn::Ammount))->checkState() == Qt::Checked;
    m_ref.m_price = ((DoubleTableWidgetItem*)item(idx, (int)ShoppingListColumn::Price))->getValue();
    m_ref.m_bPrice = ((DoubleTableWidgetItem*)item(idx, (int)ShoppingListColumn::Price))->checkState() == Qt::Checked;
    m_ref.m_package = ((PackageTableWidgetItem*)item(idx, (int)ShoppingListColumn::Unity))->getItem();
    m_ref.m_product = dynamic_cast<const Product&>(((ProductTableWidgetItem*)item(idx, (int)ShoppingListColumn::Description))->getItem());
    m_ref.m_supplier = dynamic_cast<const Person&>
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
  setItem(row, (int)ShoppingListColumn::Ammount, new DoubleTableWidgetItem(JItem::DataType::Ammount,
                                                                           DoubleTableWidgetItem::Color::None,
                                                                           true));
  setItem(row, (int)ShoppingListColumn::Price, new DoubleTableWidgetItem(JItem::DataType::Money,
                                                                         DoubleTableWidgetItem::Color::None,
                                                                         true));
  setItem(row, (int)ShoppingListColumn::Supplier, new PersonTableWidgetItem);

  setCurrentCell(row, (int)ShoppingListColumn::Ammount);

  const ShoppingListItem& _o = dynamic_cast<const ShoppingListItem&>(o);
  ((DoubleTableWidgetItem*)item(row, (int)ShoppingListColumn::Ammount))->setValue(_o.m_ammount);
  ((DoubleTableWidgetItem*)item(row, (int)ShoppingListColumn::Ammount))->setCheckState(_o.m_bAmmount ? Qt::Checked : Qt::Unchecked);
  ((DoubleTableWidgetItem*)item(row, (int)ShoppingListColumn::Price))->setValue(_o.m_price);
  ((DoubleTableWidgetItem*)item(row, (int)ShoppingListColumn::Price))->setCheckState(_o.m_bPrice ? Qt::Checked : Qt::Unchecked);
  ((ProductTableWidgetItem*)item(row, (int)ShoppingListColumn::Description))->setItem(_o.m_product);
  ((PackageTableWidgetItem*)item(row, (int)ShoppingListColumn::Unity))->setItem(_o.m_package, _o.m_product.m_unity);

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
}

void ShoppingListTable::update(int row, int column)
{
  blockSignals(true);
  switch ((ShoppingListColumn)column)
  {
    case ShoppingListColumn::Ammount:
    case ShoppingListColumn::Price:
    {
      auto pt = static_cast<DoubleTableWidgetItem*>(item(row, column));
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
    ptProduct->selectItem(PRODUCT_FILTER_SHOP);
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
    auto ptSupplier = (PersonTableWidgetItem*)item(row, column);
    ptSupplier->selectItem(PERSON_FILTER_SUPPLIER);
  }
}

void ShoppingListTable::itemDelete(int row, int column)
{
  if (column == (int)ShoppingListColumn::Supplier)
  {
    auto ptSupplier = (PersonTableWidgetItem*)item(row, column);
    ptSupplier->setItem(Person());
  }
}

void ShoppingListTable::showSupplierColumn(bool b)
{
  b ? showColumn((int)ShoppingListColumn::Supplier)
    : hideColumn((int)ShoppingListColumn::Supplier);
}
