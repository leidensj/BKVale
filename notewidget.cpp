#include "notewidget.h"
#include "noteview.h"
#include "notedatabase.h"
#include "printutils.h"
#include "databaseutils.h"
#include <QSplitter>
#include <QDockWidget>
#include <QLayout>
#include <QMessageBox>

NoteWidget::NoteWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_database(nullptr)
  , m_dock(nullptr)
{
  m_view = new NoteView();
  m_database = new NoteDatabase();
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
                   SIGNAL(noteSelectedSignal(int)),
                   this,
                   SLOT(setNote(int)));

  QObject::connect(m_database,
                   SIGNAL(noteRemoveSignal(int)),
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

bool NoteWidget::print(QSerialPort& printer, int id)
{
  Note note;
  note.m_id = id;
  int number = 0;
  QString error;
  if (NoteSQL::select(m_database->get(), note, number, error))
  {
    QString str(NotePrinter::build(note, number));
    if (Printer::print(printer, str, error))
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

  if (Note::isValidID(note.m_id)
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

void NoteWidget::saveAndPrint(QSerialPort& printer)
{
  if (save())
  {
    create();
    print(printer, m_view->getLastID());
  }
}

void NoteWidget::create()
{
  m_view->create(NoteSQL::nextNumber(m_database->get()),
                 NoteSQL::suppliers(m_database->get()));
}

void NoteWidget::setNote(int id)
{
  Note note;
  note.m_id = id;
  QString error;
  int number = 0;
  if (NoteSQL::select(m_database->get(), note, number, error))
  {
    m_view->setLastID(note.m_id);
    m_view->setNote(note, number, NoteSQL::suppliers(m_database->get()));
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
  m_database->set(db);
}

bool NoteWidget::isValid() const
{
  return m_view->isValid();
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
  int number = 0;
  NoteSQL::select(m_database->get(), note, number, error);
  if (QMessageBox::question(this,
                            tr("Remover vale"),
                            tr("Tem certeza que deseja remover o seguinte vale:\n"
                               "Número: %1\n"
                               "Fornecedor: %2\n"
                               "Data: %3").arg(Note::strNumber(number),
                                               note.m_supplier,
                                               note.strDate()),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    if (NoteSQL::remove(m_database->get(), note.m_id, error))
    {
      if (id == m_view->getLastID())
        m_view->setLastID(INVALID_NOTE_ID);
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
