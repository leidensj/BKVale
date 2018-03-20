#include "notewidget.h"
#include "noteview.h"
#include "printutils.h"
#include "databaseutils.h"
#include "jdatabase.h"
#include <QSplitter>
#include <QDockWidget>
#include <QLayout>
#include <QMessageBox>
#include <QSqlTableModel>

class NoteTableModel : public QSqlTableModel
{

public:
  NoteTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
  {

  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    QVariant value = QSqlTableModel::data(idx, role);
    if (role == Qt::DisplayRole)
    {
      if (idx.column() == 2)
        value = QDate::fromJulianDay(value.toLongLong()).toString("dd/MM/yyyy");
      else if (idx.column() == 4)
        value = "R$ " + QString::number(value.toDouble(), 'f', 2);
    }
    return value;
  }
};

NoteWidget::NoteWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_database(nullptr)
  , m_dock(nullptr)
{
  m_view = new NoteView();
  m_database = new JDatabase();
  m_database->layout()->setContentsMargins(0, 9, 9, 0);
  m_dock = new QDockWidget();
  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle(tr("Pesquisar"));
  m_dock->setWidget(m_database);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(m_view);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->addWidget(splitter);
  setLayout(vlayout1);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(setNote(int)));
  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removeNote(int)));
  QObject::connect(m_view,
                   SIGNAL(showSearchSignal()),
                   this,
                   SLOT(showSearch()));
  QObject::connect(m_view,
                   SIGNAL(createSignal()),
                   this,
                   SLOT(create()));
  QObject::connect(m_view,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(emitChangedSignal()));
  QObject::connect(m_view,
                   SIGNAL(openLastSignal(int)),
                   this,
                   SLOT(setNote(int)));
  QObject::connect(m_view,
                   SIGNAL(searchProductSignal(int)),
                   this,
                   SLOT(searchProduct(int)));
  m_dock->close();
}

void NoteWidget::showSearch()
{
  if (m_dock->isVisible())
  {
    m_dock->close();
    m_view->layout()->setContentsMargins(0, 0, 0, 0);
  }
  else
  {
    m_dock->show();
    m_view->layout()->setContentsMargins(9, 0, 0, 0);
  }
}

bool NoteWidget::print(QIODevice* printer,
                       InterfaceType type,
                       const QString& user,
                       int id)
{
  Note note;
  note.m_id = id;
  QString error;
  if (NoteSQL::select(m_database->get(), note, error))
  {
    QString str(NotePrinter::build(note, user));
    if (Printer::printString(printer, type, str, error))
      return true;
  }

  QMessageBox::warning(this,
                       tr("Erro"),
                       tr("Erro '%1' ao imprimir o vale.").arg(error),
                       QMessageBox::Ok);
  return false;
}

bool NoteWidget::save()
{
  QString error;
  Note note = m_view->getNote();

  if (note.isValidId()
      ? NoteSQL::update(m_database->get(), note, error)
      : NoteSQL::insert(m_database->get(), note, error))
  {
    m_view->setLastID(note.m_id);
    m_database->refresh();
    return true;
  }

  QMessageBox::critical(this,
                        tr("Erro"),
                        tr("Erro '%1' ao salvar vale.").arg(error),
                        QMessageBox::Ok);
  return false;
}

void NoteWidget::saveAndPrint(QIODevice* printer,
                              InterfaceType type,
                              const QString& user)
{
  if (save())
  {
    create();
    print(printer, type, user, m_view->getLastID());
  }
}

void NoteWidget::create()
{
  m_view->create(NoteSQL::nextNumber(m_database->get()));
}

void NoteWidget::setNote(int id)
{
  Note note;
  note.m_id = id;
  QString error;
  if (NoteSQL::select(m_database->get(), note, error))
  {
    m_view->setLastID(note.m_id);
    m_view->setNote(note);
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao abrir a nota com ID '%2'.").arg(error, QString::number(note.m_id)),
                          QMessageBox::Ok);
  }
}

void NoteWidget::setDatabase(QSqlDatabase db)
{
  NoteTableModel* model = new NoteTableModel(m_database, db);
  m_database->set(model, NOTE_SQL_TABLE_NAME, Note::getColumns());
  m_view->setDatabase(db);
}

void NoteWidget::emitChangedSignal()
{
  emit changedSignal();
}

void NoteWidget::removeNote(int id)
{
  Note note;
  note.m_id = id;
  QString error;
  NoteSQL::select(m_database->get(), note, error);
  if (QMessageBox::question(this,
                            tr("Remover vale"),
                            tr("Tem certeza que deseja remover o seguinte vale:\n"
                               "Número: %1\n"
                               "Fornecedor: %2\n"
                               "Data: %3").arg(Note::st_strNumber(note.m_number),
                                               note.m_supplier.m_alias,
                                               note.strDate()),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    if (NoteSQL::remove(m_database->get(), note.m_id, error))
    {
      if (id == m_view->getLastID())
        m_view->setLastID(INVALID_ID);
      Note note = m_view->getNote();
      if (id == note.m_id)
        create();
      m_database->refresh();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao remover a nota com ID '%2'.").arg(error,
                                                                               QString::number(note.m_id)),
                            QMessageBox::Ok);
    }
  }
}

void NoteWidget::searchProduct(int row)
{
  QSqlTableModel* model = new QSqlTableModel(this, m_database->get());
  JDatabaseSelector dlg(tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.set(model, PRODUCT_SQL_TABLE_NAME, Product::getColumns());
  dlg.exec();
  if (IS_VALID_ID(dlg.getCurrentId()))
  {
    Product product;
    product.m_id = dlg.getCurrentId();
    QString error;
    bool bSuccess = ProductSQL::select(m_database->get(), product, error);
    if (bSuccess)
    {
      if (row < 0)
      {
        NoteItem noteItem;
        noteItem.m_product = product;
        m_view->addNoteItem(noteItem);
      }
      else
      {
        m_view->setProduct(row, product);
      }
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao buscar produto com ID '%2'.").arg(error,
                                                                               QString::number(dlg.getCurrentId())),
                            QMessageBox::Ok);
    }
  }
}

Note NoteWidget::getNote() const
{
  return m_view->getNote();
}
