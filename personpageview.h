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
class QGroupBox;
class QLabel;

class PersonPageView : public QFrame
{
  Q_OBJECT

public:
  explicit PersonPageView(QWidget* parent = 0);
  Person getPerson() const;
  void setPerson(const Person& person,
                 const QString& imageName,
                 const QByteArray& arImage);
  void setImage(int id, const QString& name, const QByteArray& ar);

private slots:
  void switchUserType();
  void updateControls();
  void emitSearchImageSignal();

public slots:
  void clear();

signals:
  void changedSignal();
  void searchImageSignal();

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
  JPicker* m_imagePicker;
  QCheckBox* m_cbCustomer;
  QCheckBox* m_cbSupplier;
  QCheckBox* m_cbEmployee;
  QDateEdit* m_dtCreationDate;
};

#endif // PERSONPAGEVIEW_H
