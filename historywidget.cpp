#include "historywidget.h"
#include "ui_historywidget.h"
#include <QDate>
#include <QSqlRecord>

HistoryWidget::HistoryWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::HistoryWidget)
{
  ui->setupUi(this);

  QObject::connect(ui->table,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SLOT(noteSelected(const QModelIndex&)));
}

void HistoryWidget::set(const QSqlDatabase& sqldb)
{
  if (m_model == nullptr)
    return;

  m_model = new QSqlTableModel(this, sqldb);
  m_model->setTable("_PROMISSORYNOTES");
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_model->select();
  m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
  m_model->setHeaderData(1, Qt::Horizontal, tr("Número"));
  m_model->setHeaderData(2, Qt::Horizontal, tr("Data"));
  m_model->setHeaderData(3, Qt::Horizontal, tr("Fornecedor"));
  m_model->setHeaderData(4, Qt::Horizontal, tr("Itens"));
  m_model->setHeaderData(5, Qt::Horizontal, tr("Total"));
  ui->table->setModel(m_model);
  ui->table->hideColumn(0);
  ui->table->hideColumn(4);
}

HistoryWidget::~HistoryWidget()
{
  delete ui;
}

void HistoryWidget::noteSelected(const QModelIndex& idx)
{
  emit noteSelectedSignal(idx.row());
}

Note HistoryWidget::at(int idx) const
{
  QSqlRecord record = m_model->record(idx);
  Note note;
  note.m_id = record.value("_ID").toInt();
  note.m_number = record.value("_NUMBER").toInt();
  note.m_date = record.value("_DATE").toLongLong();
  note.m_supplier = record.value("_SUPPLIER").toString();
  note.m_items = record.value("_ITEMS").toString();
  note.m_total = record.value("_TOTAL").toDouble();
  return note;
}
