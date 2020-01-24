#include "packageeditor.h"
#include <QLayout>
#include <QLabel>
#include <QFormLayout>
#include <QGroupBox>
#include <QIcon>
#include <QDialogButtonBox>
#include "widgets/jlineedit.h"
#include "widgets/jdoublespinbox.h"

PackageEditor::PackageEditor(bool bUnityEditor, QWidget* parent)
  : QDialog(parent)
  , m_bUnityEditor(bUnityEditor)
  , m_grpIsPackage(nullptr)
  , m_edUnity(nullptr)
  , m_edAmmount(nullptr)
  , m_lblUnity(nullptr)
{
  m_grpIsPackage = new QGroupBox;
  m_grpIsPackage->setCheckable(m_bUnityEditor ? false : true);
  m_grpIsPackage->setChecked(false);
  m_grpIsPackage->setTitle(tr("Utilizar outra unidade"));

  m_edUnity = new JLineEdit(Text::Input::Alphanumeric, JLineEdit::st_defaultFlags1);
  m_edUnity->setMaxLength(PRODUCT_MAX_UNITY_LENGTH);

  m_edAmmount = new JExpLineEdit(Data::Type::Ammount);
  m_lblUnity = new QLabel;

  QHBoxLayout* ltAmmount = new QHBoxLayout;
  ltAmmount->setContentsMargins(0, 0, 0, 0);
  ltAmmount->setAlignment(Qt::AlignLeft);
  ltAmmount->addWidget(m_edAmmount);
  ltAmmount->addWidget(m_lblUnity);

  QFormLayout* flayout = new QFormLayout;
  flayout->addRow(tr("Unidade:"), m_edUnity);
  flayout->addRow(tr("Quantidade:"), ltAmmount);

  m_grpIsPackage->setLayout(flayout);

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_grpIsPackage);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(m_bUnityEditor ? tr("Modificar Unidade") : tr("Modificar Embalagem"));
  setWindowIcon(QIcon(":/icons/res/package.png"));
  setWindowFlags(Qt::WindowCloseButtonHint);
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void PackageEditor::setPackage(const Package& package,
                               const QString& productUnity)
{
  if (!m_bUnityEditor)
    m_grpIsPackage->setChecked(package.m_bIsPackage);
  m_edUnity->setText(package.m_unity);
  m_edAmmount->setText(package.m_ammount);
  m_lblUnity->setText(productUnity);
}

Package PackageEditor::getPackage() const
{
  Package package;
  package.m_bIsPackage = m_bUnityEditor ? true : m_grpIsPackage->isChecked();
  package.m_unity = m_edUnity->text();
  package.m_ammount = m_edAmmount->getValue();
  return package;
}
