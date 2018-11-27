#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QFrame>
#include <QVector>
#include "address.h"
#include "phone.h"
#include "person.h"
#include "jitemview.h"


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
class QGroupBox;
class PhoneTableWidget;

Q_DECLARE_METATYPE(Address)

class PersonView : public JItemView
{
  Q_OBJECT

public:
  explicit PersonView(bool bAccessEmployee,
                      bool bAccessSupplier,
                      QWidget* parent = 0);
  ~PersonView();
  const JItem& getItem() const;

private slots:
  void updateControls();
  void switchUserType();
  Address getAddress() const;
  void addAddress();
  void addAddress(const Address& address);
  void removeAddress();
  void openAddress();
  void clearAddress();
  void processPostalCode();

public slots:
  void create();

private:
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

  PhoneTableWidget* m_phoneTable;
  QPushButton* m_btnPhoneAdd;
  QPushButton* m_btnPhoneRemove;

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

  QCheckBox* m_cbNoteEdit;
  QCheckBox* m_cbNoteRemove;

  QGroupBox* m_grpEmployee;
  QGroupBox* m_grpSupplier;

  void setItem(const JItem &o);
};

#endif // PERSONVIEW_H
