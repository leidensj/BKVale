#ifndef FORMVIEW_H
#define FORMVIEW_H

#include <QFrame>
#include <QVector>
#include "form.h"
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

class FormView : public JItemView
{
  Q_OBJECT

public:
  explicit FormView(QWidget* parent = 0);
  ~FormView();
  const JItemSQL& getItem() const;

private slots:
  void updateControls();
  void switchUserType();

public slots:
  void create();

private:
  mutable Form m_ref;
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

  PhoneTableWidget* m_tblPhone;
  QPushButton* m_btnPhoneAdd;
  QPushButton* m_btnPhoneRemove;

  AddressTableWidget* m_tblAddress;
  QPushButton* m_btnAddressAdd;
  QPushButton* m_btnAddressRemove;

  void setItem(const JItemSQL &o);
};

#endif // FORMVIEW_H
