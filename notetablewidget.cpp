#include "notetablewidget.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "jtablewidgetitem.h"

NoteTableWidget::NoteTableWidget(QWidget* parent)
  : QTableWidget(parent)
{
  setColumnCount(NOTE_NUMBER_OF_COLUMNS);
  QStringList headers;
  headers << "Quantidade" << "Unidade" << "Produto" << "Preço" << "Subtotal";
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
  verticalHeader()->setSectionsMovable(true);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Unity, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Description, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Price, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::SubTotal, QHeaderView::ResizeToContents);

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

void NoteTableWidget::keyPressEvent(QKeyEvent *event)
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

double NoteTableWidget::computePrice(int row) const
{
  const double ammount = ((DoubleTableWidgetItem*)item(row, (int)NoteColumn::Ammount))->getValue();
  const double subTotal = ((DoubleTableWidgetItem*)item(row, (int)NoteColumn::SubTotal))->getValue();
  const double price = ammount ? subTotal / ammount : 0.0;
  return price;
}

double NoteTableWidget::computeSubTotal(int row) const
{
  const double ammount = ((DoubleTableWidgetItem*)item(row, (int)NoteColumn::Ammount))->getValue();
  const double price = ((DoubleTableWidgetItem*)item(row, (int)NoteColumn::Price))->getValue();
  const double subTotal = ammount * price;
  return subTotal;
}

double NoteTableWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != rowCount(); ++row)
    total += ((DoubleTableWidgetItem*)item(row, (int)NoteColumn::SubTotal))->getValue();
  return total;
}

QVector<NoteItem> NoteTableWidget::getNoteItems() const
{
  QVector<NoteItem> vNoteItem;
  for (int i = 0; i != rowCount(); ++i)
  {
    int idx = verticalHeader()->logicalIndex(i);
    NoteItem noteItem = item(idx, (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    noteItem.m_ammount = ((DoubleTableWidgetItem*)item(idx, (int)NoteColumn::Ammount))->getValue();
    noteItem.m_price = ((DoubleTableWidgetItem*)item(idx, (int)NoteColumn::Price))->getValue();
    vNoteItem.push_back(noteItem);
  }
  return vNoteItem;
}

void NoteTableWidget::addNoteItem(const NoteItem& noteItem)
{
  blockSignals(true);
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)NoteColumn::Ammount, new DoubleTableWidgetItem(JItem::DataType::Ammount,
                                                                   DoubleTableWidgetItem::Color::Background));
  setItem(row, (int)NoteColumn::Description, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::Price, new DoubleTableWidgetItem(JItem::DataType::Money,
                                                                 DoubleTableWidgetItem::Color::Background));
  setItem(row, (int)NoteColumn::SubTotal, new DoubleTableWidgetItem(JItem::DataType::Money,
                                                                    DoubleTableWidgetItem::Color::Foreground));
  setItem(row, (int)NoteColumn::Unity, new QTableWidgetItem);
  setCurrentCell(row, (int)NoteColumn::Ammount);
  setNoteItem(noteItem);
  blockSignals(false);
}

void NoteTableWidget::setPackage(const Package& package)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);
    NoteItem noteItem = item(currentRow(), (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    noteItem.m_package = package;
    QVariant var;
    var.setValue(noteItem);
    item(currentRow(), (int)NoteColumn::Description)->setData(Qt::UserRole, var);
    item(currentRow(), (int)NoteColumn::Unity)->setText(noteItem.m_package.strFormattedUnity(noteItem.m_product.m_unity));
    blockSignals(false);
  }
}

