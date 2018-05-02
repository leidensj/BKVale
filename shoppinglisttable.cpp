#include "shoppinglisttable.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>

ShoppingListTable::ShoppingListTable(QWidget* parent)
  : QTableWidget(parent)
{
  setColumnCount(SHOPPING_LIST_NUMBER_OF_COLUMNS);
  QStringList headers;
  headers << "Unidade" << "Descrição" << "Quantidade" << "Preço";
  setHorizontalHeaderLabels(headers);
  {
    QFont f = font();
    f.setPointSize(12);
    f.setCapitalization(QFont::AllUppercase);
    setFont(f);
  }
  {
    QFont f = horizontalHeader()->font();
    f.setPointSize(12);
    f.setCapitalization(QFont::Capitalize);
    horizontalHeader()->setFont(f);
  }

  setSelectionBehavior(QAbstractItemView::SelectItems);
  setSelectionMode(QAbstractItemView::SingleSelection);
  horizontalHeader()->setHighlightSections(false);
  verticalHeader()->setHighlightSections(false);
  horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

  QObject::connect(this,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(update(int, int)));

  QObject::connect(this,
                   SIGNAL(currentCellChanged(int, int, int, int)),
                   this,
                   SLOT(emitChangedSignal()));
}

void ShoppingListTable::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    QKeyEvent modEvent(event->type(),
                       Qt::Key_Tab,
                       event->modifiers(),
                       event->nativeScanCode(),
                       event->nativeVirtualKey(),
                       event->nativeModifiers(),
                       event->text(),
                       event->isAutoRepeat(),
                       event->count());
    QTableWidget::keyPressEvent(&modEvent);
  }
  else
  {
    QKeyEvent modEvent(event->type(),
                       event->key(),
                       event->modifiers(),
                       event->nativeScanCode(),
                       event->nativeVirtualKey(),
                       event->nativeModifiers(),
                       event->text().toUpper(),
                       event->isAutoRepeat(),
                       event->count());
    QTableWidget::keyPressEvent(&modEvent);
  }
}

double ShoppingListTable::evaluate(int row, int column) const
{
  auto pt = item(row, column);
  if (pt == nullptr)
    return 0.0;
  auto exp = pt->text().toStdString();
  int error = 0;
  double res = te_interp(exp.c_str(), &error);
  if (!error)
    pt->setData(Qt::UserRole, res);
  return pt->data(Qt::UserRole).toDouble();
}

QString ShoppingListTable::text(int row, int column) const
{
  QString str;
  auto p = item(row, (int)column);
  if (p != nullptr)
    str = p->text();
  return str;
}

void ShoppingListTable::setText(int row, int column, const QString& str)
{
  auto p = item(row, column);
  if (p != nullptr)
    p->setText(str);
}

QVector<ShoppingListItem> ShoppingListTable::getShopItems() const
{
  QVector<ShoppingListItem> vShopItem;
  for (int i = 0; i != rowCount(); ++i)
  {
    ShoppingListItem shopItem = item(i, 1)->data(Qt::UserRole).value<ShoppingListItem>();
    shopItem.m_ammount = text(i, 2).toDouble();
    shopItem.m_price = text(i, 3).toDouble();
    if (shopItem.m_product.hasPackageUnity())
      shopItem.m_bIsPackageAmmount = item(i, 0)->checkState() == Qt::Checked ? true : false;
    else
      shopItem.m_bIsPackageAmmount = false;
    vShopItem.push_back(shopItem);
  }
  return vShopItem;
}

void ShoppingListTable::addShopItem(const ShoppingListItem& shopItem)
{
  blockSignals(true);
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, 0, new QTableWidgetItem);
  setItem(row, 1, new QTableWidgetItem);
  setItem(row, 2, new QTableWidgetItem);
  setItem(row, 3, new QTableWidgetItem);
  setCurrentCell(row, 0);
  setShopItem(shopItem);
  blockSignals(false);
}

void ShoppingListTable::setProduct(const Product& product)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);

    QPixmap pixmap(QSize(16, 16));
    pixmap.loadFromData(product.m_image.m_image);
    QIcon ico(pixmap);

    item(currentRow(), 1)->setText(product.m_name);
    item(currentRow(), 1)->setIcon(ico);
    if (product.hasPackageUnity())
      item(currentRow(), 0)->setFlags(Qt::NoItemFlags |
                                      Qt::ItemIsSelectable |
                                      Qt::ItemIsEnabled |
                                      Qt::ItemIsUserCheckable);
    else
      item(currentRow(), 0)->setFlags(Qt::NoItemFlags |
                                      Qt::ItemIsSelectable |
                                      Qt::ItemIsEnabled);
    item(currentRow(), 1)->setFlags(Qt::NoItemFlags |
                                    Qt::ItemIsSelectable |
                                    Qt::ItemIsEnabled);

    bool bIsPackageAmmount = false;
    if (product.hasPackageUnity())
      bIsPackageAmmount = item(currentRow(), 0)->checkState() == Qt::Checked;
    item(currentRow(), 0)->setText(product.strPackageUnity(bIsPackageAmmount));

    update(currentRow(), 2);
    update(currentRow(), 3);
    setCurrentCell(currentRow(), 0);
    blockSignals(false);
  }
}

void ShoppingListTable::setShopItem(const ShoppingListItem& shopItem)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);
    QVariant var;
    var.setValue(shopItem);
    item(currentRow(), 1)->setData(Qt::UserRole, var);
    item(currentRow(), 2)->setText(shopItem.strAmmount());
    item(currentRow(), 3)->setText(shopItem.strPrice());
    if (shopItem.m_product.hasPackageUnity())
      item(currentRow(), 0)->setCheckState(shopItem.m_bIsPackageAmmount ? Qt::Checked : Qt::Unchecked);
    item(currentRow(), 1)->setBackgroundColor(QColor(230, 230, 230));
    item(currentRow(), 0)->setBackgroundColor(QColor(230, 230, 230));
    setProduct(shopItem.m_product);
    blockSignals(false);
  }
}

void ShoppingListTable::setShopItems(const QVector<ShoppingListItem>& vShopItem)
{
  removeAllItems();
  for (int i = 0; i != vShopItem.size(); ++i)
    addShopItem(vShopItem.at(i));
}

void ShoppingListTable::update(int row, int column)
{
  blockSignals(true);
  switch (column)
  {
    case 0:
    {
      ShoppingListItem shopItem = item(row, 1)->data(Qt::UserRole).value<ShoppingListItem>();
      if (shopItem.m_product.hasPackageUnity())
      {
        bool bChecked = item(row, column)->checkState() == Qt::Checked;
        setText(row, column, shopItem.m_product.strPackageUnity(bChecked));
      }
    } break;
    case 2:
    {
      setText(row, column, ShoppingListItem::st_strAmmount(evaluate(row, column)));
    } break;
    case 3:
    {
      setText(row, column, ShoppingListItem::st_strPrice(evaluate(row, column)));
    } break;
    default:
      break;
  }
  blockSignals(false);
  emitChangedSignal();
}

void ShoppingListTable::emitChangedSignal()
{
  emit changedSignal();
}
