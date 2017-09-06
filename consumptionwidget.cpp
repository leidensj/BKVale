#include "consumptionwidget.h"
#include <QLayout>
#include <QDialog>
#include <QDockWidget>
#include <QSplitter>
#include "consumptionview.h"
#include "consumptiondatabase.h"
#include "consumptionfilter.h"
#include "consumptionchart.h"

ConsumptionWidget::ConsumptionWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_database(nullptr)
{
  m_view = new ConsumptionView();
  m_database = new ConsumptionDatabase();
  m_filter = new ConsumptionFilter();
  m_dock = new QDockWidget();

  m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  m_filter->layout()->setAlignment(Qt::AlignTop);

  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle("Filtros");
  m_dock->setWidget(m_filter);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(m_database);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  vlayout->addWidget(m_view);
  vlayout->addWidget(splitter);
  setLayout(vlayout);

  m_dock->close();

  QObject::connect(m_view,
                   SIGNAL(insertSignal(const Consumption&)),
                   m_database,
                   SLOT(insert(const Consumption&)));

  QObject::connect(m_filter,
                   SIGNAL(changedSignal(const Consumption::Filter&)),
                   m_database,
                   SLOT(setFilter(const Consumption::Filter&)));

  QObject::connect(m_database,
                   SIGNAL(filterSignal()),
                   this,
                   SLOT(showFilter()));

  QObject::connect(m_database,
                   SIGNAL(chartSignal(const QVector<qint64>&,
                                      const QVector<double>&)),
                   this,
                   SLOT(showChart(const QVector<qint64>&,
                                  const QVector<double>&)));
}

void ConsumptionWidget::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db);
}

void ConsumptionWidget::showFilter()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

void ConsumptionWidget::showChart(const QVector<qint64>& dates,
                                  const QVector<double>& totals)
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ConsumptionChart* chart = new ConsumptionChart(dates, totals);
  layout->addWidget(chart);
  dlg.resize(640, 480);
  dlg.setWindowTitle(tr("Análise de Consumo"));
  dlg.setWindowIcon(QIcon(":/icons/res/chart.png"));
  dlg.setWindowFlags(Qt::Window);
  dlg.setModal(true);
  dlg.exec();
}
