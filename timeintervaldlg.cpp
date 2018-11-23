#include "timeintervaldlg.h"
#include "jspinbox.h"
#include "jtimeedit.h"
#include <QObject>
#include <QPixmap>
#include <QLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>

TimeIntervalDlg::TimeIntervalDlg(QWidget* parent)
 : QDialog(parent)
{
  setWindowTitle(tr("Intervalos de Tempo"));
  setWindowIcon(QIcon(":/icons/res/timespan.png"));
  setMinimumWidth(300);
  m_spn = new JSpinBox;
  m_spn->setMinimum(1);
  m_spn->setMaximum(10);
  m_btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addWidget(m_spn);

  QHBoxLayout* ltHeader = new QHBoxLayout;
  ltHeader->setContentsMargins(0, 0, 0, 0);
  ltHeader->addWidget(new QLabel(tr("Início:")));
  ltHeader->addWidget(new QLabel(tr("Fim:")));

  ltMain->addLayout(ltHeader);

  for (int i = 0; i != 10; ++i)
  {
    m_tmBegin[i] = new JTimeEdit;
    m_tmEnd[i] = new JTimeEdit;
    m_tmBegin[i]->setDisplayFormat("hh:mm");
    m_tmEnd[i]->setDisplayFormat("hh:mm");
    QHBoxLayout* lt = new QHBoxLayout;
    lt->setContentsMargins(0, 0, 0, 0);
    lt->addWidget(m_tmBegin[i]);
    lt->addWidget(m_tmEnd[i]);
    ltMain->addLayout(lt);
  }

  ltMain->addWidget(m_btn);
  setLayout(ltMain);

  connect(m_spn, SIGNAL(valueChanged(int)), this, SLOT(adjustControls()));
  connect(m_btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_btn, SIGNAL(rejected()), this, SLOT(reject()));

  adjustControls();
  validate();
}

void TimeIntervalDlg::validate()
{

}

void TimeIntervalDlg::adjustControls()
{
  for (int i = m_spn->value(); i != 10; ++i)
  {
    m_tmBegin[i]->hide();
    m_tmEnd[i]->hide();
  }
  for (int i = 0; i != m_spn->value(); ++i)
  {
    m_tmBegin[i]->show();
    m_tmEnd[i]->show();
  }
  setMaximumHeight(sizeHint().height());
}

void TimeIntervalDlg::setItems(const QVector<TimeInterval>& v)
{

}

const QVector<TimeInterval>& TimeIntervalDlg::get() const
{

}
