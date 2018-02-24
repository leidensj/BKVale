#include "phonenumberpageview.h"
#include "jlineedit.h"
#include <QSpinBox>
#include <QRadioButton>
#include <QLayout>
#include <QLabel>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QFormLayout>

PhoneNumberPageView::PhoneNumberPageView(QWidget* parent)
  : QFrame(parent)
  , m_edNumber(nullptr)
  , m_rdoComercial(nullptr)
  , m_rdoMobile(nullptr)
  , m_spnCountryCode(nullptr)
  , m_spnCode(nullptr)
  , m_edDescription(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_list(nullptr)
{
  m_edNumber = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edNumber->setPlaceholderText(tr("*"));
  m_rdoComercial = new QRadioButton();
  m_rdoComercial->setText(tr("Fixo"));
  m_rdoComercial->setIcon(QIcon(":/icons/res/phonecomercial.png"));
  m_rdoComercial->setChecked(true);
  m_rdoMobile = new QRadioButton();
  m_rdoMobile->setText(tr("Móvel"));
  m_rdoMobile->setIcon(QIcon(":/icons/res/phonemobile.png"));
  m_spnCountryCode = new QSpinBox();
  m_spnCountryCode->setMinimum(0);
  m_spnCountryCode->setMaximum(999);
  m_spnCountryCode->setValue(55);
  m_spnCountryCode->setPrefix("+");
  m_spnCode = new QSpinBox();
  m_spnCode->setMinimum(0);
  m_spnCode->setMaximum(999);
  m_spnCode->setValue(54);
  m_edDescription = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDescription->setPlaceholderText(tr("Ex. 'Telefone do irmão' ou 'Casa'"));
  m_btnAdd = new QPushButton();
  m_btnAdd->setFlat(true);
  m_btnAdd->setIconSize(QSize(16, 16));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar número (Alt++)"));
  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setIconSize(QSize(16, 16));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover número (Alt+-)"));
  m_list = new QListWidget();

  QGroupBox* typeGroupBox = new QGroupBox();
  typeGroupBox->setTitle(tr("Tipo de telefone"));
  QHBoxLayout* typeLayout = new QHBoxLayout();
  typeLayout->addWidget(m_rdoComercial);
  typeLayout->addWidget(m_rdoMobile);
  typeGroupBox->setLayout(typeLayout);

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_spnCountryCode);
  hlayout0->addWidget(m_spnCode);
  hlayout0->addWidget(new QLabel(tr("Número:")));
  hlayout0->addWidget(m_edNumber);

  QFormLayout* flayout0 = new QFormLayout();
  flayout0->setContentsMargins(0, 0, 0, 0);
  flayout0->addRow(tr("Descrição:"), m_edDescription);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnAdd);
  hlayout1->addWidget(m_btnRemove);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(typeGroupBox);
  vlayout0->addLayout(hlayout0);
  vlayout0->addLayout(flayout0);
  vlayout0->addLayout(hlayout1);
  vlayout0->addWidget(m_list);
  setLayout(vlayout0);
  m_edNumber->setFocus();

  QObject::connect(m_rdoMobile,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_rdoComercial,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  updateControls();
  m_edNumber->setFocus();
}

void PhoneNumberPageView::updateControls()
{
  QString mask = m_rdoMobile->isChecked()
                 ? "99999-9999;_"
                 : "9999-9999;_";
  m_edNumber->setInputMask(mask);
  m_edNumber->setFocus();
}
