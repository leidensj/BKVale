#include "personview.h"
#include "jlineedit.h"
#include "jpicker.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>

PersonView::PersonView(QWidget* parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_edDetails(nullptr)
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

  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edName->setMaxLength(MAX_PERSON_NAME_LENGTH);
  m_edName->setPlaceholderText("*");

  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDetails->setMaxLength(MAX_PERSON_DETAILS_LENGTH);

  m_imagePicker = new JPicker(INVALID_IMAGE_ID, tr("Imagem"), true);

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnCreate);
  hlayout0->addWidget(m_btnSave);

  QFormLayout* flayout0 = new QFormLayout();
  flayout0->setContentsMargins(0, 0, 0, 0);
  flayout0->addRow(tr("Nome:"), m_edName);
  flayout0->addRow(tr("Detalhes:"), m_edDetails);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->setAlignment(Qt::AlignTop);
  vlayout1->addLayout(hlayout0);
  vlayout1->addLayout(flayout0);
  vlayout1->addWidget(m_imagePicker);
  setLayout(vlayout1);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSaveSignal()));

  QObject::connect(m_imagePicker,
                   SIGNAL(searchSignal()),
                   this,
                   SLOT(emitSearchImageSignal()));

  QObject::connect(m_edName,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));

  updateControls();
}

PersonView::~PersonView()
{

}

Person PersonView::getPerson() const
{
  Person person;
  return person;
}

void PersonView::setImage(int id, const QString& name, const QByteArray& ar)
{
  m_imagePicker->setId(id);
  m_imagePicker->setText(name);
  m_imagePicker->setImage(ar);
}

void PersonView::setPerson(const Person &person,
                           const QString& imageName,
                           const QByteArray& arImage)
{
  m_currentPerson = person;
  m_edName->setText(m_currentPerson.m_name);
  m_edDetails->setText(m_currentPerson.m_details);
  setImage(m_currentPerson.m_imageId, imageName, arImage);
  updateControls();
}

void PersonView::create()
{
  Person person;
  setPerson(person, "", QByteArray());
  updateControls();
}

void PersonView::emitSearchImageSignal()
{
  emit searchImageSignal();
}

void PersonView::emitSaveSignal()
{
  emit saveSignal();
}

void PersonView::updateControls()
{
  Person person = getPerson();
  bool bEnable = person.isValid();
  QString saveIcon(":/icons/res/save.png");
  if (m_currentPerson.isValidId())
  {
    saveIcon = ":/icons/res/saveas.png";
    bEnable = bEnable && m_currentPerson != person;
  }
  m_btnSave->setEnabled(bEnable);
  m_btnSave->setIcon(QIcon(saveIcon));
}
