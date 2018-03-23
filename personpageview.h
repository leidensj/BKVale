#ifndef PERSONPAGEVIEW_H
#define PERSONPAGEVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include "person.h"

class JLineEdit;
class JDatabasePicker;
class QDateEdit;
class QCheckBox;
class QRadioButton;
class QComboBox;
class QGroupBox;
class QLabel;

class PersonPageView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonPageView(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  Person getPerson() const;
  void setPerson(const Person& person);

private slots:
  void switchUserType();
  void updateControls();

public slots:
  void clear();

signals:
  void changedSignal();

private:
  Person m_currentPerson;
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
  QGroupBox* m_grpBirthDate;
  QDateEdit* m_dtBirthDate;
  JDatabasePicker* m_imagePicker;
  QCheckBox* m_cbCustomer;
  QCheckBox* m_cbSupplier;
  QCheckBox* m_cbEmployee;
  QDateEdit* m_dtCreationDate;
  JLineEdit* m_edPinCode;
};

#endif // PERSONPAGEVIEW_H
