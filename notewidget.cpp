#include "notewidget.h"
#include "ui_bkframe.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QStringList>
#include <QKeyEvent>

NoteWidget::NoteWidget(QWidget *parent)
  : QFrame(parent)
  , ui(new Ui::NoteWidget)
  , m_bDirty(false)
  , m_bHistoryMode(false)
{
  ui->setupUi(this);

  QObject::connect(ui->table,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(updateTable(int, int)));

  QObject::connect(ui->table,
                   SIGNAL(itemSelectionChanged()),
                   this,
                   SLOT(changed()));

  QObject::connect(ui->supplier,
                   SIGNAL(editTextChanged(const QString &)),
                   this,
                   SLOT(changed()));

  QObject::connect(ui->supplier,
                   SIGNAL(editTextChanged(const QString&)),
                   this,
                   SLOT(toUpper(const QString&)));

  ui->supplier->lineEdit()->setPlaceholderText(tr("FORNECEDOR"));
  ui->date->setDate(QDate::currentDate());
}

NoteWidget::~NoteWidget()
{
  delete ui;
}

QString NoteWidget::computeUnitValue(int row) const
{
  Q_ASSERT(ui->table->item(row, (int)Column::Ammount) != nullptr);
  Q_ASSERT(ui->table->item(row, (int)Column::SubTotal) != nullptr);
  const double ammount = ui->table->item(row, (int)Column::Ammount)->text().toDouble();
  const double subTotal = ui->table->item(row, (int)Column::SubTotal)->text().toDouble();
  const double unitValue = ammount ? subTotal / ammount : 0.0;
  return Note::format(unitValue, false);
}

QString NoteWidget::computeSubTotal(int row) const
{
  Q_ASSERT(ui->table->item(row, (int)Column::Ammount) != nullptr);
  Q_ASSERT(ui->table->item(row, (int)Column::UnitValue) != nullptr);
  const double ammount = ui->table->item(row, (int)Column::Ammount)->text().toDouble();
  const double unitValue = ui->table->item(row, (int)Column::UnitValue)->text().toDouble();
  const double subTotal = ammount * unitValue;
  return Note::format(subTotal, false);
}

QString NoteWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != ui->table->rowCount(); ++row)
  {
    Q_ASSERT(ui->table->item(row, (int)Column::SubTotal) != nullptr);
    total += ui->table->item(row, (int)Column::SubTotal)->text().toDouble();
  }
  return Note::format(total, false);
}

double NoteWidget::evaluate(int row, int column) const
{
  Q_ASSERT(ui->table->item(row, column) != nullptr);

  auto pt = ui->table->item(row, column);
  auto exp = pt->text().toStdString();
  int error = 0;
  double res = te_interp(exp.c_str(), &error);
  if (!error && res >= 0)
    pt->setData(Qt::UserRole, res);
  return pt->data(Qt::UserRole).toDouble();
}

