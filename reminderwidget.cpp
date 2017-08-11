#include "reminderwidget.h"
#include <QLayout>
#include <QSplitter>

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
}

void ReminderWidget::emitChangedSignal()
{
  emit changedSignal();
}
