#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QFrame>
#include <QVector>
#include "person.h"
#include "jitemview.h"

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
class AddressTableWidget;

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

public slots:
  void create();

private:
  mutable Person m_ref;
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

  PhoneTableWidget* m_tblPhone;
  QPushButton* m_btnPhoneAdd;
  QPushButton* m_btnPhoneRemove;

  AddressTableWidget* m_tblAddress;
  QPushButton* m_btnAddressAdd;
  QPushButton* m_btnAddressRemove;

  QCheckBox* m_cbNoteEdit;
  QCheckBox* m_cbNoteRemove;

  QGroupBox* m_grpEmployee;
  QGroupBox* m_grpSupplier;

  void setItem(const JItem &o);
};

#endif // PERSONVIEW_H
