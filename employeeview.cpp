#include "employeeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include "jspinbox.h"
#include "jtimeedit.h"
#include <QPushButton>
#include "timeinterval.h"

TimeIntervalWidget::TimeIntervalWidget(QWidget* parent)
  : QWidget(parent)
{
  setWindowTitle(tr("Intervalos de Tempo"));
  setWindowIcon(QIcon(":/icons/res/timespan.png"));
  setMinimumWidth(300);
  m_spn = new JSpinBox;
  m_spn->setMinimum(0);
  m_spn->setMaximum(10);

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

  setLayout(ltMain);

  connect(m_spn, SIGNAL(valueChanged(int)), this, SLOT(adjustControls()));

  adjustControls();
  validate();
}

void TimeIntervalWidget::setItems(const QVector<TimeInterval>& v)
{
  for (int i = 0; i != (v.size() < 10 ? v.size() : 10); ++i)
  {
    m_tmBegin[i]->setTime(v.at(i).m_tmBegin);
    m_tmEnd[i]->setTime(v.at(i).m_tmEnd);
  }
}

QVector<TimeInterval> TimeIntervalWidget::getItems() const
{
  QVector<TimeInterval> v;
  for (int i = 0; i != m_spn->value(); ++i)
  {
    TimeInterval o;
    o.m_tmBegin = m_tmBegin[i]->time();
    o.m_tmEnd = m_tmEnd[i]->time();
    v.push_back(o);
  }
  return v;
}

void TimeIntervalWidget::validate()
{

}

void TimeIntervalWidget::adjustControls()
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

EmployeeView::EmployeeView(QWidget* parent)
  : JItemView(EMPLOYEE_SQL_TABLE_NAME, parent)
  , m_formPicker(nullptr)
  , m_storePicker(nullptr)
  , m_edPincode(nullptr)
  , m_cbNoteEdit(nullptr)
  , m_cbNoteRemove(nullptr)
  , m_time(nullptr)
{
  m_formPicker = new JDatabasePicker(FORM_SQL_TABLE_NAME);
  m_formPicker->getDatabase()->setFixedFilter(EMPLOYEE_UNIQUE_FORM_FILTER);
  m_storePicker = new JDatabasePicker(STORE_SQL_TABLE_NAME);
  m_edPincode = new JLineEdit(JLineEdit::Input::Alphanumeric);
  m_cbNoteEdit = new QCheckBox;
  m_cbNoteEdit->setText(tr("Criar e editar"));
  m_cbNoteEdit->setIcon(QIcon(":/icons/res/file.png"));
  m_cbNoteRemove = new QCheckBox;
  m_cbNoteRemove->setText(tr("Remover"));
  m_cbNoteRemove->setIcon(QIcon(":/icons/res/remove.png"));

  QFormLayout* lt = new QFormLayout;
  lt->addRow(m_formPicker->getText() + ":", m_formPicker);
  lt->addRow(m_storePicker->getText() + ":", m_storePicker);
  lt->addRow(tr("Código PIN:"), m_edPincode);
  lt->addRow(tr("Vales:"), m_cbNoteEdit);
  lt->addRow("", m_cbNoteRemove);

  QFrame* fr = new QFrame;
  fr->setLayout(lt);

  m_time = new TimeIntervalWidget;

  QVBoxLayout* ltHours = new QVBoxLayout;
  ltHours->addWidget(m_time);
  ltHours->addStretch();

  QFrame* frHours = new QFrame;
  frHours->setLayout(ltHours);

  m_tab->addTab(fr, QIcon(":/icons/res/employee.png"), tr("Funcionário"));
  m_tab->addTab(frHours, QIcon(":/icons/res/clock.png"), tr("Horário"));
}

void EmployeeView::create()
{
  selectItem(Employee());
}

const JItemSQL& EmployeeView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_form.m_id = m_formPicker->getId();
  m_ref.m_store.m_id = m_storePicker->getId();
  m_ref.m_pincode = m_edPincode->text();
  m_ref.m_bNoteEdit = m_cbNoteEdit->isChecked();
  m_ref.m_bNoteRemove = m_cbNoteRemove->isChecked();
  m_ref.m_hours = m_time->getItems();
  return m_ref;
}

void EmployeeView::setItem(const JItemSQL& o)
{
  auto ref = static_cast<const Employee&>(o);
  m_currentId = o.m_id;
  m_formPicker->setItem(ref.m_form);
  m_storePicker->setItem(ref.m_store);
  m_edPincode->setText(ref.m_pincode);
  m_cbNoteEdit->setChecked(ref.m_bNoteEdit);
  m_cbNoteRemove->setChecked(ref.m_bNoteRemove);
  m_time->setItems(ref.m_hours);
}
