#include "notedatabasewidget.h"
#include "ui_notedatabasewidget.h"
#include <QDate>
#include <QSqlRecord>
#include <QMessageBox>

NoteDatabaseTableModel::NoteDatabaseTableModel(QObject *parent, QSqlDatabase db)
  : QSqlTableModel(parent, db)
{

}

QVariant NoteDatabaseTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QModelIndex();

  QVariant value = QSqlTableModel::data(index, role);
  if (role == Qt::DisplayRole)
  {
    if (index.column() == (int)NoteTableIndex::Date)
      value = QDate::fromJulianDay(value.toLongLong()).toString("dd/MM/yyyy");
    else if (index.column() == (int)NoteTableIndex::Total)
      value = "R$ " + Note::format(value.toString());
  }

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

  QObject::connect(ui->buttonRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(ui->buttonRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedNote()));
}

void NoteDatabaseWidget::setDatabase(QSqlDatabase db)
{
  if (m_model != nullptr)
    return;

  m_model = new NoteDatabaseTableModel(this, db);
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
  QObject::connect(ui->table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh();
}

QSqlDatabase NoteDatabaseWidget::getDatabase() const
{
  return m_model != nullptr ? m_model->database() : QSqlDatabase();
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

void NoteDatabaseWidget::refresh()
{
  if (m_model != nullptr)
    m_model->select();
  enableControls();
}

void NoteDatabaseWidget::enableControls()
{
  bool bNoteSelected = ui->table->currentIndex().isValid();
  ui->buttonOpen->setEnabled(bNoteSelected);
  ui->buttonRemove->setEnabled(bNoteSelected);
}

void NoteDatabaseWidget::removeSelectedNote()
{
  if (m_model != nullptr)
  {
    if (ui->table->currentIndex().isValid())
    {
      int row = ui->table->currentIndex().row();
      int id = m_model->index(row, (int)NoteTableIndex::ID).data(Qt::EditRole).toInt();
      int number = m_model->index(row, (int)NoteTableIndex::Number).data(Qt::EditRole).toInt();
      QDate date = QDate::fromJulianDay(m_model->index(row, (int)NoteTableIndex::Date).data(Qt::EditRole).toLongLong());
      QString supplier = m_model->index(row, (int)NoteTableIndex::Supplier).data(Qt::EditRole).toString();
      if (QMessageBox::question(this,
                                tr("Remover vale"),
                                tr("Tem certeza que deseja remover o seguinte vale:\n"
                                   "Número: %1\n"
                                   "Fornecedor: %2\n"
                                   "Data: %3").arg(QString::number(number),
                                                   supplier,
                                                   date.toString("dd/MM/yyyy")),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
      {
        m_model->removeRow(row);
        m_model->submitAll();
        emit noteRemovedSignal(id);
      }
    }
  }
  enableControls();
}
