#include "purchasetable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "widgets/jdatabase.h"
#include "tableitems/doubleitem.h"
#include "tableitems/packageitem.h"
#include "tableitems/sqlitem.h"

PurchaseTable::PurchaseTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(5);
  QStringList headers;
  headers << "Quantidade" << "Embalagem" << "Produto" << "Preço" << "Subtotal";
  setHorizontalHeaderLabels(headers);
  setHeaderIcon((int)Column::Package, QIcon(":/icons/res/package.png"));
  setHeaderIcon((int)Column::Product, QIcon(":/icons/res/item.png"));

  horizontalHeader()->setSectionResizeMode((int)Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Package, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Product, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Price, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::SubTotal, QHeaderView::ResizeToContents);

  connect(this, SIGNAL(changedSignal(int,int)), this, SLOT(update(int,int)));
}

double PurchaseTable::computePrice(int row) const
{
  const double ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
  const double subTotal = getItem(row, (int)Column::SubTotal)->getValue().toDouble();
  const double price = ammount ? subTotal / ammount : 0.0;
  return price;
}

double PurchaseTable::computeSubTotal(int row) const
{
  const double ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
  const double price = getItem(row, (int)Column::Price)->getValue().toDouble();
  const double subTotal = ammount * price;
  return subTotal;
}

void PurchaseTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itAmmount = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  auto itPackage = new PackageItem();
  auto itProduct = new SQLItem(PRODUCT_SQL_TABLE_NAME, PRODUCT_FILTER_BUY);
  auto itPrice = new DoubleItem(Data::Type::Money, DoubleItem::Color::Background);
  auto itSubtotal = new DoubleItem(Data::Type::Money, DoubleItem::Color::Background);

  blockSignals(true);
  setItem(row, (int)Column::Ammount, itAmmount);
  setItem(row, (int)Column::Package, itPackage);
  setItem(row, (int)Column::Product, itProduct);
  setItem(row, (int)Column::Price, itPrice);
  setItem(row, (int)Column::SubTotal, itSubtotal);
  blockSignals(false);

  setCurrentItem(itAmmount);
  setFocus();
}

void PurchaseTable::addRowAndActivate()
{
  addRow();
  getItem(rowCount() - 1, (int)Column::Product)->activate();
  if (SQLItem::st_idFromVariant(getItem(rowCount() - 1, (int)Column::Product)->getValue()).isValid())
    loadProductInfo(rowCount() - 1);
  else
    removeItem();
}

void PurchaseTable::addRowByCode()
{
  JDatabaseSelector dlg(PRODUCT_CODE_ITEMS_SQL_TABLE_NAME, false, this);
  dlg.getDatabase()->setFixedFilter(PRODUCT_FILTER_BUY);
  if (dlg.exec())
  {
    ProductCode* p = dynamic_cast<ProductCode*>(dlg.getDatabase()->getCurrentItem());
    Product o;
    QString error;
    if (o.SQL_select_by_code(*p, error))
    {
      addRow();
      getItem(rowCount() - 1, (int)Column::Product)->setValue(SQLItem::st_toVariant(o));
      loadProductInfo(rowCount() - 1);
    }
  }
}

void PurchaseTable::getPurchaseElements(QVector<PurchaseElement>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    PurchaseElement o;
    o.m_ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
    o.m_price = getItem(row, (int)Column::Price)->getValue().toDouble();
    o.m_package = PackageItem::toPackage(getItem(row, (int)Column::Package)->getValue());
    o.m_product.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
    o.m_product.m_name = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Product)->getValue());
    v.push_back(o);
  }
}

void PurchaseTable::setPurchaseElements(const QVector<PurchaseElement>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Product)->setValue(SQLItem::st_toVariant(v.at(i).m_product));
    getItem(row, (int)Column::Package)->setValue(PackageItem::toVariant(v.at(i).m_package));
    getItem(row, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
    getItem(row, (int)Column::Price)->setValue(v.at(i).m_price);
  }
}

void PurchaseTable::loadProductInfo(int row)
{
  Product o;
  o.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
  if (o.m_id.isValid())
  {
    QString error;
    o.SQL_select(error);
    PurchaseElement e;
    e.SQL_select_last(m_supplierId, o.m_id);
    getItem(row, (int)Column::Price)->setValue(e.m_price);
    dynamic_cast<PackageItem*>(getItem(row, (int)Column::Package))->setProductUnity(o.m_unity);
    getItem(row, (int)Column::Package)->setValue(PackageItem::toVariant(e.m_package));
  }
}

void PurchaseTable::setSupplierId(Id id)
{
  m_supplierId = id;
}

void PurchaseTable::update(int row, int column)
{
  blockSignals(true);
  switch ((Column)column)
  {
    case Column::Ammount:
    case Column::Price:
      getItem(row, (int)Column::SubTotal)->setValue(computeSubTotal(row));
      break;
    case Column::SubTotal:
      getItem(row, (int)Column::Price)->setValue(computePrice(row));
      getItem(row, (int)Column::SubTotal)->setValue(computeSubTotal(row));
      break;
    case Column::Product:
      loadProductInfo(row);
    default:
      break;
  }

  blockSignals(false);
  emitChangedSignal();
}
