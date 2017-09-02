#include "consumptionwidget.h"
#include <QLayout>
#include <QDialog>
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

  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->setContentsMargins(0, 0, 0, 0);
  hlayout->addWidget(m_view);
  hlayout->addWidget(m_filter);
  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  vlayout->addLayout(hlayout);
  vlayout->addWidget(m_database);
  setLayout(vlayout);

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

void ConsumptionWidget::showChart(const QVector<qint64>& dates,
                                  const QVector<double>& totals)
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ConsumptionChart* chart = new ConsumptionChart(dates, totals);
  layout->addWidget(chart);
  dlg.resize(540, 280);
  dlg.setWindowTitle(tr("Buscar Produto"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  dlg.exec();
}
