#include "promissorynotewidget.h"
#include "ui_bkframe.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QStringList>

const QChar PromissoryNoteWidget::st_separator = ';';

PromissoryNoteWidget::PromissoryNoteWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::PromissoryNoteWidget)
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

PromissoryNoteWidget::~PromissoryNoteWidget()
{
  delete ui;
}

QString PromissoryNoteWidget::format(const QString& str, bool b3places /*= false*/)
{
  return format(str.toDouble(), b3places);
}

QString PromissoryNoteWidget::format(double d, bool b3places /*= false*/)
{
  return QString::number(d, 'f', b3places ? 3 : 2);
}

QString PromissoryNoteWidget::computeUnitValue(int row) const
{
  Q_ASSERT(ui->table->item(row, (int)Column::Ammount) != nullptr);
  Q_ASSERT(ui->table->item(row, (int)Column::SubTotal) != nullptr);
  const double ammount = ui->table->item(row, (int)Column::Ammount)->text().toDouble();
  const double subTotal = ui->table->item(row, (int)Column::SubTotal)->text().toDouble();
  const double unitValue = ammount ? subTotal / ammount : 0.0;
  return format(unitValue, false);
}

QString PromissoryNoteWidget::computeSubTotal(int row) const
{
  Q_ASSERT(ui->table->item(row, (int)Column::Ammount) != nullptr);
  Q_ASSERT(ui->table->item(row, (int)Column::UnitValue) != nullptr);
  const double ammount = ui->table->item(row, (int)Column::Ammount)->text().toDouble();
  const double unitValue = ui->table->item(row, (int)Column::UnitValue)->text().toDouble();
  const double subTotal = ammount * unitValue;
  return format(subTotal, false);
}

QString PromissoryNoteWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != ui->table->rowCount(); ++row)
  {
    Q_ASSERT(ui->table->item(row, (int)Column::SubTotal) != nullptr);
    total += ui->table->item(row, (int)Column::SubTotal)->text().toDouble();
  }
  return format(total, false);
}

double PromissoryNoteWidget::evaluate(int row, int column)
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

void PromissoryNoteWidget::updateTable(int row, int column)
{
  ui->table->blockSignals(true);
  switch (column)
  {
    case (int)Column::Ammount:
    {
      QString res(format(evaluate(row, column), true));
      ui->table->item(row, column)->setText(res);
      ui->table->item(row, (int)Column::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case (int)Column::UnitValue:
    {
      QString res(format(evaluate(row, column), false));
      ui->table->item(row, column)->setText(res);
      ui->table->item(row, (int)Column::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case (int)Column::SubTotal:
    {
      QString res(format(evaluate(row, column), false));
      ui->table->item(row, column)->setText(res);
      ui->table->item(row, (int)Column::UnitValue)->setText(computeUnitValue(row));
      ui->total->setText(computeTotal());
    } break;
    default:
      break;
  }
  ui->table->blockSignals(false);
}

void PromissoryNoteWidget::addItem()
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

void PromissoryNoteWidget::removeItem()
{
  ui->table->removeRow(ui->table->currentRow());
  ui->total->setText(computeTotal());
}

<<<<<<< HEAD:promissorynotewidget.cpp
void PromissoryNoteWidget::getContent(TableContent& tableContent) const
=======
void BKFrame::getContent(TableContent& tableContent, QString& total)
>>>>>>> parent of da3042d... criando banco de dados:bkframe.cpp
{
  tableContent.clear();
  tableContent.reserve(ui->table->rowCount());
  total.clear();
  for (int row = 0; row != ui->table->rowCount(); ++row)
  {
    std::vector<QString> v;
    v.reserve(ui->table->columnCount());
    for (int column = 0; column != ui->table->columnCount(); ++column)
    {
      switch(column)
      {
        case (int)Column::Unity:
        {
          auto pt = static_cast<QComboBox*>(ui->table->cellWidget(row, column));
          v.emplace_back(pt->currentText());
        } break;
        default:
        {
          v.emplace_back(ui->table->item(row, column)->text());
        } break;
      }
    }
    tableContent.emplace_back(v);
  }
}

void PromissoryNoteWidget::tableSelectionChanged()
{
  emit tableSelectionChangedSignal();
}

<<<<<<< HEAD:promissorynotewidget.cpp
bool PromissoryNoteWidget::isValidSelection() const
=======
bool BKFrame::isValidSelection()
>>>>>>> parent of da3042d... criando banco de dados:bkframe.cpp
{
  return ui->table->currentRow() >= 0;
}

QString PromissoryNoteWidget::serializeTable() const
{
  QString items;
  for (int row = 0; row != ui->table->rowCount(); ++row)
  {
    for (int column = 0; column != ui->table->columnCount(); ++column)
    {
      items += getTableText(row, column);
      items += st_separator;
    }
  }
  return items;
}

void PromissoryNoteWidget::deserializeTable(const QString& str)
{

}

void PromissoryNoteWidget::setSupplier(const QString& supplier)
{
  ui->supplier->setCurrentText(supplier);
}

QString PromissoryNoteWidget::getSupplier() const
{
  return ui->supplier->currentText();
}

void PromissoryNoteWidget::setNumber(int number)
{
  ui->number->setValue(number);
}

int PromissoryNoteWidget::getNumber() const
{
  return ui->number->value();
}

void PromissoryNoteWidget::setDate(const QDate& date)
{
  ui->date->setDate(date);
}

QDate PromissoryNoteWidget::getDate() const
{
  return ui->date->date();
}

QString PromissoryNoteWidget::getTableText(int row, Column column) const
{
  QString text;
  if (row >= 0 && row < ui->table->rowCount())
  {
    switch(column)
    {
      case (int)Column::Unity:
      {
        auto pt = static_cast<QComboBox*>(ui->table->cellWidget(row, (int)column));
        if (pt != nullptr)
          text = pt->currentText();
      } break;
      default:
      {
        items += ui->table->item(row, (int)column)->text();
      } break;
    }
  }
  return text;
}

int PromissoryNoteWidget::getTableCount() const
{
  return ui->table->rowCount();
}

QString PromissoryNoteWidget::getTotal() const
{
  return ui->total->text();
}
