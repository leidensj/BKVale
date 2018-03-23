#include "notewidget.h"
#include "noteview.h"
#include "printutils.h"
#include "databaseutils.h"
#include "jdatabase.h"
#include "pincodeview.h"
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
        value = QDate::fromString(value.toString(), Qt::ISODate).toString("dd/MM/yyyy");
      else if (idx.column() == 3)
      {
        Person person;
        person.m_id = value.toLongLong();
        QString error;
        PersonSQL::select(database(), person, error);
        value = person.m_alias;
      }
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
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(setNote(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(checkRemovedNote(qlonglong)));
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
                   SIGNAL(openLastSignal(qlonglong)),
                   this,
                   SLOT(setNote(qlonglong)));
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
                       int id)
{
  Note note;
  note.m_id = id;
  QString error;
  if (NoteSQL::select(m_database->get(), note, error))
  {
    PinCodeView w(this);
    w.exec();
    if (w.getCurrentPerson().isValidId())
    {
      QString str(NotePrinter::build(note, w.getCurrentPerson().m_alias));
      if (Printer::printString(printer, type, str, error))
        return true;
    }

  }

  QMessageBox::warning(this,
                       tr("Erro"),
                       tr("Erro '%1' ao imprimir o vale.").arg(error),
                       QMessageBox::Ok);
  return false;
}

bool NoteWidget::save()
{
  Note note = m_view->getNote();
  bool bSuccess = m_database->save(note);
  if (bSuccess)
    m_view->setLastID(note.m_id);
  return bSuccess;
}

void NoteWidget::saveAndPrint(QIODevice* printer,
                              InterfaceType type)
{
  if (save())
  {
    create();
    print(printer, type, m_view->getLastID());
  }
}

void NoteWidget::create()
{
  m_view->create(NoteSQL::nextNumber(m_database->get()));
}

void NoteWidget::setNote(qlonglong id)
{
  m_database->selectItem(id);
}

void NoteWidget::setNote(const JItem& jItem)
{
  const Note& note = dynamic_cast<const Note&>(jItem);
  m_view->setLastID(note.m_id);
  m_view->setNote(note);
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

void NoteWidget::checkRemovedNote(qlonglong id)
{
  if (id == m_view->getLastID())
    m_view->setLastID(INVALID_ID);
  Note note = m_view->getNote();
  if (id == note.m_id)
    create();
}

void NoteWidget::searchProduct(int row)
{
  QSqlTableModel* model = new QSqlTableModel(this, m_database->get());
  JDatabaseSelector dlg(tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.set(model, PRODUCT_SQL_TABLE_NAME, Product::getColumns());
  dlg.exec();
  Product product = dlg.getCurrentProduct();
  if (product.isValidId())
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
}

Note NoteWidget::getNote() const
{
  return m_view->getNote();
}
