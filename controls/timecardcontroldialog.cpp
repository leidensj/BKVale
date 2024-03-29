#include "timecardcontroldialog.h"
#include <QDate>
#include "items/jitem.h"
#include "items/store.h"
#include "items/employee.h"
#include "controls/databasepicker.h"
#include "tables/timecardcontroltable.h"
#include <QDateEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QComboBox>

TimeCardControlDialog::TimeCardControlDialog(QWidget* parent)
 : QDialog(parent)
 , m_storePicker(nullptr)
 , m_cbEmployee(nullptr)
 , m_date(nullptr)
 , m_table(nullptr)
 , m_buttons(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Gerenciar Livro Ponto"));
  setWindowIcon(QIcon(":/icons/res/pen.png"));
  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME);
  m_cbEmployee = new QComboBox;
  m_date = new QDateEdit;
  m_date->setDate(QDate::currentDate());
  m_date->setDisplayFormat("MMMM yyyy");
  m_table = new TimeCardControlTable;

  m_buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

  connect(m_buttons, SIGNAL(accepted()), this, SLOT(saveAndAccept()));
  connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));

  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow(tr("Loja:"), m_storePicker);
  formLayout->addRow(tr("Funcionário:"), m_cbEmployee);
  formLayout->addRow(tr("Data:"), m_date);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(formLayout);
  ltMain->addWidget(m_table);
  ltMain->addWidget(m_buttons);
  ltMain->setAlignment(Qt::AlignTop);
  setLayout(ltMain);

  connect(m_storePicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_date, SIGNAL(changedSignal(bool, const QDate&, const QDate&);), this, SLOT(updateControls()));
  updateControls();
}

void TimeCardControlDialog::updateControls()
{
  m_cbEmployee->clear();
  Store o;
  o.m_id = m_storePicker->getFirstId();
  bool bSuccess = false;
  if (o.m_id.isValid())
  {
    QString error;
    if (o.SQL_select(error))
    {
      for (int i = 0; i != o.m_vEmployee.size(); ++i)
        m_cbEmployee->addItem(o.m_vEmployee.at(i).name(), o.m_vEmployee.at(i).m_id.get());
      bSuccess = true;

    }
  }

  m_table->removeAllItems();
  if (bSuccess)
  {
    m_cbEmployee->setEnabled(bSuccess);
    m_date->setEnabled(bSuccess);
    m_table->setEnabled(bSuccess);
    for (int i = 0; i != m_date->date().daysInMonth(); ++i)
      m_table->addRow();
  }
}

void TimeCardControlDialog::saveAndAccept()
{

}
