#include "notedatabasewidget.h"
#include "ui_notedatabasewidget.h"
#include <QDate>
#include <QSqlRecord>

NoteDatabaseTableModel::NoteDatabaseTableModel(QObject *parent, QSqlDatabase db)
  : QSqlTableModel(parent, db)
{

}

QVariant NoteDatabaseTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QModelIndex();

  QVariant value = QSqlTableModel::data(index, role);
  if (role == Qt::DisplayRole && index.column() == (int)NoteTableIndex::Date)
    value = QDate::fromJulianDay(value.toLongLong()).toString("dd/MM/yyyy");

  return value;
}

NoteDatabaseWidget::NoteDatabaseWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::NoteDatabaseWidget)
{
  ui->setupUi(this);
  ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

  QObject::connect(ui->table,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SLOT(noteSelected(const QModelIndex&)));

  QObject::connect(ui->buttonOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(noteSelected()));
}

void NoteDatabaseWidget::setDatabase(const QSqlDatabase& sqldb)
{
  if (m_model == nullptr)
    return;

  m_model = new NoteDatabaseTableModel(this, sqldb);
  m_model->setTable("_PROMISSORYNOTES");
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_model->setHeaderData((int)NoteTableIndex::ID, Qt::Horizontal, tr("ID"));
  m_model->setHeaderData((int)NoteTableIndex::Number, Qt::Horizontal, tr("Número"));
  m_model->setHeaderData((int)NoteTableIndex::Date, Qt::Horizontal, tr("Data"));
  m_model->setHeaderData((int)NoteTableIndex::Supplier, Qt::Horizontal, tr("Fornecedor"));
  m_model->setHeaderData((int)NoteTableIndex::Items, Qt::Horizontal, tr("Itens"));
  m_model->setHeaderData((int)NoteTableIndex::Total, Qt::Horizontal, tr("Total"));
  ui->table->setModel(m_model);
  ui->table->hideColumn((int)NoteTableIndex::ID);
  ui->table->hideColumn((int)NoteTableIndex::Items);
  refresh();
}

NoteDatabaseWidget::~NoteDatabaseWidget()
{
  delete ui;
}

void NoteDatabaseWidget::noteSelected()
{
  noteSelected(ui->table->currentIndex());
}

void NoteDatabaseWidget::noteSelected(const QModelIndex& idx)
{
  Note note;
  if (m_model != nullptr && idx.isValid())
  {
    const int row = idx.row();
    note.m_id = m_model->index(row, (int)NoteTableIndex::ID).data(Qt::EditRole).toInt();
    note.m_number = m_model->index(row, (int)NoteTableIndex::Number).data(Qt::EditRole).toInt();
    note.m_date = m_model->index(row, (int)NoteTableIndex::Date).data(Qt::EditRole).toLongLong();
    note.m_supplier = m_model->index(row, (int)NoteTableIndex::Supplier).data(Qt::EditRole).toString();
    note.m_items = m_model->index(row, (int)NoteTableIndex::Items).data(Qt::EditRole).toString();
    note.m_total = m_model->index(row, (int)NoteTableIndex::Total).data(Qt::EditRole).toDouble();
    emit noteSelectedSignal(note);
  }
}

Note NoteDatabaseWidget::at(int idx) const
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

void NoteDatabaseWidget::refresh()
{
  if (m_model != nullptr)
    m_model->select();
}
