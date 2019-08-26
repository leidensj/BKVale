#include "discounttable.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>

DiscountTable::DiscountTable(QWidget* parent)
  : QTableWidget(parent)
{
  setColumnCount(3);
  QStringList headers;
  headers << "Quantidade" << "Unidade" << "Produto";
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
  horizontalHeader()->setSectionResizeMode((int)DiscountItem::Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)DiscountItem::Column::Unity, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)DiscountItem::Column::Name, QHeaderView::Stretch);

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

void DiscountTable::keyPressEvent(QKeyEvent *event)
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

double DiscountTable::evaluate(int row, int column) const
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

QString DiscountTable::text(int row, int column) const
{
  QString str;
  auto p = item(row, (int)column);
  if (p != nullptr)
    str = p->text();
  return str;
}

void DiscountTable::setText(int row, int column, const QString& str)
{
  auto p = item(row, column);
  if (p != nullptr)
    p->setText(str);
}

QVector<DiscountItem> DiscountTable::getDiscountItems() const
{
  QVector<DiscountItem> v;
  for (int i = 0; i != rowCount(); ++i)
  {
    DiscountItem o = item(i, (int)::DiscountItem::Column::Name)->data(Qt::UserRole).value<DiscountItem>();
    o.m_ammount = text(i, (int)DiscountItem::Column::Ammount).toDouble();
    v.push_back(o);
  }
  return v;
}

void DiscountTable::addDiscountItem(const DiscountItem& o)
{
  blockSignals(true);
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)DiscountItem::Column::Ammount, new QTableWidgetItem);
  setItem(row, (int)DiscountItem::Column::Unity, new QTableWidgetItem);
  setItem(row, (int)DiscountItem::Column::Name, new QTableWidgetItem);
  setCurrentCell(row, (int)DiscountItem::Column::Ammount);
  setDiscountItem(o);
  blockSignals(false);
}

void DiscountTable::setProduct(const Product& product)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);

    DiscountItem o = item(currentRow(), (int)::DiscountItem::Column::Name)->data(Qt::UserRole).value<DiscountItem>();
    o.m_product = product;
    QVariant var;
    var.setValue(o);
    item(currentRow(), (int)DiscountItem::Column::Name)->setData(Qt::UserRole, var);

    QPixmap pixmap(QSize(16, 16));
    pixmap.loadFromData(product.m_image.m_image);
    QIcon ico(pixmap);

    item(currentRow(), (int)DiscountItem::Column::Name)->setText(product.m_name);
    item(currentRow(), (int)DiscountItem::Column::Name)->setIcon(ico);
    item(currentRow(), (int)DiscountItem::Column::Name)->setFlags(Qt::NoItemFlags |
                                                                  Qt::ItemIsSelectable |
                                                                  Qt::ItemIsEnabled);
    item(currentRow(), (int)DiscountItem::Column::Unity)->setFlags(Qt::NoItemFlags |
                                                                   Qt::ItemIsSelectable |
                                                                   Qt::ItemIsEnabled);
    item(currentRow(), (int)DiscountItem::Column::Unity)->setText(product.m_unity);

    update(currentRow(), (int)DiscountItem::Column::Ammount);
    setCurrentCell(currentRow(), 0);
    blockSignals(false);
  }
}

void DiscountTable::setDiscountItem(const DiscountItem& o)
{
  if (currentRow() >= 0)
  {
    blockSignals(true);
    QVariant var;
    var.setValue(o);
    item(currentRow(), (int)DiscountItem::Column::Name)->setData(Qt::UserRole, var);
    item(currentRow(), (int)DiscountItem::Column::Ammount)->setText(o.strAmmount());
    item(currentRow(), (int)DiscountItem::Column::Unity)->setTextColor(QColor(Qt::darkGray));
    item(currentRow(), (int)DiscountItem::Column::Name)->setTextColor(QColor(Qt::darkGray));

    setProduct(o.m_product);
    blockSignals(false);
  }
}

void DiscountTable::setDiscountItems(const QVector<DiscountItem>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
    addDiscountItem(v.at(i));
}

void DiscountTable::update(int row, int column)
{
  blockSignals(true);
  switch ((DiscountItem::Column)column)
  {

    case DiscountItem::Column::Ammount:
    {
      double value = evaluate(row, column);
      setText(row, column, Data::strAmmount(value));
    } break;
    case DiscountItem::Column::Unity:
    case DiscountItem::Column::Name:
    default:
      break;
  }

  double ammount = text(row, (int)DiscountItem::Column::Ammount).toDouble();
  item(row, (int)DiscountItem::Column::Ammount)->setBackgroundColor(ammount == 0
                                                                    ? QColor(255, 200, 200)
                                                                    : QColor(Qt::white));
  blockSignals(false);
  emitChangedSignal();
}

void DiscountTable::emitChangedSignal()
{
  emit changedSignal();
}

void DiscountTable::emitEditSignal(int row, int column)
{
  if (column == (int)DiscountItem::Column::Name && row >= 0)
  {
    DiscountItem o = item(row, (int)DiscountItem::Column::Name)->data(Qt::UserRole).value<DiscountItem>();
    emit productSignal(o.m_product);
  }
}
