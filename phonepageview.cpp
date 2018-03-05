#include "phonepageview.h"
#include "jlineedit.h"
#include <QPushButton>
#include <QSpinBox>
#include <QListWidget>
#include <QLayout>

PhonePageView::PhonePageView(QWidget *parent)
  : QFrame(parent)
  , m_currentItem(nullptr)
  , m_spnCountryCode(nullptr)
  , m_spnCode(nullptr)
  , m_edNumber(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_btnUndo(nullptr)
  , m_btnRemove(nullptr)
  , m_list(nullptr)
{
  m_edNumber = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edNumber->setPlaceholderText(tr("*"));
  m_spnCountryCode = new QSpinBox;
  m_spnCountryCode->setMinimum(0);
  m_spnCountryCode->setMaximum(999999);
  m_spnCountryCode->setPrefix("+");
  m_spnCountryCode->setValue(55);
  m_spnCode = new QSpinBox;
  m_spnCode->setMinimum(0);
  m_spnCode->setMaximum(999999);
  m_spnCode->setPrefix("(");
  m_spnCode->setValue(54);
  m_spnCode->setSuffix(")");
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setIconSize(QSize(16, 16));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setToolTip(tr("Novo"));
  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setIconSize(QSize(16, 16));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setToolTip(tr("Salvar"));
  m_btnUndo = new QPushButton;
  m_btnUndo->setFlat(true);
  m_btnUndo->setIconSize(QSize(16, 16));
  m_btnUndo->setIcon(QIcon(":/icons/res/undo.png"));
  m_btnUndo->setToolTip(tr("Desfazer"));
  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setIconSize(QSize(16, 16));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnRemove->setToolTip(tr("Remover"));
  m_list = new QListWidget();

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);
  buttonlayout->addWidget(m_btnUndo);
  buttonlayout->addWidget(m_btnRemove);

  QHBoxLayout* phonelayout = new QHBoxLayout;
  phonelayout->setContentsMargins(0, 0, 0, 0);
  phonelayout->addWidget(m_spnCountryCode);
  phonelayout->addWidget(m_spnCode);
  phonelayout->addWidget(m_edNumber);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addLayout(buttonlayout);
  mainlayout->addLayout(phonelayout);
  mainlayout->addWidget(m_list);
  setLayout(mainlayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearInputOnly()));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedPhone()));
  QObject::connect(m_btnUndo,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(undo()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_list,
                   SIGNAL(currentRowChanged(int)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_list,
                   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this,
                   SLOT(openSelectedPhone()));
  QObject::connect(m_edNumber,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_spnCountryCode,
                   SIGNAL(valueChanged(int)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_spnCode,
                   SIGNAL(valueChanged(int)),
                   this,
                   SLOT(updateControls()));
  updateControls();
}

void PhonePageView::save()
{
  Phone phone = getPhone();
  savePhone(phone);
}

void PhonePageView::updateControls()
{
  bool bEditMode = m_currentItem != nullptr;
  m_list->setEnabled(!bEditMode);
  m_btnCreate->setEnabled(!bEditMode);
  m_btnUndo->setEnabled(bEditMode);
  m_btnRemove->setEnabled(!bEditMode && m_list->currentRow() != -1);
  QString saveIcon = bEditMode
                     ? ":/icons/res/saveas.png"
                     : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(saveIcon));
  Phone phone = getPhone();
  bool bEnableSave = phone.isValid();
  if (bEditMode)
    bEnableSave = bEnableSave &&
                  m_currentItem->data(Qt::UserRole).value<Phone>() != phone;
  m_btnSave->setEnabled(bEnableSave);

  emit changedSignal();
}

void PhonePageView::undo()
{
  if (m_currentItem != nullptr)
    clearInputOnly();
  updateControls();
}

void PhonePageView::savePhone(const Phone& phone)
{
  QVariant var;
  var.setValue(phone);
  QListWidgetItem* p = m_currentItem != nullptr
                       ? m_currentItem
                       : new QListWidgetItem;
  p->setText(phone.getFormattedPhone());
  p->setData(Qt::UserRole, var);
  if (m_currentItem == nullptr)
    m_list->addItem(p);
  m_currentItem = nullptr;
  clearInputOnly();
  updateControls();
}

void PhonePageView::openSelectedPhone()
{
  QListWidgetItem* p = m_list->item(m_list->currentRow());
  if (p != nullptr)
  {
    m_currentItem = p;
    setPhone(p->data(Qt::UserRole).value<Phone>());
    updateControls();
  }
}

void PhonePageView::removeSelectedPhone()
{
  QListWidgetItem* p = m_list->takeItem(m_list->currentRow());
  if (p != nullptr)
  {
    m_vRemovedPhoneId.push_back(p->data(Qt::UserRole).value<Phone>().m_id);
    delete p;
    updateControls();
  }
}

void PhonePageView::clear()
{
  m_vRemovedPhoneId.clear();
  clearInputOnly();
  m_list->clear();
  updateControls();
}

void PhonePageView::clearInputOnly()
{
  m_currentItem = nullptr;
  setPhone(Phone());
  updateControls();
}

void PhonePageView::setPhone(const Phone& phone)
{
  m_spnCountryCode->setValue(phone.m_countryCode);
  m_spnCode->setValue(phone.m_code);
  m_edNumber->setText(phone.m_number);
  updateControls();
}

Phone PhonePageView::getPhone()
{
  Phone phone;
  if (m_currentItem != nullptr)
    phone.m_id = m_currentItem->data(Qt::UserRole).value<Phone>().m_id;
  phone.m_countryCode = m_spnCountryCode->value();
  phone.m_code = m_spnCode->value();
  phone.m_number = m_edNumber->text();
  return phone;
}

QVector<Phone> PhonePageView::getPhones() const
{
  QVector<Phone> vPhone;
  for (int i = 0; i != m_list->count(); ++i)
    vPhone.push_back(m_list->item(i)->data(Qt::UserRole).value<Phone>());
  return vPhone;
}

QVector<int> PhonePageView::getRemovedPhones() const
{
  return m_vRemovedPhoneId;
}

void PhonePageView::setPhones(const QVector<Phone>& vPhone)
{
  clear();
  for (int i = 0; i != vPhone.size(); ++i)
    savePhone(vPhone.at(i));
}
