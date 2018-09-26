#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QFrame>
#include <QVector>
#include "address.h"
#include "phone.h"
#include "person.h"

class QPushButton;
class AddressPageView;
class QTabWidget;
class QLabel;
class QRadioButton;
class JLineEdit;
class QDateEdit;
class JDatabasePicker;
class QCheckBox;
class QSpinBox;
class QListWidget;
class QComboBox;
class JDatabase;
class QGroupBox;

Q_DECLARE_METATYPE(Phone)
Q_DECLARE_METATYPE(Address)

class PersonView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonView(bool bAccessEmployee,
                      bool bAccessSupplier,
                      QWidget* parent = 0);
  ~PersonView();
  Person getPerson() const;

private slots:
  void updateControls();
  void switchUserType();
  Phone getPhone() const;
  void addPhone();
  void addPhone(const Phone& phone);
  void removePhone();
  void openPhone();
  void clearPhone();
  Address getAddress() const;
  void addAddress();
  void addAddress(const Address& address);
  void removeAddress();
  void openAddress();
  void clearAddress();
  void processPostalCode();

  void itemSelected(const JItem& jItem);
  void itemsRemoved(const QVector<qlonglong>& ids);
  void save();

public slots:
  void setPerson(const Person& person);
  void create();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  AddressPageView* m_addressPage;

  QRadioButton* m_rdoPerson;
  QRadioButton* m_rdoCompany;
  JLineEdit* m_edName;
  QLabel* m_lblName;
  JLineEdit* m_edAlias;
  QLabel* m_lblAlias;
  JLineEdit* m_edEmail;
  JLineEdit* m_edCpfCnpj;
  QLabel* m_lblCpfCnpj;
  JLineEdit* m_edRgIE;
  QLabel* m_lblRgIE;
  JLineEdit* m_edDetails;
  QDateEdit* m_dtBirthDate;
  QCheckBox* m_cbBirthDate;
  JDatabasePicker* m_imagePicker;
  QDateEdit* m_dtCreationDate;
  JLineEdit* m_edPinCode;

  QSpinBox* m_spnPhoneCountryCode;
  QSpinBox* m_spnPhoneCode;
  JLineEdit* m_edPhoneNumber;
  JLineEdit* m_edPhoneName;
  QPushButton* m_btnAddPhone;
  QPushButton* m_btnRemovePhone;
  QListWidget* m_lstPhone;

  JLineEdit* m_edAddressPostalCode;
  QPushButton* m_btnAddressPostalCode;
  JLineEdit* m_edAddressNeighborhood;
  JLineEdit* m_edAddressStreet;
  QSpinBox* m_spnAddressNumber;
  JLineEdit* m_edAddressCity;
  QComboBox* m_cbAddressState;
  JLineEdit* m_edAddressComplement;
  JLineEdit* m_edAddressReference;
  QPushButton* m_btnAddAddress;
  QPushButton* m_btnRemoveAddress;
  QListWidget* m_lstAddress;

  JDatabase* m_database;
  QTabWidget* m_tab;
  QGroupBox* m_grpEmployee;
  QGroupBox* m_grpSupplier;
};

#endif // PERSONVIEW_H
