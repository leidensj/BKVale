#include "personview.h"
#include "jlineedit.h"
#include "jpicker.h"
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

Person PersonView::getPerson() const
{
  return m_personPage->getPerson();
}

void PersonView::getPerson(Person& person,
                           QVector<Phone>& vPhone,
                           QVector<Address>& vAddress,
                           QVector<qlonglong>& vRemovedPhoneId,
                           QVector<qlonglong>& vRemoveAddressId) const
{
  person.clear();
  vPhone.clear();
  vAddress.clear();

  person = m_personPage->getPerson();
  vPhone = m_phonePage->getPhones();
  vAddress = m_addressPage->getAddresses();
  vRemovedPhoneId = m_phonePage->getRemovedPhones();
  vRemoveAddressId = m_addressPage->getRemovedAddresses();
}

void PersonView::setImage(int id, const QString& name, const QByteArray& ar)
{
  m_personPage->setImage(id, name, ar);
}

void PersonView::setPerson(const FullPerson &fPerson)
{
  m_currentPerson = fPerson.m_person;
  m_vCurrentPhone = fPerson.m_vPhone;
  m_vCurrentAddress = fPerson.m_vAddress;
  m_personPage->setPerson(fPerson.m_person, fPerson.m_image.m_name, fPerson.m_image.m_image);
  m_phonePage->setPhones(m_vCurrentPhone);
  m_addressPage->setAddresses(m_vCurrentAddress);
  updateControls();
}

void PersonView::create()
{
  m_currentPerson.clear();
  m_vCurrentPhone.clear();
  m_vCurrentAddress.clear();
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
  QVector<Phone> vPhone = m_phonePage->getPhones();
  QVector<Address> vAddress = m_addressPage->getAddresses();
  bool bEnable = person.isValid() &&
                 !vPhone.isEmpty() &&
                 !vAddress.isEmpty();
  QString saveIcon(":/icons/res/save.png");
  if (m_currentPerson.isValidId())
  {
    saveIcon = ":/icons/res/saveas.png";
    bEnable = bEnable && m_currentPerson != person;
    if (!bEnable)
    {
      bEnable &= m_vCurrentPhone != vPhone;
      if (!bEnable)
        bEnable &= m_vCurrentAddress != vAddress;
    }
  }

  m_btnSave->setEnabled(bEnable);
  m_btnSave->setIcon(QIcon(saveIcon));
}
