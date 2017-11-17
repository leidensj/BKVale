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
                   SIGNAL(noteSelectedSignal(const Note&)),
                   this,
                   SLOT(setNote(const Note&)));

  QObject::connect(m_database,
                   SIGNAL(noteRemovedSignal(int)),
                   this,
                   SLOT(checkForRemovedNote(int)));

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
                   SLOT(openLast(int)));

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

bool NoteWidget::print(QSerialPort& printer)
{
  QString str(NotePrinter::build(m_view->getNote()));
  QString error;
  bool bSuccess = Printer::print(printer, str, error);
  if (!bSuccess)
  {
    QMessageBox::warning(this,
                         tr("Erro"),
                         tr("Erro '%1' ao imprimir o vale.").arg(error),
                         QMessageBox::Ok);
  }
  return bSuccess;
}

bool NoteWidget::save()
{
  QString error;
  Note note = m_view->getNote();
  bool bSuccess = NoteSQL::insertOrUpdate(m_database->get(), note, error);

  if (bSuccess)
  {
    m_view->setLastID(note.m_id);
    m_database->refresh();
  }
  else
  {
    QMessageBox msgBox(QMessageBox::Warning,
                       tr("Erro"),
                       tr("Erro '%1' ao salvar vale.").arg(error),
                       QMessageBox::Ok);
    msgBox.exec();
  }

  return bSuccess;
}

void NoteWidget::create()
{
  m_view->create(NoteSQL::nextNumber(m_database->get()),
                 NoteSQL::suppliers(m_database->get()));
}

void NoteWidget::setNote(const Note& note)
{
  m_view->setLastID(note.m_id);
  m_view->setNote(note, NoteSQL::suppliers(m_database->get()));
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

void NoteWidget::checkForRemovedNote(int id)
{
  if (Note::isValidID(id))
  {
    if (id == m_view->getLastID())
      m_view->setLastID(INVALID_NOTE_ID);
    Note note = m_view->getNote();
    if (id == note.m_id)
      create();
  }
}

void NoteWidget::openLast(int id)
{
  Note note;
  QString error;
  if (NoteSQL::select(m_database->get(), id, note, error))
  {
    setNote(note);
  }
  else
  {
    QMessageBox msgBox(QMessageBox::Warning,
                       tr("Erro"),
                       tr("Erro '%1' ao abrir a nota com ID '%2'.").arg(error, QString::number(id)),
                       QMessageBox::Ok);
    msgBox.exec();
  }
}
