#include "itemview.h"
#include "jlineedit.h"
#include <QLayout>
#include <QPushButton>
#include <QDoubleSpinBox>

ItemView::ItemView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ITEM_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_edUnity(nullptr)
  , m_edPackageUnity(nullptr)
  , m_sbPackageAmmount(nullptr)
  , m_edDetails(nullptr)
  , m_edMidasCode(nullptr)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton();
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_edName = new JLineEdit(true, true);
  m_edName->setMaxLength(MAX_ITEM_NAME_LENGTH);
  m_edName->setPlaceholderText(tr("Nome"));

  m_edUnity = new JLineEdit(true, true);
  m_edUnity->setMaxLength(MAX_ITEM_UNITY_LENGTH);
  m_edUnity->setPlaceholderText(tr("Unidade"));

  m_edPackageUnity = new JLineEdit(true, true);
  m_edPackageUnity->setMaxLength(MAX_ITEM_PACKAGE_UNITY_LENGTH);
  m_edPackageUnity->setPlaceholderText(tr("Unidade da Embalagem"));

  m_sbPackageAmmount = new QDoubleSpinBox();
  m_sbPackageAmmount->setMaximum(MAX_ITEM_PACKAGE_AMMOUNT_LENGTH);
  m_sbPackageAmmount->setMinimum(0.0);
  m_sbPackageAmmount->setSpecialValueText(tr("Quantidade da Embalagem"));

  m_edDetails = new JLineEdit(true, true);
  m_edDetails->setMaxLength(MAX_ITEM_DETAILS_LENGTH);
  m_edDetails->setPlaceholderText(tr("Detalhes"));

  m_edMidasCode = new JLineEdit(true, true);
  m_edMidasCode->setMaxLength(MAX_ITEM_MIDASCODE_LENGTH);
  m_edMidasCode->setPlaceholderText(tr("Código Midas"));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_btnCreate);
  hlayout0->addWidget(m_btnSave);

  QVBoxLayout* vlayout0 = new QVBoxLayout;
  vlayout0->addLayout(hlayout0);
  vlayout0->addWidget(m_edName);
  vlayout0->addWidget(m_edUnity);
  vlayout0->addWidget(m_edPackageUnity);
  vlayout0->addWidget(m_sbPackageAmmount);
  vlayout0->addWidget(m_edDetails);
  vlayout0->addWidget(m_edMidasCode);
  setLayout(vlayout0);
}

Item ItemView::getItem() const
{
  Item item;
  item.m_id = m_currentId;
  item.m_name = m_edName->text();
  item.m_unity = m_edUnity->text();
  item.m_packageUnity = m_edPackageUnity->text();
  item.m_packageAmmount = m_sbPackageAmmount->value();
  item.m_details = m_edDetails->text();
  item.m_midasCode = m_edMidasCode->text();
  item.m_icon = INVALID_ITEM_ICON;
}

void ItemView::setItem(const Item &item)
{
  m_currentId = item.m_id;
  m_edName->setText(item.m_name);
  m_edUnity->setText(item.m_unity);
  m_edPackageUnity->setText(item.m_packageUnity);
  m_sbPackageAmmount->setValue(item.m_packageAmmount);
  m_edDetails->setText(item.m_details);
  m_edMidasCode->setText(item.m_midasCode);
}

void ItemView::create()
{
  m_currentId = INVALID_ITEM_ID;
  m_edName->setText("");
  m_edUnity->setText("");
  m_edPackageUnity->setText("");
  m_sbPackageAmmount->setValue(0.0);
  m_edDetails->setText("");
  m_edMidasCode->setText("");
}
