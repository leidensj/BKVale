#include "timecardcontroldialog.h"
#include <QDate>
#include "items/jitem.h"
#include "items/store.h"
#include "items/employee.h"
#include "controls/databasepicker.h"
#include "widgets/jdateinterval.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>


TimeCardControlDialog::TimeCardControlDialog(QWidget* parent)
 : QDialog(parent)
 , m_storePicker(nullptr)
 , m_employeePicker(nullptr)
 , m_date(nullptr)
 , m_buttons(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Gerenciar Livro Ponto"));
  setWindowIcon(QIcon(":/icons/res/pen.png"));
  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME);
  m_employeePicker = new DatabasePicker(EMPLOYEE_SQL_TABLE_NAME);
  m_date = new JDateInterval;
  m_date->setCheckable(false);

  m_buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

  connect(m_buttons, SIGNAL(accepted()), this, SLOT(saveAndAccept()));
  connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));

  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow(tr("Loja:"), m_storePicker);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(formLayout);
  ltMain->addWidget(m_date);
  ltMain->addWidget(m_buttons);
  ltMain->setAlignment(Qt::AlignTop);
  setLayout(ltMain);

  connect(m_storePicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_date, SIGNAL(changedSignal(bool, const QDate&, const QDate&);), this, SLOT(updateControls()));
  updateControls();
}

void TimeCardControlDialog::updateControls()
{
  m_employeePicker->getViewer()->setDynamicFilter("");
}

void TimeCardControlDialog::saveAndAccept()
{

}
