#include "reminderwidget.h"
#include <QLayout>
#include <QSplitter>
#include <QMessageBox>

ReminderWidget::ReminderWidget(QWidget *parent)
  : QFrame(parent)
  , m_view(this)
  , m_db(this)
{
  QHBoxLayout* hlayout = new QHBoxLayout();
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  m_db.layout()->setContentsMargins(0, 0, 9, 0);
  splitter->addWidget(&m_db);
  splitter->addWidget(&m_view);
  hlayout->addWidget(splitter);
  setLayout(hlayout);

  QObject::connect(&m_view,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(&m_db,
                   SIGNAL(selectedSignal(const Reminder&)),
                   &m_view,
                   SLOT(setReminder(const Reminder&)));
}

void ReminderWidget::emitChangedSignal()
{
  emit changedSignal();
}

bool ReminderWidget::print(QSerialPort& printer)
{
  QString str(ReminderPrinter::build(m_view.reminder()));
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
  QString error;
  bool bSuccess = m_db.insertOrUpdate(m_view.reminder(), error);
  if (!bSuccess)
  {
    QMessageBox::warning(this,
                         tr("Erro"),
                         tr("Erro '%1' ao salvar o lembrete.").arg(error),
                         QMessageBox::Ok);
  }
  else
  {
    m_db.refresh();
  }
  return bSuccess;
}

void ReminderWidget::setDatabase(QSqlDatabase db)
{
  m_db.setDatabase(db);
}

void ReminderWidget::clear()
{
  m_view.clear();
}
