#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QFrame>
#include <QVector>
#include "address.h"
#include "phone.h"
#include "person.h"

class QPushButton;
class PersonPageView;
class PhonePageView;
class AddressPageView;
class QTabWidget;

class PersonView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonView(QWidget* parent = 0);
  ~PersonView();
  Person getPerson() const;
  void getPerson(Person& person,
                 QVector<Phone>& vPhone,
                 QVector<Address>& vAddress,
                 QVector<qlonglong>& vRemovedPhoneId,
                 QVector<qlonglong>& vRemoveAddressId) const;
  void setImage(int id, const QString& name, const QByteArray& ar);

private slots:
  void emitSearchImageSignal();
  void emitSaveSignal();
  void updateControls();

public slots:
  void setPerson(const FullPerson& fPerson);
  void create();

signals:
  void searchImageSignal();
  void saveSignal();

private:
  Person m_currentPerson;
  QVector<Phone> m_vCurrentPhone;
  QVector<Address> m_vCurrentAddress;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  PersonPageView* m_personPage;
  PhonePageView* m_phonePage;
  AddressPageView* m_addressPage;
};

#endif // PERSONVIEW_H