void NoteTableWidget::setProduct(const Product& product)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);

    NoteItem noteItem = item(currentRow(), (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    noteItem.m_package.clear();
    noteItem.m_product = product;
    QVariant var;
    var.setValue(noteItem);
    item(currentRow(), (int)NoteColumn::Description)->setData(Qt::UserRole, var);

    QPixmap pixmap(QSize(16, 16));
    pixmap.loadFromData(product.m_image.m_image);
    QIcon ico(pixmap);

    item(currentRow(), (int)NoteColumn::Description)->setText(product.m_name);
    item(currentRow(), (int)NoteColumn::Description)->setIcon(ico);
    item(currentRow(), (int)NoteColumn::Description)->setFlags(Qt::NoItemFlags |
                                                               Qt::ItemIsSelectable |
                                                               Qt::ItemIsEnabled);
    item(currentRow(), (int)NoteColumn::Unity)->setFlags(Qt::NoItemFlags |
                                                         Qt::ItemIsSelectable |
                                                         Qt::ItemIsEnabled |
                                                         Qt::ItemIsUserCheckable);
    item(currentRow(), (int)NoteColumn::Unity)->setText(product.m_unity);

    update(currentRow(), (int)NoteColumn::Ammount);
    update(currentRow(), (int)NoteColumn::Price);
    setCurrentCell(currentRow(), 0);
    blockSignals(false);
  }
}

void NoteTableWidget::setNoteItem(const NoteItem& noteItem)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);
    QVariant var;
    var.setValue(noteItem);
    item(currentRow(), (int)NoteColumn::Description)->setData(Qt::UserRole, var);
    ((DoubleTableWidgetItem*)item(currentRow(), (int)NoteColumn::Ammount))->setValue(noteItem.m_ammount);
    ((DoubleTableWidgetItem*)item(currentRow(), (int)NoteColumn::Price))->setValue(noteItem.m_price);
    ((DoubleTableWidgetItem*)item(currentRow(), (int)NoteColumn::SubTotal))->setValue(noteItem.subtotal());
    item(currentRow(), (int)NoteColumn::Description)->setTextColor(QColor(Qt::darkGray));
    item(currentRow(), (int)NoteColumn::Unity)->setTextColor(QColor(Qt::darkGray));
    setProduct(noteItem.m_product);
    setPackage(noteItem.m_package);
    blockSignals(false);
  }
}

void NoteTableWidget::setNoteItems(const QVector<NoteItem>& vNoteItem)
{
  removeAllItems();
  for (int i = 0; i != vNoteItem.size(); ++i)
    addNoteItem(vNoteItem.at(i));
}

void NoteTableWidget::update(int row, int column)
{
  blockSignals(true);
  switch ((NoteColumn)column)
  {
    case NoteColumn::Unity:
    {
    } break;
    case NoteColumn::Ammount:
    {
      auto ptAmmount = (DoubleTableWidgetItem*)item(row, column);
      ptAmmount->evaluate();
      auto ptSubtotal = (DoubleTableWidgetItem*)item(row, (int)NoteColumn::SubTotal);
      ptSubtotal->setValue(computeSubTotal(row));
    } break;
    case NoteColumn::Price:
    {
      auto ptPrice = (DoubleTableWidgetItem*)item(row, column);
      ptPrice->evaluate();
      auto ptSubtotal = (DoubleTableWidgetItem*)item(row, (int)NoteColumn::SubTotal);
      ptSubtotal->setValue(computeSubTotal(row));
    } break;
    case NoteColumn::SubTotal:
    {
      auto ptSubtotal = (DoubleTableWidgetItem*)item(row, column);
      ptSubtotal->evaluate();
      auto ptPrice = (DoubleTableWidgetItem*)item(row, (int)NoteColumn::Price);
      ptPrice->setValue(computePrice(row));
      //Re-compute subtotal
      ptSubtotal->setValue(computeSubTotal(row));
    } break;
    case NoteColumn::Description:
    default:
      break;
  }

  blockSignals(false);
  emitChangedSignal();
}

void NoteTableWidget::emitChangedSignal()
{
  emit changedSignal();
}

void NoteTableWidget::emitEditSignal(int row, int column)
{
  if (column == (int)NoteColumn::Unity && row >= 0)
  {
    NoteItem noteItem = item(row, (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    emit packageSignal(noteItem.m_package, noteItem.m_product.m_unity);
  }
  else if (column == (int)NoteColumn::Description && row >= 0)
  {
    NoteItem noteItem = item(row, (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    emit productSignal(noteItem.m_product);
  }
}
