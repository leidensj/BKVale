#include "personview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "personpageview.h"
#include "phonepageview.h"
#include "addresspageview.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>

PersonView::PersonView(QWidget* parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_personPage(nullptr)
  , m_phonePage(nullptr)
  , m_addressPage(nullptr)
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

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnCreate);
  hlayout0->addWidget(m_btnSave);

  m_personPage = new PersonPageView();
  m_phonePage = new PhonePageView();
  m_addressPage = new AddressPageView();

  QTabWidget* tabWidget = new QTabWidget();
  tabWidget->addTab(m_personPage,
                    QIcon(":/icons/res/resume.png"),
                    tr("Informações"));
  tabWidget->addTab(m_phonePage,
                    QIcon(":/icons/res/phone.png"),
                    tr("Telefones"));
  tabWidget->addTab(m_addressPage,
                    QIcon(":/icons/res/address.png"),
                    tr("Endereços"));

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->setAlignment(Qt::AlignTop);
  vlayout1->addLayout(hlayout0);
  vlayout1->addWidget(tabWidget);
  setLayout(vlayout1);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSaveSignal()));
  QObject::connect(m_personPage,
                   SIGNAL(searchImageSignal()),
                   this,
                   SLOT(emitSearchImageSignal()));
  QObject::connect(m_personPage,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_phonePage,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_addressPage,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
  updateControls();
}

PersonView::~PersonView()
{

}

void PersonView::setDatabase(QSqlDatabase db)
{
  m_personPage->setDatabase(db);
}

Person PersonView::getPerson() const
{
  return m_personPage->getPerson();
}

void PersonView::getPerson(Person& person) const
{
  person.clear();
  person = m_personPage->getPerson();
  person.m_vAddress = m_addressPage->getAddresses();
  person.m_vPhone = m_phonePage->getPhones();
}

void PersonView::setPerson(const Person &person)
{
  m_currentPerson = person;
  m_personPage->setPerson(person);
  m_phonePage->setPhones(person.m_vPhone);
  m_addressPage->setAddresses(person.m_vAddress);
  updateControls();
}

void PersonView::create()
{
  m_currentPerson.clear();
  m_personPage->clear();
  m_phonePage->clear();
  m_addressPage->clear();
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
  Person person = m_personPage->getPerson();
  person.m_vPhone = m_phonePage->getPhones();
  person.m_vAddress = m_addressPage->getAddresses();
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
