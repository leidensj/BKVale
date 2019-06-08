#include "packageeditor.h"
#include <QLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QIcon>
#include <QDialogButtonBox>
#include "widgets/jlineedit.h"
#include "widgets/jdoublespinbox.h"

PackageEditor::PackageEditor(const Package& package,
                             const QString& productUnity,
                             QWidget* parent)
  : QDialog(parent)
  , m_grpIsPackage(nullptr)
  , m_edUnity(nullptr)
  , m_spnAmmount(nullptr)
{
  m_grpIsPackage = new QGroupBox;
  m_grpIsPackage->setCheckable(true);
  m_grpIsPackage->setChecked(false);
  m_grpIsPackage->setTitle(tr("Utilizar outra unidade"));

  m_edUnity = new JLineEdit(JLineEdit::Input::Alphanumeric, JLineEdit::st_defaultFlags1);
  m_edUnity->setMaxLength(PRODUCT_MAX_UNITY_LENGTH);
  m_spnAmmount = new JDoubleSpinBox(true);
  m_spnAmmount->setValue(1.0);
  m_spnAmmount->setMinimum(0.0);
  m_spnAmmount->setMaximum(999999.0);

  QFormLayout* flayout = new QFormLayout;
  flayout->addRow(tr("Unidade:"), m_edUnity);
  flayout->addRow(tr("Quantidade:"), m_spnAmmount);

  m_grpIsPackage->setLayout(flayout);

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_grpIsPackage);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(tr("Modificar Embalagem"));
  setWindowIcon(QIcon(":/icons/res/item.png"));
  setWindowFlags(Qt::WindowCloseButtonHint);
  layout()->setSizeConstraint(QLayout::SetFixedSize);
  setPackage(package, productUnity);

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void PackageEditor::setPackage(const Package& package,
                               const QString& productUnity)
{
  m_grpIsPackage->setChecked(package.m_bIsPackage);
  m_edUnity->setText(package.m_unity);
  m_spnAmmount->setValue(package.m_ammount);
  m_spnAmmount->setSuffix(" " + productUnity);
}

Package PackageEditor::getPackage() const
{
  Package package;
  package.m_bIsPackage = m_grpIsPackage->isChecked();
  package.m_unity = m_edUnity->text();
  package.m_ammount = m_spnAmmount->value();
  return package;
}
