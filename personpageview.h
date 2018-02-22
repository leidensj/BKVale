#ifndef PERSONPAGEVIEW_H
#define PERSONPAGEVIEW_H

#include <QFrame>
#include "person.h"

class JLineEdit;
class JPicker;
class QDateEdit;
class QCheckBox;
class QRadioButton;

class PersonPageView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonPageView(QWidget* parent = 0);

private:
  QRadioButton* m_rdoPerson;
  QRadioButton* m_rdoCompany;
  Person m_currentPerson;
  JLineEdit* m_edName;
  JLineEdit* m_edAlias;
  JLineEdit* m_edEmail;
  JLineEdit* m_edCpfCnpj;
  JLineEdit* m_edRgInsc;
  JLineEdit* m_edDetails;
  QDateEdit* m_dtBirthDate;
  JPicker* m_imagePicker;
  QCheckBox* m_cbClient;
  QCheckBox* m_cbSupplier;
  QCheckBox* m_cbEmployee;
  QFrame* m_frPinCode;
  JLineEdit* m_edPinCode;
};

#endif // PERSONPAGEVIEW_H
