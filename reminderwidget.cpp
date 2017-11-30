#include "reminderwidget.h"
#include "reminderview.h"
#include "reminderdatabase.h"
#include <QLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QDockWidget>

ReminderWidget::ReminderWidget(QWidget *parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_db(nullptr)
  , m_dock(nullptr)
{
  m_view = new ReminderView();
  m_db = new ReminderDatabase();
  m_dock = new QDockWidget();
  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle("Pesquisar");
  m_dock->setWidget(m_db);

  QHBoxLayout* hlayout = new QHBoxLayout();
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  m_db->layout()->setContentsMargins(9, 9, 9, 9);
  splitter->addWidget(m_dock);
  splitter->addWidget(m_view);
  hlayout->addWidget(splitter);
  setLayout(hlayout);

  m_dock->close();

  QObject::connect(m_view,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_db,
                   SIGNAL(selectedSignal(const Reminder&)),
                   m_view,
                   SLOT(setReminder(const Reminder&)));

  QObject::connect(m_view,
                   SIGNAL(searchClickedSignal()),
                   this,
                   SLOT(showDock()));

  QObject::connect(m_db,
                   SIGNAL(removedSignal(int)),
                   this,
                   SLOT(removed(int)));
}

void ReminderWidget::emitChangedSignal()
{
  emit changedSignal();
}

void ReminderWidget::showDock()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

bool ReminderWidget::print(QSerialPort& printer)
{
  QString str(ReminderPrinter::build(m_view->reminder()));
  QString error;
  bool bSuccess = Printer::print(printer, str, error);
  if (!bSuccess)
  {
    QMessageBox::warning(this,
                         tr("Erro"),
                         tr("Erro '%1' ao imprimir o lembrete.").arg(error),
                         QMessageBox::Ok);
  }
  return bSuccess;
}

bool ReminderWidget::save()
{
  if (!m_view->isSaveChecked())
    return true;

  QString error;
  bool bSuccess = m_db->insertOrUpdate(m_view->reminder(), error);
  if (!bSuccess)
  {
    QMessageBox::warning(this,
                         tr("Erro"),
                         tr("Erro '%1' ao salvar o lembrete.").arg(error),
                         QMessageBox::Ok);
  }
  else
  {
    m_db->refresh();
  }
  return bSuccess;
}

void ReminderWidget::setDatabase(QSqlDatabase db)
{
  m_db->setDatabase(db);
}

void ReminderWidget::clear()
{
  m_view->clear();
}

bool ReminderWidget::isValid() const
{
  return m_view->isValid();
}

void ReminderWidget::removed(int id)
{
  if (m_view->reminder().m_id == id)
    m_view->clear();
}
