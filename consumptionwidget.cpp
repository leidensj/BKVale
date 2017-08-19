#include "consumptionwidget.h"
#include <QLayout>
#include "consumptionview.h"
#include "consumptiondatabase.h"

ConsumptionWidget::ConsumptionWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_database(nullptr)
{
  m_view = new ConsumptionView();
  m_database = new ConsumptionDatabase();

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  vlayout->addWidget(m_view);
  vlayout->addWidget(m_database);
  setLayout(vlayout);
}

void ConsumptionWidget::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db);
}
