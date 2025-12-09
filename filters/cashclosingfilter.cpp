#include "cashclosingfilter.h"
#include "defines.h"
#include "controls/databasepicker.h"
#include "widgets/jdatetimeinterval.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QFrame>

CashClosingFilter::CashClosingFilter(QWidget* parent)
  : JFilter(parent)
  , m_cash(nullptr)
  , m_dt(nullptr)
{
  m_cash = new DatabasePicker(CASH_SQL_TABLE_NAME, true);
  m_dt = new JDateTimeInterval;

  QFormLayout* ltcash = new QFormLayout;
  ltcash->addRow(tr("Caixa:"), m_cash);
  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltcash);
  ltMain->addWidget(m_dt);
  ltMain->setAlignment(Qt::AlignTop);

  m_fr->setLayout(ltMain);

  connect(m_cash, SIGNAL(changedSignal()), this, SLOT(emitFilterChangedSignal()));
  connect(m_dt, SIGNAL(changedSignal()), this, SLOT(emitFilterChangedSignal()));
  CashClosingFilter::clear();
}

QString CashClosingFilter::getFilter() const
{
  QString str;
  Ids ids = m_cash->getIds();
  if (!ids.isEmpty())
  {
    str += " " CASH_CLOSING_SQL_COL_CID
                 " IN (";
    for (const auto& id : ids)
      str += id.str() + ",";
    str.chop(1);
    str += ") ";
  }

  if (m_dt->isChecked())
  {
    if (!str.isEmpty())
      str += " AND ";
    str += CASH_CLOSING_SQL_COL_DAT " BETWEEN '" +
           m_dt->getInitialDateTime().toString(Qt::ISODate) + "' AND '" +
           m_dt->getFinalDateTime().toString(Qt::ISODate) + "' ";
  }

  return str;
}

void CashClosingFilter::clear()
{
  m_cash->clear();
  m_dt->clear();
}
