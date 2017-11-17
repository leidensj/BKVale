#include "notedatabase.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QComboBox>
#include <QMessageBox>
#include <QSqlTableModel>


class NoteTableModel : public QSqlTableModel
{

public:
  NoteTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
  {

  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
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
  }
};


NoteDatabase::NoteDatabase(QWidget *parent)
  : QFrame(parent)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
  , m_table(nullptr)
{
  m_btnOpen = new QPushButton();
  m_btnOpen->setFlat(true);
  m_btnOpen->setText("");
  m_btnOpen->setIconSize(QSize(24, 24));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));

  m_btnRefresh = new QPushButton();
  m_btnRefresh->setFlat(true);
  m_btnRefresh->setText("");
  m_btnRefresh->setIconSize(QSize(24, 24));
  m_btnRefresh->setIcon(QIcon(":/icons/res/refresh.png"));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/trash.png"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnOpen);
  hlayout1->addWidget(m_btnRefresh);
  hlayout1->addWidget(m_btnRemove);

  m_table = new QTableView();
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QTableView::NoEditTriggers);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(m_table);
  setLayout(vlayout1);

  QObject::connect(m_table,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SLOT(noteSelected(const QModelIndex&)));

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(noteSelected()));

  QObject::connect(m_btnRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedNote()));
}

void NoteDatabase::set(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  NoteTableModel* model = new NoteTableModel(this, db);
  model->setTable("_PROMISSORYNOTES");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setHeaderData((int)NoteTableIndex::ID, Qt::Horizontal, tr("ID"));
  model->setHeaderData((int)NoteTableIndex::Number, Qt::Horizontal, tr("Número"));
  model->setHeaderData((int)NoteTableIndex::Date, Qt::Horizontal, tr("Data"));
  model->setHeaderData((int)NoteTableIndex::Supplier, Qt::Horizontal, tr("Fornecedor"));
  model->setHeaderData((int)NoteTableIndex::Items, Qt::Horizontal, tr("Itens"));
  model->setHeaderData((int)NoteTableIndex::Total, Qt::Horizontal, tr("Total"));
  m_table->setModel(model);
  m_table->hideColumn((int)NoteTableIndex::ID);
  m_table->hideColumn((int)NoteTableIndex::Items);
  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh();
}

QSqlDatabase NoteDatabase::get() const
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    return model->database();
  }
  return QSqlDatabase();
}

NoteDatabase::~NoteDatabase()
{

}

void NoteDatabase::noteSelected()
{
  noteSelected(m_table->currentIndex());
}

void NoteDatabase::noteSelected(const QModelIndex& idx)
{
  Note note;
  if (m_table->model() != nullptr && idx.isValid())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    const int row = idx.row();
    note.m_id = model->index(row, (int)NoteTableIndex::ID).data(Qt::EditRole).toInt();
    note.m_number = model->index(row, (int)NoteTableIndex::Number).data(Qt::EditRole).toInt();
    note.m_date = model->index(row, (int)NoteTableIndex::Date).data(Qt::EditRole).toLongLong();
    note.m_supplier = model->index(row, (int)NoteTableIndex::Supplier).data(Qt::EditRole).toString();
    note.m_items = model->index(row, (int)NoteTableIndex::Items).data(Qt::EditRole).toString();
    note.m_total = model->index(row, (int)NoteTableIndex::Total).data(Qt::EditRole).toDouble();
    emit noteSelectedSignal(note);
  }
}

void NoteDatabase::refresh()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
  }
  enableControls();
}

void NoteDatabase::enableControls()
{
  bool bNoteSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bNoteSelected);
  m_btnRemove->setEnabled(bNoteSelected);
}

void NoteDatabase::removeSelectedNote()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    if (m_table->currentIndex().isValid())
    {
      int row = m_table->currentIndex().row();
      int id = model->index(row, (int)NoteTableIndex::ID).data(Qt::EditRole).toInt();
      int number = model->index(row, (int)NoteTableIndex::Number).data(Qt::EditRole).toInt();
      QDate date = QDate::fromJulianDay(model->index(row, (int)NoteTableIndex::Date).data(Qt::EditRole).toLongLong());
      QString supplier = model->index(row, (int)NoteTableIndex::Supplier).data(Qt::EditRole).toString();
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
        model->removeRow(row);
        model->submitAll();
        emit noteRemovedSignal(id);
      }
    }
  }
  enableControls();
}
