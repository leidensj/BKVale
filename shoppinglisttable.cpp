#include "shoppinglisttable.h"
#include "tinyexpr.h"
#include "jtablewidgetitem.h"
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
  verticalHeader()->setSectionsMovable(true);
  verticalHeader()->setHighlightSections(false);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Unity, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Description, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)ShoppingListColumn::Price, QHeaderView::ResizeToContents);

  QObject::connect(this,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(update(int, int)));

  QObject::connect(this,
                   SIGNAL(currentCellChanged(int, int, int, int)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(this,
                   SIGNAL(cellDoubleClicked(int, int)),
                   this,
                   SLOT(emitEditSignal(int, int)));
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

QVector<ShoppingListItem> ShoppingListTable::getShoppingItems() const
{
  QVector<ShoppingListItem> v;
  for (int i = 0; i != rowCount(); ++i)
  {
    int idx = verticalHeader()->logicalIndex(i);
    ShoppingListItem o = item(idx, (int)::ShoppingListColumn::Description)->data(Qt::UserRole).value<ShoppingListItem>();
    o.m_ammount = ((DoubleTableWidgetItem*)item(idx, (int)ShoppingListColumn::Ammount))->getValue();
    o.m_price = ((DoubleTableWidgetItem*)item(idx, (int)ShoppingListColumn::Price))->getValue();
    o.m_bAmmount = item(idx, (int)ShoppingListColumn::Ammount)->checkState();
    o.m_bPrice = item(idx, (int)ShoppingListColumn::Price)->checkState();
    v.push_back(o);
  }
  return v;
}

void ShoppingListTable::addShoppingItem(const ShoppingListItem& o)
{
  blockSignals(true);
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)ShoppingListColumn::Unity, new QTableWidgetItem);
  setItem(row, (int)ShoppingListColumn::Description, new QTableWidgetItem);
  setItem(row, (int)ShoppingListColumn::Ammount, new DoubleTableWidgetItem(JItem::DataType::Ammount, DoubleTableWidgetItem::Color::None, true));
  setItem(row, (int)ShoppingListColumn::Price, new DoubleTableWidgetItem(JItem::DataType::Money, DoubleTableWidgetItem::Color::None, true));
  setCurrentCell(row, (int)ShoppingListColumn::Ammount);
  setShoppingItem(o);
  blockSignals(false);
}

void ShoppingListTable::setPackage(const Package& package)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);
    ShoppingListItem o = item(currentRow(), (int)::ShoppingListColumn::Description)->data(Qt::UserRole).value<ShoppingListItem>();
    o.m_package = package;
    QVariant var;
    var.setValue(o);
    item(currentRow(), (int)ShoppingListColumn::Description)->setData(Qt::UserRole, var);
    item(currentRow(), (int)ShoppingListColumn::Unity)->setText(o.m_package.strFormattedUnity(o.m_product.m_unity));
    blockSignals(false);
  }
}

void ShoppingListTable::setProduct(const Product& product)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);

    ShoppingListItem o = item(currentRow(), (int)::ShoppingListColumn::Description)->data(Qt::UserRole).value<ShoppingListItem>();
    o.m_package.clear();
    o.m_product = product;
    QVariant var;
    var.setValue(o);
    item(currentRow(), (int)ShoppingListColumn::Description)->setData(Qt::UserRole, var);

    QPixmap pixmap(QSize(16, 16));
    pixmap.loadFromData(product.m_image.m_image);
    QIcon ico(pixmap);

    item(currentRow(), (int)ShoppingListColumn::Description)->setText(product.m_name);
    item(currentRow(), (int)ShoppingListColumn::Description)->setIcon(ico);
    item(currentRow(), (int)ShoppingListColumn::Description)->setFlags(Qt::NoItemFlags |
                                                                       Qt::ItemIsSelectable |
                                                                       Qt::ItemIsEnabled);
    item(currentRow(), (int)ShoppingListColumn::Unity)->setFlags(Qt::NoItemFlags |
                                                                 Qt::ItemIsSelectable |
                                                                 Qt::ItemIsEnabled |
                                                                 Qt::ItemIsUserCheckable);
    item(currentRow(), (int)ShoppingListColumn::Unity)->setText(product.m_unity);

    update(currentRow(), (int)ShoppingListColumn::Ammount);
    update(currentRow(), (int)ShoppingListColumn::Price);
    setCurrentCell(currentRow(), 0);
    blockSignals(false);
  }
}

void ShoppingListTable::setShoppingItem(const ShoppingListItem& o)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);
    QVariant var;
    var.setValue(o);
    item(currentRow(), (int)ShoppingListColumn::Description)->setData(Qt::UserRole, var);
    ((DoubleTableWidgetItem*)item(currentRow(), (int)ShoppingListColumn::Ammount))->setValue(o.m_ammount);
    item(currentRow(), (int)ShoppingListColumn::Ammount)->setCheckState(o.m_bAmmount ? Qt::Checked : Qt::Unchecked);
    ((DoubleTableWidgetItem*)item(currentRow(), (int)ShoppingListColumn::Price))->setValue(o.m_price);
    item(currentRow(), (int)ShoppingListColumn::Price)->setCheckState(o.m_bPrice ? Qt::Checked : Qt::Unchecked);
    item(currentRow(), (int)ShoppingListColumn::Description)->setTextColor(QColor(Qt::darkGray));
    item(currentRow(), (int)ShoppingListColumn::Unity)->setTextColor(QColor(Qt::darkGray));
    setProduct(o.m_product);
    setPackage(o.m_package);
    blockSignals(false);
  }
}

void ShoppingListTable::setShoppingItems(const QVector<ShoppingListItem>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
    addShoppingItem(v.at(i));
}

void ShoppingListTable::update(int row, int column)
{
  blockSignals(true);
  switch ((ShoppingListColumn)column)
  {
    case ShoppingListColumn::Unity:
    {
    } break;
    case ShoppingListColumn::Ammount:
    case ShoppingListColumn::Price:
    {
      auto pt = static_cast<DoubleTableWidgetItem*>(item(row, column));
      pt->evaluate();
    } break;
    case ShoppingListColumn::Description:
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

void ShoppingListTable::emitEditSignal(int row, int column)
{
  if (column == (int)ShoppingListColumn::Unity && row >= 0)
  {
    ShoppingListItem o = item(row, (int)::ShoppingListColumn::Description)->data(Qt::UserRole).value<ShoppingListItem>();
    emit packageSignal(o.m_package, o.m_product.m_unity);
  }
  else if (column == (int)ShoppingListColumn::Description && row >= 0)
  {
    ShoppingListItem o = item(row, (int)::ShoppingListColumn::Description)->data(Qt::UserRole).value<ShoppingListItem>();
    emit productSignal(o.m_product);
  }
}
