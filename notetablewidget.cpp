#include "notetablewidget.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>

NoteTableWidget::NoteTableWidget(QWidget* parent)
  : QTableWidget(parent)
{
  setColumnCount(NUMBER_OF_COLUMNS);
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

QString NoteTableWidget::computeUnitValue(int row) const
{
  const double ammount = text(row, (int)NoteColumn::Ammount).toDouble();
  const double subTotal = text(row, (int)NoteColumn::SubTotal).toDouble();
  const double unitValue = ammount ? subTotal / ammount : 0.0;
  return QString::number(unitValue, 'f', 2);
}

QString NoteTableWidget::computeSubTotal(int row) const
{
  const double ammount = text(row, (int)NoteColumn::Ammount).toDouble();
  const double unitValue = text(row, (int)NoteColumn::Price).toDouble();
  const double subTotal = ammount * unitValue;
  return QString::number(subTotal, 'f', 2);
}

QString NoteTableWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != rowCount(); ++row)
    total += text(row, (int)NoteColumn::SubTotal).toDouble();
  return QString::number(total, 'f', 2);
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

QVector<NoteItem>& NoteTableWidget::getItems() const
{
  QVector<NoteItem> v;
  for (int i = 0; i != rowCount(); ++i)
  {
    FullNoteItem o = item(i, (int)::NoteColumn::Description)->data(Qt::UserRole).value<FullNoteItem>();
    o.m_item.m_ammount = text(i, (int)NoteColumn::Ammount).toDouble();
    o.m_item.m_price = text(i, (int)NoteColumn::Price).toDouble();
    o.m_item.m_bIsPackageAmmount = item(i, (int)NoteColumn::Unity)->checkState() == Qt::Checked
                                   ? true : false;
    v.push_back(o.m_item);
  }
  return v;
}

void NoteTableWidget::addItem(const FullNoteItem& fItem)
{
  insertRow(rowCount());
  const int row = rowCount() - 1;

  add
  blockSignals(true);
  setItem(row, (int)NoteColumn::Ammount,
          new QTableWidgetItem(fItem.m_item.strAmmount()));
  setItem(row, (int)NoteColumn::Unity,
          new QTableWidgetItem(fItem.m_item.m_bIsPackageAmmount
                               ? fItem.m_product.m_packageUnity
                               : fItem.m_product.m_unity));
  setItem(row, (int)NoteColumn::Description,
          new QTableWidgetItem(fItem.m_product.m_name));
  setItem(row, (int)NoteColumn::Price,
          new QTableWidgetItem(fItem.m_item.strPrice()));
  setItem(row, (int)NoteColumn::SubTotal,
          new QTableWidgetItem(fItem.m_item.strSubtotal()));
  item(row, (int)NoteColumn::Description)->setData(Qt::UserRole, fItem);

  item(row, (int)NoteColumn::Unity)->setFlags(Qt::NoItemFlags |
                                              Qt::ItemIsUserCheckable |
                                              Qt::ItemIsSelectable |
                                              Qt::ItemIsEnabled);
  item(row, (int)NoteColumn::Description)->setFlags(Qt::NoItemFlags |
                                                    Qt::ItemIsSelectable |
                                                    Qt::ItemIsEnabled);
  item(row, (int)NoteColumn::Unity)->setCheckState(fItem.m_item.m_bIsPackageAmmount
                                                   ? Qt::Checked : Qt::Unchecked);

  setCurrentCell(row, (int)NoteColumn::Ammount);
  blockSignals(false);
}

void NoteTableWidget::setItems(const QVector<FullNoteItem>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
    addItem(v.at(i));
}

void NoteTableWidget::update(int row, int column)
{
  blockSignals(true);
  switch ((NoteColumn)column)
  {
    case NoteColumn::Ammount:
    {
      setText(row, column, NoteItem::st_strAmmount(evaluate(row, column)));
      setText(row, (int)NoteColumn::SubTotal, computeSubTotal(row));
    } break;
    case NoteColumn::Price:
    {
      setText(row, column, NoteItem::st_strPrice(evaluate(row, column)));
      setText(row, (int)NoteColumn::SubTotal, computeSubTotal(row));
    } break;
    case NoteColumn::SubTotal:
    {
      setText(row, column, NoteItem::st_strSubTotal(evaluate(row, column)));
      setText(row, (int)NoteColumn::Price, computeUnitValue(row));
    } break;
    case NoteColumn::Unity:
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
