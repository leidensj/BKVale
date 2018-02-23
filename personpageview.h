#ifndef PERSONPAGEVIEW_H
#define PERSONPAGEVIEW_H

#include <QFrame>
#include "person.h"

class JLineEdit;
class JPicker;
class QDateEdit;
class QCheckBox;
class QRadioButton;
class QComboBox;

class PersonPageView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonPageView(QWidget* parent = 0);

private slots:
  void updateControls();

private:
  Person m_currentPerson;
  QRadioButton* m_rdoPerson;
  QRadioButton* m_rdoCompany;
  JLineEdit* m_edName;
  JLineEdit* m_edAlias;
  JLineEdit* m_edEmail;
  JLineEdit* m_edCpfCnpj;
  JLineEdit* m_edRgIE;
  JLineEdit* m_edDetails;
  QDateEdit* m_dtBirthDate;
  JPicker* m_imagePicker;
  QCheckBox* m_cbCustomer;
  QCheckBox* m_cbSupplier;
  QCheckBox* m_cbEmployee;
  QFrame* m_frPinCode;
  JLineEdit* m_edPinCode;
  QComboBox* m_cbState;
};

#endif // PERSONPAGEVIEW_H