void NoteWidget::updateTable(int row, int column)
{
  ui->table->blockSignals(true);
  switch ((Column)column)
  {
    case Column::Ammount:
    {
      QString res(Note::format(evaluate(row, column), true));
      ui->table->item(row, column)->setText(res);
      ui->table->item(row, (int)Column::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case Column::UnitValue:
    {
      QString res(Note::format(evaluate(row, column), false));
      ui->table->item(row, column)->setText(res);
      ui->table->item(row, (int)Column::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case Column::SubTotal:
    {
      QString res(Note::format(evaluate(row, column), false));
      ui->table->item(row, column)->setText(res);
      ui->table->item(row, (int)Column::UnitValue)->setText(computeUnitValue(row));
      ui->total->setText(computeTotal());
    } break;
    default:
      break;
  }
  ui->table->blockSignals(false);
}

void NoteWidget::addItem()
{
  ui->table->insertRow(ui->table->rowCount());
  QComboBox* unit = new QComboBox();
  QStringList list;
  list << tr("UN")
       << tr("KG")
       << tr("FD")
       << tr("CX")
       << tr("ML")
       << tr("PCT");
  unit->insertItems(0, list);
  QComboBox* description = new QComboBox();
  description->setEditable(true);
  description->addItems(m_descriptions);
  description->setCurrentText("");

  QObject::connect(description,
                   SIGNAL(editTextChanged(const QString&)),
                   this,
                   SLOT(toUpper(const QString&)));

  const int row = ui->table->rowCount() - 1;
  ui->table->blockSignals(true);
  ui->table->setCellWidget(row, (int)Column::Unity, unit);
  ui->table->setCellWidget(row, (int)Column::Description, description);
  ui->table->setItem(row, (int)Column::Ammount, new QTableWidgetItem("0.000"));
  ui->table->setItem(row, (int)Column::UnitValue, new QTableWidgetItem("0.00"));
  ui->table->setItem(row, (int)Column::SubTotal, new QTableWidgetItem("0.00"));
  ui->table->setCurrentCell(row, (int)Column::Ammount);
  ui->table->setFocus();
  ui->table->blockSignals(false);
  ui->total->setText(computeTotal());
  emit changedSignal();
}

void NoteWidget::removeItem()
{
  ui->table->removeRow(ui->table->currentRow());
  if (ui->table->rowCount() != 0)
    ui->total->setText(computeTotal());
  else
    ui->total->clear();
}

void NoteWidget::changed()
{
  m_bDirty = true;
  emit changedSignal();
}

bool NoteWidget::isValidSelection() const
{
  return ui->table->currentRow() >= 0;
}

QString NoteWidget::text(int row, int column) const
{
  QString str;
  switch((Column)column)
  {
    case Column::Ammount:
    case Column::UnitValue:
    case Column::SubTotal:
    {
      auto p = ui->table->item(row, (int)column);
      if (p != nullptr)
        str = p->text();
    } break;
    case Column::Description:
    case Column::Unity:
    {
      QComboBox* pt = dynamic_cast<QComboBox*>(ui->table->cellWidget(row, (int)column));
      if (pt != nullptr)
        str = pt->currentText();
    } break;
    default:
    {
      Q_ASSERT(false);
    } break;
  }
  return str;
}

void NoteWidget::setText(int row, int column, const QString& str)
{
  switch((Column)column)
  {
    case Column::Ammount:
    case Column::UnitValue:
    case Column::SubTotal:
    {
      auto p = ui->table->item(row, column);
      if (p != nullptr)
        p->setText(str);
    } break;
    case Column::Description:
    {
      QComboBox* pt = dynamic_cast<QComboBox*>(ui->table->cellWidget(row, column));
      if (pt != nullptr)
        pt->setCurrentText(str);
    } break;
    case Column::Unity:
    {
      QComboBox* pt = dynamic_cast<QComboBox*>(ui->table->cellWidget(row, column));
      if (pt != nullptr)
      {
        int idx = pt->findText(str, Qt::MatchFixedString);
        if (idx != -1)
          pt->setCurrentIndex(idx);
      }
    } break;
    default:
    {
      Q_ASSERT(false);
    } break;
  }
}

QString NoteWidget::serializeItems() const
{
  QString str;
  for (int i = 0; i != ui->table->rowCount(); ++i)
    for (int j = 0; j!= ui->table->columnCount(); ++j)
      str += text(i, j) + ";";

  if (!str.isEmpty())
    str.chop(1);

  return str;
}

Note NoteWidget::getNote() const
{
  Note note;
  note.m_date = ui->date->date().toJulianDay();
  note.m_supplier = ui->supplier->currentText();
  note.m_number = ui->number->text().toInt();
  note.m_total = ui->total->text().toDouble();
  note.m_items = serializeItems();
  return note;
}

void NoteWidget::setNote(const Note& note)
{
  ui->table->setRowCount(0);
  ui->date->setDate(QDate::fromJulianDay(note.m_date));
  ui->supplier->setCurrentText(note.m_supplier);
  ui->number->setValue(note.m_number);
  NoteItems items(note.m_items);
  for (int row = 0; row != items.m_size; ++row)
  {
    addItem();
    setText(row, (int)Column::Ammount, items.at(row, Column::Ammount));
    setText(row, (int)Column::Unity, items.at(row, Column::Unity));
    setText(row, (int)Column::UnitValue, items.at(row, Column::UnitValue));
    setText(row, (int)Column::Description, items.at(row, Column::Description));
    setText(row, (int)Column::SubTotal, items.at(row, Column::SubTotal));
  }
  m_bDirty = false;
  m_bHistoryMode = true;
}

bool NoteWidget::isValid() const
{
  return !ui->supplier->currentText().isEmpty() &&
      ui->table->rowCount() != 0;
}

bool NoteWidget::isDirty() const
{
  return m_bDirty;
}

bool NoteWidget::isHistoryMode() const
{
  return m_bHistoryMode;
}

void NoteWidget::clear()
{
  ui->date->setSpecialValueText(" ");
  ui->date->setDate(ui->date->minimumDate());
  ui->supplier->setCurrentText("");
  ui->number->clear();
  ui->total->setText("");
  ui->table->setRowCount(0);
  while (ui->supplier->count())
    ui->supplier->removeItem(ui->supplier->count() - 1);
  m_bDirty = false;
  m_bHistoryMode = false;
}

void NoteWidget::createNew(int number,
                           const QStringList& suppliers,
                           const QStringList& descriptions)
{
  clear();
  ui->date->setDate(QDate::currentDate());
  ui->number->setValue(number);
  ui->supplier->addItems(suppliers);
  ui->supplier->setCurrentText("");
  ui->supplier->setFocus();
  m_descriptions = descriptions;
}

void NoteWidget::setEnabled(bool bEnable)
{
  ui->date->setEnabled(bEnable);
  ui->supplier->setEnabled(bEnable);
  ui->number->setEnabled(bEnable);
  ui->total->setEnabled(bEnable);
  ui->table->setEnabled(bEnable);
}

void NoteWidget::toUpper(const QString& text)
{
  QComboBox* cb = qobject_cast<QComboBox*>(sender());
  if (cb != nullptr)
  {
    cb->blockSignals(true);
    cb->setCurrentText(text.toUpper());
    cb->blockSignals(false);
  }
}

bool NoteWidget::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent* key = static_cast<QKeyEvent*>(event);
    if ((key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return))
    {
      if (ui->supplier->hasFocus() && ui->table->rowCount() != 0)
        ui->table->item(0, 0)->setSelected(true);
      else if(ui->table->hasFocus())
      {
        auto list = ui->table->selectedItems();
        if (list.size() != 0)
        {
          auto p = list.at(0);
          if (p != nullptr)
          {
            if (p->column() < (NUMBER_OF_COLUMNS - 1))
              ui->table->item(p->row(), p->column() + 1)->setSelected(true);
          }
        }
      }
    }
    else
    {
      return QObject::eventFilter(obj, event);
    }
    return true;
  }
  else
  {
    return QObject::eventFilter(obj, event);
  }
  return false;
}

QStringList NoteWidget::getItemDescriptions() const
{
  QStringList ls;
  for (auto row = 0; row != ui->table->rowCount(); ++row)
    ls << text(row, (int)Column::Description);
  return ls;
}
