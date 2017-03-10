#include "notewidget.h"
#include "ui_bkframe.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QStringList>

const QChar NoteWidget::st_separator = ';';

NoteWidget::NoteWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::NoteWidget)
{
  ui->setupUi(this);

  QObject::connect(ui->table,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(updateTable(int, int)));

  QObject::connect(ui->table,
                   SIGNAL(itemSelectionChanged()),
                   this,
                   SLOT(tableSelectionChanged()));

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

double NoteWidget::evaluate(int row, Column column) const
{
  Q_ASSERT(ui->table->item(row, (int)column) != nullptr);

  auto pt = ui->table->item(row, (int)column);
  auto exp = pt->text().toStdString();
  int error = 0;
  double res = te_interp(exp.c_str(), &error);
  if (!error && res >= 0)
    pt->setData(Qt::UserRole, res);
  return pt->data(Qt::UserRole).toDouble();
}

void NoteWidget::updateTable(int row, Column column)
{
  ui->table->blockSignals(true);
  switch (column)
  {
    case Column::Ammount:
    {
      QString res(Note::format(evaluate(row, column), true));
      ui->table->item(row, (int)column)->setText(res);
      ui->table->item(row, (int)Column::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case Column::UnitValue:
    {
      QString res(Note::format(evaluate(row, column), false));
      ui->table->item(row, (int)column)->setText(res);
      ui->table->item(row, (int)Column::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case Column::SubTotal:
    {
      QString res(Note::format(evaluate(row, column), false));
      ui->table->item(row, (int)column)->setText(res);
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
  list << tr("UN") << tr("KG") << tr("FD");
  unit->insertItems(0, list);
  unit->setEditable(true);
  const int row = ui->table->rowCount() - 1;
  ui->table->blockSignals(true);
  ui->table->setCellWidget(row, (int)Column::Unity, unit);
  ui->table->setItem(row, (int)Column::Ammount, new QTableWidgetItem("0.000"));
  ui->table->setItem(row, (int)Column::Description, new QTableWidgetItem(""));
  ui->table->setItem(row, (int)Column::UnitValue, new QTableWidgetItem("0.00"));
  ui->table->setItem(row, (int)Column::SubTotal, new QTableWidgetItem("0.00"));
  ui->table->blockSignals(false);
}

void NoteWidget::removeItem()
{
  ui->table->removeRow(ui->table->currentRow());
  ui->total->setText(computeTotal());
}

void NoteWidget::tableSelectionChanged()
{
  emit tableSelectionChangedSignal();
}

bool NoteWidget::isValidSelection() const
{
  return ui->table->currentRow() >= 0;
}

QString NoteWidget::text(int row, Column column) const
{
  QString str;
  switch(column)
  {
    case Column::Ammount:
    case Column::Unity:
    case Column::Description:
    case Column::SubTotal:
    {
      auto p = ui->table->item(row, (int)column);
      if (p != nullptr)
        str = p->text();
    } break;
    case Column::UnitValue:
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

QString NoteWidget::serializeItems() const
{
  QString str;
  for (int i = 0; i != ui->table->rowCount(); ++i)
  {
    for (int j = 0; j!= ui->table->columnCount(); ++i)
    {
      str += text(i, (Column)j) + ";";
    }
  }

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

}
