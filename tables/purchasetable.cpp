#include "purchasetable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "widgets/jtablewidgetitem.h"

PurchaseTable::PurchaseTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable((int)Flags::BigFont, parent)
{
  setColumnCount(5);
  QStringList headers;
  headers << "Quantidade" << "Unidade" << "Produto" << "Preço" << "Subtotal";
  setHorizontalHeaderLabels(headers);
  setHeaderIcon((int)Column::Unity, QIcon(":/icons/res/unity.png"));
  setHeaderIcon((int)Column::Product, QIcon(":/icons/res/item.png"));

  horizontalHeader()->setSectionResizeMode((int)Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Unity, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Product, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Price, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::SubTotal, QHeaderView::ResizeToContents);

  connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(update(int,int)));
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

double PurchaseTable::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != rowCount(); ++row)
    total += getItem(row, (int)Column::SubTotal)->getValue().toDouble();
  return total;
}

void PurchaseTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;


  setItem(row, (int)Column::Ammount, new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background));
  setItem(row, (int)Column::Unity, new PackageTableWidgetItem);
  setItem(row, (int)Column::Description, new ProductTableWidgetItem);
  setItem(row, (int)Column::Price, new DoubleItem(Data::Type::Money, DoubleItem::Color::Background));
  setItem(row, (int)Column::SubTotal, new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground));

  auto itAmmount = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::None, false, false, "+");
  //auto itUnity = new DoubleItem(Data::Type::Integer, DoubleItem::Color::None, false, false, "(", ")");
  auto itProduct = new JItemSQLItem(PRODUCT_SQL_TABLE_NAME);
  auto itPrice = new DoubleItem(Data::Type::Money, true);
  auto itSubtotal = new DoubleItem(Data::Type::Money, true);

  setItem(row, (int)Column::Ammount, itAmmount);
  //setItem(row, (int)Column::Unity, itCode);
  setItem(row, (int)Column::Product, itProduct);
  setItem(row, (int)Column::Price, itPrice);
  setItem(row, (int)Column::SubTotal, itSubtotal);

  itProduct->activate();
  setCurrentItem(itAmmount);
  setFocus();
}

const JItem& PurchaseTable::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref.m_ammount = ((DoubleItem*)item(idx, (int)Column::Ammount))->getValue();
    m_ref.m_price = ((DoubleItem*)item(idx, (int)Column::Price))->getValue();
    m_ref.m_package = ((PackageTableWidgetItem*)item(idx, (int)Column::Unity))->getItem();
    m_ref.m_product = dynamic_cast<const Product&>(((ProductTableWidgetItem*)item(idx, (int)Column::Description))->getItem());
  }
  return m_ref;
}

void PurchaseTable::addItemAndLoadPrices(Id supplierId, bool bCode)
{
  addItem(PurchaseElement());
  int row = rowCount() - 1;
  auto ptProductCell = dynamic_cast<ProductTableWidgetItem*>(item(row, (int)Column::Description));
  if (bCode)
    ptProductCell->selectItemByCode(PRODUCT_FILTER_BUY);
  else
    ptProductCell->selectItem(PRODUCT_FILTER_BUY);
  const Product& product = dynamic_cast<const Product&>(ptProductCell->getItem());
  if (product.m_id.isValid())
  {
    PurchaseElement o;
    if (supplierId.isValid())
      o.SQL_select_last(supplierId, product.m_id);
    auto ptPriceCell = dynamic_cast<DoubleItem*>(item(row, (int)Column::Price));
    ptPriceCell->setValue(o.m_price);
    auto ptPackageCell = dynamic_cast<PackageTableWidgetItem*>(item(row, (int)Column::Unity));
    ptPackageCell->setItem(o.m_package, product.m_unity);
  }
  else
    removeItem();

  setFocus();
}

void PurchaseTable::addItem(const JItem& o)
{
  const PurchaseElement& _o = dynamic_cast<const PurchaseElement&>(o);

  blockSignals(true);

  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)Column::Ammount, new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background));
  setItem(row, (int)Column::Unity, new PackageTableWidgetItem);
  setItem(row, (int)Column::Description, new ProductTableWidgetItem);
  setItem(row, (int)Column::Price, new DoubleItem(Data::Type::Money, DoubleItem::Color::Background));
  setItem(row, (int)Column::SubTotal, new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground));
  setCurrentCell(row, (int)Column::Ammount);

  ((DoubleItem*)item(row, (int)Column::Ammount))->setValue(_o.m_ammount);
  ((DoubleItem*)item(row, (int)Column::Price))->setValue(_o.m_price);
  ((DoubleItem*)item(row, (int)Column::SubTotal))->setValue(_o.subtotal());
  ((ProductTableWidgetItem*)item(row, (int)Column::Description))->setItem(_o.m_product);
  ((PackageTableWidgetItem*)item(row, (int)Column::Unity))->setItem(_o.m_package, _o.m_product.m_unity);

  setCurrentCell(row, (int)Column::Ammount);
  blockSignals(false);
}


void PurchaseTable::update(int row, int column)
{
  blockSignals(true);
  switch ((Column)column)
  {
    case Column::Ammount:
    {
      auto ptAmmount = (DoubleItem*)item(row, column);
      ptAmmount->evaluate();
      auto ptSubtotal = (DoubleItem*)item(row, (int)Column::SubTotal);
      ptSubtotal->setValue(computeSubTotal(row));
    } break;
    case Column::Price:
    {
      auto ptPrice = (DoubleItem*)item(row, column);
      ptPrice->evaluate();
      auto ptSubtotal = (DoubleItem*)item(row, (int)Column::SubTotal);
      ptSubtotal->setValue(computeSubTotal(row));
    } break;
    case Column::SubTotal:
    {
      auto ptSubtotal = (DoubleItem*)item(row, column);
      ptSubtotal->evaluate();
      auto ptPrice = (DoubleItem*)item(row, (int)Column::Price);
      ptPrice->setValue(computePrice(row));
      //Re-compute subtotal
      ptSubtotal->setValue(computeSubTotal(row));
    } break;
    default:
      break;
  }

  blockSignals(false);
  emitChangedSignal();
}
