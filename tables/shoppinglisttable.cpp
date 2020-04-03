#include "shoppinglisttable.h"
#include "libraries/tinyexpr.h"
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
  headers << "Embalagem" << "Produto" << "Quantidade" << "Preço" << "Fornecedor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Package, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Product, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Price, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Supplier, QHeaderView::ResizeToContents);

  setHeaderIcon((int)Column::Package, QIcon(":/icons/res/package.png"));
  setHeaderIcon((int)Column::Product, QIcon(":/icons/res/item.png"));
  setHeaderIcon((int)Column::Supplier, QIcon(":/icons/res/supplier.png"));

  connect(this, SIGNAL(changedSignal(int,int)), this, SLOT(update(int,int)));
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

  blockSignals(true);
  setItem(row, (int)Column::Package, itPackage);
  setItem(row, (int)Column::Product, itProduct);
  setItem(row, (int)Column::Ammount, itAmmount);
  setItem(row, (int)Column::Price, itPrice);
  setItem(row, (int)Column::Supplier, itSupplier);
  blockSignals(false);

  setCurrentItem(itAmmount);
  setFocus();
}

void ShoppingListTable::getListElements(QVector<ShoppingListItem>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    ShoppingListItem o;
    o.m_package = PackageItem::st_fromVariant(getItem(row, (int)Column::Package)->getValue());
    o.m_product.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
    o.m_ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
    o.m_price = getItem(row, (int)Column::Price)->getValue().toDouble();
    o.m_supplier.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Supplier)->getValue());
    v.push_back(o);
  }
}

void ShoppingListTable::setListElements(const QVector<ShoppingListItem>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Product)->setValue(SQLItem::st_toVariant(v.at(i).m_product));
    getItem(i, (int)Column::Package)->setValue(PackageItem::st_toVariant(v.at(i).m_package));
    getItem(i, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
    getItem(i, (int)Column::Price)->setValue(v.at(i).m_price);
    getItem(i, (int)Column::Supplier)->setValue(SQLItem::st_toVariant(v.at(i).m_supplier));
  }
}

void ShoppingListTable::addRowAndActivate()
{
  addRow();
  getItem(rowCount() - 1, (int)Column::Product)->activate();
  Id id = SQLItem::st_idFromVariant(getItem(rowCount() - 1, (int)Column::Product)->getValue());
  QString name =  SQLItem::st_nameFromVariant(getItem(rowCount() - 1, (int)Column::Product)->getValue());
  if (!id.isValid())
    removeItem();
  else
  {
    for (int i = 0; i != rowCount() - 1; ++i)
    {
      if (SQLItem::st_idFromVariant(getItem(i, (int)Column::Product)->getValue()) == id)
      {
        if (QMessageBox::question(this,
                              tr("Produto duplicado"),
                              tr("O produto '%1' já consta na lista. Deseja adicioná-lo novamente?").arg(name),
                              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
          removeItem();
        break;
      }
    }
  }
}

void ShoppingListTable::showSupplierColumn(bool b)
{
  b ? showColumn((int)Column::Supplier) : hideColumn((int)Column::Supplier);
}

void ShoppingListTable::update(int row, int column)
{
  blockSignals(true);
  switch ((Column)column)
  {
    case Column::Product:
    {
      Product o;
      o.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
      if (o.m_id.isValid())
      {
        QString error;
        o.SQL_select(error);
        dynamic_cast<PackageItem*>(getItem(row, (int)Column::Package))->setProductUnity(o.m_unity);
      }
    } break;
    default:
      break;
  }

  blockSignals(false);
  emitChangedSignal();
}
