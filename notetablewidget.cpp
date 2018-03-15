#include "notetablewidget.h"
#include "tinyexpr.h"
#include <QHeaderView>
#include <QKeyEvent>

#define NOTE_TABLE_UNITY_FLAGS Qt::NoItemFlags | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled
#define NOTE_TABLE_DESCRIPTION_FLAGS Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled

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

QVector<NoteItem> NoteTableWidget::getNoteItems() const
{
  QVector<NoteItem> vNoteItem;
  for (int i = 0; i != rowCount(); ++i)
  {
    NoteItem noteItem = item(i, (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
    noteItem.m_ammount = text(i, (int)NoteColumn::Ammount).toDouble();
    noteItem.m_price = text(i, (int)NoteColumn::Price).toDouble();
    noteItem.m_bIsPackageAmmount = item(i, (int)NoteColumn::Unity)->checkState() == Qt::Checked
                                   ? true : false;
    vNoteItem.push_back(noteItem);
  }
  return vNoteItem;
}

void NoteTableWidget::addNoteItem(const NoteItem& noteItem)
{
  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)NoteColumn::Ammount, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::Description, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::Price, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::SubTotal, new QTableWidgetItem);
  setItem(row, (int)NoteColumn::Unity, new QTableWidgetItem);
  setNoteItem(row, noteItem);
  blockSignals(true);
  setCurrentCell(row, (int)NoteColumn::Ammount);
  blockSignals(false);
}

void NoteTableWidget::setProduct(int row, const Product& product)
{
  if (row >= 0)
  {
    blockSignals(true);
    bool bIsPackageAmmount = item(row, (int)NoteColumn::Unity)->checkState() == Qt::Checked;
    item(row, (int)NoteColumn::Unity)->setText(bIsPackageAmmount
                                               ? product.m_packageUnity
                                               : product.m_unity);

    QPixmap pixmap(QSize(16, 16));
    pixmap.loadFromData(product.m_image.m_image);
    QIcon ico(pixmap);

    item(row, (int)NoteColumn::Description)->setText(product.m_name);
    item(row, (int)NoteColumn::Description)->setIcon(ico);
    item(row, (int)NoteColumn::Unity)->setFlags(NOTE_TABLE_UNITY_FLAGS);
    item(row, (int)NoteColumn::Description)->setFlags(NOTE_TABLE_DESCRIPTION_FLAGS);
    blockSignals(false);
  }
}

void NoteTableWidget::setNoteItem(int row, const NoteItem& noteItem)
{
  if (row >= 0)
  {
    blockSignals(true);
    QVariant var;
    var.setValue(noteItem);
    item(row, (int)NoteColumn::Description)->setData(Qt::UserRole, var);
    item(row, (int)NoteColumn::Ammount)->setText(noteItem.strAmmount());
    item(row, (int)NoteColumn::Price)->setText(noteItem.strPrice());
    item(row, (int)NoteColumn::SubTotal)->setText(noteItem.strSubtotal());
    item(row, (int)NoteColumn::Unity)->setCheckState(noteItem.m_bIsPackageAmmount ? Qt::Checked : Qt::Unchecked);
    setProduct(row, noteItem.m_product);
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
      NoteItem noteItem = item(row, (int)::NoteColumn::Description)->data(Qt::UserRole).value<NoteItem>();
      bool bChecked = item(row, column)->checkState() == Qt::Checked;
      setText(row, column, bChecked
              ? noteItem.m_product.m_packageUnity
              : noteItem.m_product.m_unity);
    } break;
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
