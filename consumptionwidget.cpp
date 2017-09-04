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

  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWidget(m_filter);
  m_filter->layout()->setAlignment(Qt::AlignTop);
  m_dock->close();

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  vlayout->addWidget(m_view);
  vlayout->addWidget(m_database);

  QWidget* w = new QWidget();
  w->setLayout(vlayout);
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(w);

  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->setContentsMargins(0, 0, 0, 0);
  hlayout->addWidget(splitter);
  setLayout(hlayout);

  QObject::connect(m_view,
                   SIGNAL(insertSignal(const Consumption&)),
                   m_database,
                   SLOT(insert(const Consumption&)));

  QObject::connect(m_filter,
                   SIGNAL(changedSignal(bool, qint64, qint64)),
                   m_database,
                   SLOT(setFilter(bool,qint64,qint64)));

  QObject::connect(m_database,
                   SIGNAL(totalSignal(double)),
                   m_filter,
                   SLOT(updateTotal(double)));

  QObject::connect(m_database,
                   SIGNAL(filterSignal()),
                   this,
                   SLOT(showFilter()));

  QObject::connect(m_filter,
                   SIGNAL(chartSignal(bool, qint64, qint64)),
                   m_database,
                   SLOT(processChartData(bool,qint64,qint64)));

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
