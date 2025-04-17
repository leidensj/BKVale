#include "sectorview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

SectorView::SectorView(QWidget* parent)
  : JItemView(SECTOR_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
{
  addViewButton(IMAGE_SQL_TABLE_NAME);

  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(SECTOR_MAX_NAME_LENGTH);

  m_imagePicker = new DatabasePicker(IMAGE_SQL_TABLE_NAME);

  QFormLayout* ltForm = new QFormLayout;
  ltForm->addRow(tr("Nome:"), m_edName);
  ltForm->addRow(tr("Imagem:"), m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_tab->addTab(tabframe, QIcon(":/icons/res/sector.png"), tr("Setor"));
  setFocusWidgetOnClear(m_edName);
  clear();
}

void SectorView::getItem(JItemSQL& o) const
{
  Sector& _o = dynamic_cast<Sector&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_name = m_edName->text();
}

void SectorView::setItem(const JItemSQL& o)
{
  const Sector& _o = static_cast<const Sector&>(o);
  m_edName->setText(_o.m_name);
  m_imagePicker->addItem(_o.m_image);
}
