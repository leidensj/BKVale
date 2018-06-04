#include "notetablewidget.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>

NoteTableWidget::NoteTableWidget(QWidget* parent)
  : QTableWidget(parent)
{
  setColumnCount(NOTE_NUMBER_OF_COLUMNS);
  QStringList headers;
  headers << "Quantidade" << "Unidade" << "Descrição" << "Preço" << "Subtotal";
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
  const double ammount = text(row, (int)NoteColumn::Ammount).toDouble();
  const double subTotal = text(row, (int)NoteColumn::SubTotal).toDouble();
  const double price = ammount ? subTotal / ammount : 0.0;
  return price;
}

double NoteTableWidget::computeSubTotal(int row) const
{
  const double ammount = text(row, (int)NoteColumn::Ammount).toDouble();
  const double unitValue = text(row, (int)NoteColumn::Price).toDouble();
  const double subTotal = ammount * unitValue;
  return subTotal;
}

double NoteTableWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != rowCount(); ++row)
    total += text(row, (int)NoteColumn::SubTotal).toDouble();
  return total;
}

double NoteTableWidget::evaluate(int row, int column) const
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

QString NoteTableWidget::text(int row, int column) const
{
  QString str;
  auto p = item(row, (int)column);
  if (p != nullptr)
    str = p->text();
  return str;
}

void NoteTableWidget::setText(int row, int column, const QString& str)
{
  auto p = item(row, column);
  if (p != nullptr)
    p->setText(str);
}

QVector<NoteItem> NoteTableWidget::getNoteItems() const
{
  QVector<NoteItem> vNoteItem;
  for (int i = 0; i != rowCount(); ++i)
  {
    NoteItem noteItem = item(i, (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    noteItem.m_ammount = text(i, (int)NoteColumn::Ammount).toDouble();
    noteItem.m_price = text(i, (int)NoteColumn::Price).toDouble();
    vNoteItem.push_back(noteItem);
  }
  return vNoteItem;
}

void NoteTableWidget::addNoteItem(const NoteItem& noteItem)
{
  blockSignals(true);
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)NoteColumn::Ammount, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::Description, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::Price, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::SubTotal, new QTableWidgetItem);
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
    item(currentRow(), (int)NoteColumn::Ammount)->setText(noteItem.strAmmount());
    item(currentRow(), (int)NoteColumn::Price)->setText(noteItem.strPrice());
    item(currentRow(), (int)NoteColumn::SubTotal)->setText(noteItem.strSubtotal());
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
      double value = evaluate(row, column);
      setText(row, column, JItem::st_strAmmount(value));
      setText(row, (int)NoteColumn::SubTotal, JItem::st_strMoney(computeSubTotal(row)));
    } break;
    case NoteColumn::Price:
    {
      double value = evaluate(row, column);
      setText(row, column, JItem::st_strMoney(value));
      setText(row, (int)NoteColumn::SubTotal, JItem::st_strMoney(computeSubTotal(row)));
    } break;
    case NoteColumn::SubTotal:
    {
      setText(row, column, JItem::st_strMoney(evaluate(row, column)));
      setText(row, (int)NoteColumn::Price, JItem::st_strMoney(computePrice(row)));
    } break;
    case NoteColumn::Description:
    default:
      break;
  }

  double subTotal = text(row, (int)NoteColumn::SubTotal).toDouble();
  double price = text(row, (int)NoteColumn::Price).toDouble();
  double ammount = text(row, (int)NoteColumn::Ammount).toDouble();

  item(row, (int)NoteColumn::SubTotal)->setTextColor(QColor(subTotal >= 0
                                                            ? Qt::red
                                                            : Qt::darkGreen));
  item(row, (int)NoteColumn::Price)->setBackgroundColor(price == 0.0
                                                        ? QColor(255, 200, 200)
                                                        : QColor(Qt::white));
  item(row, (int)NoteColumn::Ammount)->setBackgroundColor(ammount == 0
                                                          ? QColor(255, 200, 200)
                                                          : QColor(Qt::white));
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
