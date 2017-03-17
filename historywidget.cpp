#include "historywidget.h"
#include "ui_historywidget.h"
#include <QDate>

HistoryWidget::HistoryWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::HistoryWidget)
{
  ui->setupUi(this);
}

HistoryWidget::~HistoryWidget()
{
  delete ui;
}

void HistoryWidget::refresh(Notes& notes)
{
  m_notes = std::move(notes);
  ui->table->setRowCount(0);
  for (size_t row = 0; row != m_notes.size(); ++row)
  {
    ui->table->insertRow(ui->table->rowCount());

    ui->table->setItem((int)row,
                       (int)HistColumn::Number,
                       new QTableWidgetItem(QString::number(m_notes.at(row).m_number)));

    QString strDate = QDate::fromJulianDay(m_notes.at(row).m_date).toString("dd/MM/yyyy");
    ui->table->setItem((int)row,
                       (int)HistColumn::Date,
                       new QTableWidgetItem(strDate));

    ui->table->setItem((int)row,
                       (int)HistColumn::Supplier,
                       new QTableWidgetItem(m_notes.at(row).m_supplier));

    ui->table->setItem((int)row,
                       (int)HistColumn::Total,
                       new QTableWidgetItem(Note::format(m_notes.at(row).m_total)));
  }
}
