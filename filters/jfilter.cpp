#include "jfilter.h"
#include <QLayout>
#include <QPushButton>
#include <QFrame>

JFilter::JFilter(QWidget* parent)
  : QWidget(parent)
  , m_ltButton(nullptr)
  , m_btnClear(nullptr)
  , m_fr(nullptr)
{
  m_ltButton = new QHBoxLayout;
  m_ltButton->setContentsMargins(0, 0, 0, 0);
  m_ltButton->setAlignment(Qt::AlignLeft);

  m_btnClear = new QPushButton;
  m_btnClear->setFlat(true);
  m_btnClear->setIconSize(QSize(24, 24));
  m_btnClear->setIcon(QIcon(":/icons/res/filterclear.png"));
  m_btnClear->setToolTip(tr("Limpar"));

  m_ltButton->addWidget(m_btnClear);

  m_fr = new QFrame;

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(m_ltButton);
  ltMain->addWidget(m_fr);
  setLayout(ltMain);

  connect(m_btnClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
}

void JFilter::emitFilterChangedSignal()
{
  emit filterChangedSignal(getFilter());
}
