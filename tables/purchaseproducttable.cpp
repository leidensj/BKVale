#include "purchaseproducttable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "controls/databaseselector.h"
#include "tableitems/doubleitem.h"
#include "tableitems/packageitem.h"
#include "tableitems/sqlitem.h"

PurchaseProductTable::PurchaseProductTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
  , m_isNewPurchase(true)
  , PreviousPriceRole(Qt::UserRole + 1)
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

double PurchaseProductTable::computePrice(int row) const
{
  const double ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
  const double subTotal = getItem(row, (int)Column::SubTotal)->getValue().toDouble();
  const double price = ammount ? subTotal / ammount : 0.0;
  return price;
}

double PurchaseProductTable::computeSubTotal(int row) const
{
  const double ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
  const double price = getItem(row, (int)Column::Price)->getValue().toDouble();
  const double subTotal = ammount * price;
  return subTotal;
}

void PurchaseProductTable::addRow()
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

void PurchaseProductTable::addRowAndActivate()
{
  addRow();
  getItem(rowCount() - 1, (int)Column::Product)->activate();
  if (!SQLItem::st_idFromVariant(getItem(rowCount() - 1, (int)Column::Product)->getValue()).isValid())
    removeItem();
}

void PurchaseProductTable::addRowByCode()
{
  DatabaseSelector dlg(PRODUCT_CODE_ITEMS_SQL_TABLE_NAME, false, this);
  dlg.getViewer()->setFixedFilter(PRODUCT_FILTER_BUY);
  if (dlg.exec())
  {
    ProductCode c(dlg.getViewer()->getFirstSelectedId());
    if (c.m_id.isValid())
    {
      Product o;
      QString error;
      if (o.SQL_select_by_code(c, error))
      {
        addRow();
        getItem(rowCount() - 1, (int)Column::Product)->setValue(SQLItem::st_toVariant(o));
      }
    }
  }
}

void PurchaseProductTable::get(QVector<PurchaseProduct>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    PurchaseProduct o;
    o.m_ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
    o.m_price = getItem(row, (int)Column::Price)->getValue().toDouble();
    o.m_package = PackageItem::st_fromVariant(getItem(row, (int)Column::Package)->getValue());
    o.m_product.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
    o.m_product.m_name = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Product)->getValue());
    v.push_back(o);
  }
}

void PurchaseProductTable::set(const QVector<PurchaseProduct>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Product)->setValue(SQLItem::st_toVariant(v.at(i).m_product));
    getItem(row, (int)Column::Package)->setValue(PackageItem::st_toVariant(v.at(i).m_package));
    getItem(row, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
    getItem(row, (int)Column::Price)->setValue(v.at(i).m_price);
  }
}

void PurchaseProductTable::loadProductInfo(int row)
{
  if (!m_isNewPurchase)
    return;

  Product o;
  o.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
  if (o.m_id.isValid())
  {
    QString error;
    o.SQL_select(error);
    PurchaseProduct lastSupplierProduct;
    PurchaseProduct lastProduct;
    lastSupplierProduct.SQL_select_last(m_supplierId, o.m_id);
    Purchase lastPurchase;
    lastPurchase.SQL_last_purchase_that_contains_productid(o.m_id);
    lastProduct = lastPurchase.findProduct(o.m_id);
    if (lastPurchase.m_id.isValid() && lastProduct.m_id.isValid())
    {
      double price = lastProduct.m_price;
      QString strPckPrice;
      if (lastProduct.m_package.m_bIsPackage)
      {
        strPckPrice = "\n" + Data::strMoney(price) + " / " + lastProduct.m_package.strFormattedUnity(lastProduct.m_product.m_unity);
        price = lastProduct.m_price / lastProduct.m_package.m_ammount;
      }
      getItem(row, (int)Column::Price)->setData(PreviousPriceRole, price);
      QString tt = tr("Última compra\nFornecedor: %1\nData: %2\nValor: %3%4").arg(lastPurchase.m_supplier.name(), lastPurchase.strDate(), Data::strMoney(price) + " / " + lastProduct.m_product.m_unity, strPckPrice);
      getItem(row, (int)Column::Price)->setToolTip(tt);
    }
    getItem(row, (int)Column::Price)->setValue(lastSupplierProduct.m_price);
    dynamic_cast<PackageItem*>(getItem(row, (int)Column::Package))->setProductUnity(o.m_unity);
    getItem(row, (int)Column::Package)->setValue(PackageItem::st_toVariant(lastSupplierProduct.m_package));
    update(row, (int)Column::Price);
  }
}

void PurchaseProductTable::setSupplierId(Id id)
{
  m_supplierId = id;
}

void PurchaseProductTable::update(int row, int column)
{
  blockSignals(true);
  switch ((Column)column)
  {
    case Column::Package:
    case Column::Ammount:
    case Column::Price:
      getItem(row, (int)Column::SubTotal)->setValue(computeSubTotal(row));
      if (m_isNewPurchase)
      {
        double oldPrice = getItem(row, (int)Column::Price)->data(PreviousPriceRole).toDouble();
        if (oldPrice != 0)
        {
          Package pck = PackageItem::st_fromVariant(getItem(row, (int)Column::Package)->getValue());
          auto priceItem = getItem(row, (int)Column::Price);
          double price = priceItem->getValue().toDouble();
          if (pck.m_bIsPackage)
            price = price / pck.m_ammount;
          if (!Data::areEqual(price, oldPrice, Data::Type::Money))
          {
            if (oldPrice < price)
              priceItem->setIcon(QIcon(":/icons/res/upred.png"));
            else
              priceItem->setIcon(QIcon(":/icons/res/downgreen.png"));
          }
          else
            priceItem->setIcon(QIcon(":/icons/res/calcequal.png"));
        }

      }
      break;
    case Column::SubTotal:
    {
      getItem(row, (int)Column::Price)->setValue(computePrice(row));
      getItem(row, (int)Column::SubTotal)->setValue(computeSubTotal(row));
    } break;
    case Column::Product:
      loadProductInfo(row);
    default:
      break;
  }

  blockSignals(false);
  //emitChangedSignal();
}

void PurchaseProductTable::setNewPurchase(bool b)
{
  m_isNewPurchase = b;
}
