#include "shoppinglisttable.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>
#include <QMessageBox>
#include "tableitems/doubleitem.h"
#include "tableitems/packageitem.h"
#include "tableitems/sqlitem.h"

ShoppingListTable::ShoppingListTable(JAddRemoveButtons* btns, bool bSelector, QWidget* parent)
  : JTable(btns, bSelector, parent)
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
    o.m_package = PackageItem::toPackage(getItem(row, (int)Column::Package)->getValue());
    o.m_product.m_id = SQLItem::toSQLItemAbv(getItem(row, (int)Column::Product)->getValue()).m_id;
    o.m_ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
    o.m_price = getItem(row, (int)Column::Price)->getValue().toDouble();
    o.m_supplier.m_id = SQLItem::toSQLItemAbv(getItem(row, (int)Column::Supplier)->getValue()).m_id;
    v.push_back(o);
  }
}

void ShoppingListTable::setListElements(const QVector<ShoppingListItem>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Package)->setValue(PackageItem::toVariant(v.at(i).m_package));
    getItem(i, (int)Column::Product)->setValue(SQLItem::toVariant(SQLItemAbv(v.at(i).m_product.m_id.get(), v.at(i).m_product.name())));
    getItem(i, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
    getItem(i, (int)Column::Price)->setValue(v.at(i).m_price);
    getItem(i, (int)Column::Supplier)->setValue(SQLItem::toVariant(SQLItemAbv(v.at(i).m_supplier.m_id.get(), v.at(i).m_supplier.name())));
  }
}

void ShoppingListTable::addRowAndActivate()
{
  addRow();
  getItem(rowCount() - 1, (int)Column::Product)->activate();
  SQLItemAbv abv = SQLItem::toSQLItemAbv(getItem(rowCount() - 1, (int)Column::Product)->getValue());
  if (!Id::st_isValid(abv.m_id))
    removeItem();
  else
  {
    for (int i = 0; i != rowCount() - 1; ++i)
    {
      if (SQLItem::toSQLItemAbv(getItem(i, (int)Column::Product)->getValue()).m_id == abv.m_id)
      {
        if (QMessageBox::question(this,
                              tr("Produto duplicado"),
                              tr("O produto '%1' já consta na lista. Deseja adicioná-lo novamente?").arg(abv.m_name),
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
      SQLItemAbv abv = SQLItem::toSQLItemAbv(getItem(row, (int)Column::Product)->getValue());
      if (Id::st_isValid(abv.m_id))
      {
        QString error;
        Product p;
        p.m_id = abv.m_id;
        p.SQL_select(error);
        dynamic_cast<PackageItem*>(getItem(row, (int)Column::Package))->setProductUnity(p.m_unity);
      }
    } break;
    default:
      break;
  }

  blockSignals(false);
  emitChangedSignal();
}
